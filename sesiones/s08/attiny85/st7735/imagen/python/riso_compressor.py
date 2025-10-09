from PIL import Image, ImageDraw
import numpy as np

class RisoATtiny85Simulator:
    def __init__(self, max_width=80, max_height=80):
        self.max_width = max_width
        self.max_height = max_height
        self.target_width = 0
        self.target_height = 0
        
        self.palettes = {
            'classic': [(0,0,0), (255,50,50), (255,150,50), (255,255,255)],
            'ocean': [(0,0,0), (0,100,255), (50,200,255), (255,235,210)],
            'forest': [(0,0,0), (0,150,0), (100,255,100), (255,255,255)],
            'cyberpunk': [(0,0,0), (255,20,147), (0,255,255), (138,43,226)],
            'vintage': [(25,20,15), (120,90,70), (200,170,140), (240,235,220)],
            'cyber_green': [(0,0,0), (0,255,150), (100,255,255), (180,80,255)]
}
    
    def calculate_target_size(self, img_width, img_height):
        """Calcula tamaño objetivo manteniendo relación de aspecto"""
        if img_width > img_height:  # Horizontal
            self.target_width = self.max_width
            self.target_height = int((img_height * self.max_width) / img_width)
        else:  # Vertical o cuadrada
            self.target_height = self.max_height
            self.target_width = int((img_width * self.max_height) / img_height)
        
        print(f"Imagen original: {img_width}x{img_height}")
        print(f"Imagen reducida: {self.target_width}x{self.target_height}")
        
        # Asegurar dimensiones pares para la compresión
        self.target_width = (self.target_width // 4) * 4
        if self.target_height % 2 != 0:
            self.target_height += 1
            
        print(f"Imagen ajustada: {self.target_width}x{self.target_height} (para compresión)")
    
    def reduce_resolution(self, image_path):
        """PASO 1: Reducir resolución manteniendo relación de aspecto"""
        img = Image.open(image_path).convert('RGB')
        self.calculate_target_size(img.width, img.height)
        reduced_img = img.resize((self.target_width, self.target_height), Image.LANCZOS)
        return reduced_img
    
    def color_distance(self, color1, color2):
        """Distancia euclidiana entre colores"""
        return sum((a - b) ** 2 for a, b in zip(color1, color2))
    
    def apply_riso_dithering(self, image, palette_name='cyberpunk'):
        """PASO 2 & 3: Reducir paleta + aplicar dithering"""
        palette = self.palettes[palette_name]
        
        dither_matrix = np.array([
            [0,  8,  2,  10],
            [12, 4,  14, 6],
            [3,  11, 1,  9],
            [15, 7,  13, 5]
        ]) / 16.0
        
        output_img = Image.new('RGB', (self.target_width, self.target_height))
        pixels = output_img.load()
        
        for y in range(self.target_height):
            for x in range(self.target_width):
                original_pixel = image.getpixel((x, y))
                
                dither_value = dither_matrix[x % 4, y % 4] * 32
                dithered_pixel = (
                    min(255, max(0, original_pixel[0] + dither_value - 16)),
                    min(255, max(0, original_pixel[1] + dither_value - 16)),
                    min(255, max(0, original_pixel[2] + dither_value - 16))
                )
                
                closest_color = min(palette, 
                    key=lambda color: self.color_distance(dithered_pixel, color))
                
                pixels[x, y] = closest_color
        
        return output_img, palette
    
    def compress_to_2bpp(self, image):
        """PASO 4: Comprimir a 2 bits/píxel"""
        pixels = list(image.getdata())
        color_map = {}
        color_index = 0
        
        unique_colors = list(set(pixels))
        for color in unique_colors[:4]:
            color_map[color] = color_index
            color_index += 1
        
        compressed_data = []
        for i in range(0, len(pixels), 4):
            byte_val = 0
            for j in range(4):
                if i + j < len(pixels):
                    color_idx = color_map.get(pixels[i + j], 0)
                    byte_val |= (color_idx << (j * 2))
            compressed_data.append(byte_val)
        
        return compressed_data, color_map
    
    def simulate_attiny85_storage(self, compressed_data, color_map):
        """Verificar si cabe en ATtiny85"""
        flash_used = len(compressed_data)
        sram_used = len(color_map) * 3
        
        total_pixels = self.target_width * self.target_height
        original_size = total_pixels * 3
        compressed_size = len(compressed_data)
        
        print(f"=== ANÁLISIS DE MEMORIA ===")
        print(f"Resolución: {self.target_width}x{self.target_height} = {total_pixels} píxeles")
        print(f"Original (RGB): {original_size} bytes")
        print(f"Comprimido (2bpp): {compressed_size} bytes")
        print(f"Compresión: {compressed_size/original_size*100:.1f}% del original")
        print(f"Flash usado: {compressed_size}/8192 bytes ({compressed_size/8192*100:.1f}%)")
        print(f"SRAM usado: {sram_used}/512 bytes ({sram_used/512*100:.1f}%)")
        
        return flash_used <= 8192 and sram_used <= 512

def generate_c_code(compressed_data, color_map, width, height):
    """Genera código C listo para ATtiny85"""
    def rgb_to_rgb565(rgb):
        r, g, b = rgb
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
    
    palette_rgb565 = [rgb_to_rgb565(color) for color in color_map.keys()]
    
    c_code = f"""// Código generado automáticamente para ATtiny85
// Imagen: {width}x{height}, {len(compressed_data)} bytes, {len(color_map)} colores

#include <avr/pgmspace.h>

// Paleta de colores (RGB565)
const uint16_t PROGMEM palette[] = {{
    {', '.join([f'0x{color:04X}' for color in palette_rgb565])}
}};

// Datos de imagen comprimidos ({len(compressed_data)} bytes)
const uint8_t PROGMEM image_data[] = {{
    {', '.join([f'0x{byte:02X}' for byte in compressed_data])}
}};

void display_image() {{
    uint16_t pixel_index = 0;
    for(uint8_t y = 0; y < {height}; y++) {{
        for(uint8_t x = 0; x < {width}; x += 4) {{
            uint8_t data = pgm_read_byte(&image_data[pixel_index++]);
            for(uint8_t i = 0; i < 4; i++) {{
                uint8_t color_idx = (data >> (i * 2)) & 0x03;
                uint16_t color = pgm_read_word(&palette[color_idx]);
                draw_pixel(x + i, y, color);
            }}
        }}
    }}
}}
"""
    
    with open("attiny85_image.c", "w") as f:
        f.write(c_code)
    print("📄 Código C generado: 'attiny85_image.c'")

def create_test_vertical_image():
    """Crea una imagen VERTICAL de prueba"""
    img = Image.new('RGB', (80, 160), color='white')
    draw = ImageDraw.Draw(img)
    draw.rectangle([20, 40, 60, 120], fill='red')
    draw.ellipse([25, 10, 55, 50], fill='blue')
    draw.text((15, 130), "VERTICAL", fill='black')
    img.save("input_image.jpg")
    print("📸 Imagen VERTICAL de prueba creada: 'input_image.jpg'")
    return "input_image.jpg"

def main():
    simulator = RisoATtiny85Simulator(max_width=80, max_height=80)
    
    print("🚀 INICIANDO COMPRESIÓN PARA ATTINY85 (VERSIÓN ADAPTABLE)")
    print("=" * 50)
    
    try:
        image_path = "input_image.jpeg"
        reduced_img = simulator.reduce_resolution(image_path)
    except FileNotFoundError:
        print("📸 No se encontró 'input_image.jpg'. Creando imagen VERTICAL de prueba...")
        image_path = create_test_vertical_image()
        reduced_img = simulator.reduce_resolution(image_path)
    
    reduced_img.save("1_reduced.png")
    
    print("2. 🎨 APLICANDO PALETA RISOGRÁFICA + DITHERING")
    riso_image, palette = simulator.apply_riso_dithering(reduced_img, 'classic')
    riso_image.save("2_riso_style.png")
    
    print("3. 📦 COMPRIMIENDO A 2 BITS/PÍXEL")
    compressed_data, color_map = simulator.compress_to_2bpp(riso_image)
    
    print("4. 🔍 VERIFICANDO MEMORIA ATtiny85")
    is_viable = simulator.simulate_attiny85_storage(compressed_data, color_map)
    
    if is_viable:
        print("\n✅ ¡ÉXITO! La imagen cabe en ATtiny85")
        print(f"   - {len(compressed_data)} bytes en Flash")
        print(f"   - {len(color_map)} colores en SRAM")
        
        generate_c_code(compressed_data, color_map, simulator.target_width, simulator.target_height)
        
        print(f"\n📁 Archivos generados para imagen {simulator.target_width}x{simulator.target_height}:")
        print("   - 1_reduced.png")
        print("   - 2_riso_style.png")
        print("   - attiny85_image.c")
    else:
        print("\n❌ No cabe - necesitas más compresión")

if __name__ == "__main__":
    main()