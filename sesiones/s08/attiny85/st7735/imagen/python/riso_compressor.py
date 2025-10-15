from PIL import Image
import numpy as np

class GrayscaleATtiny85:
    def __init__(self, display_width=80, display_height=160):
        self.target_width = display_width
        self.target_height = display_height

    def prepare_image(self, image_path):
        """Ajusta la imagen al tamaño exacto 80x160 (horizontal)."""
        img = Image.open(image_path).convert('RGB')
        img = img.resize((self.target_width, self.target_height), Image.LANCZOS)
        return img

    def convert_to_grayscale(self, image):
        return image.convert('L')

    def apply_dithering(self, grayscale_img):
        """Dithering simple a 4 tonos de gris (0–3)."""
        img_array = np.array(grayscale_img)
        dither_matrix = np.array([
            [0, 8, 2, 10],
            [12, 4, 14, 6],
            [3, 11, 1, 9],
            [15, 7, 13, 5]
        ]) * 4

        output_array = np.zeros_like(img_array, dtype=np.uint8)
        for y in range(img_array.shape[0]):
            for x in range(img_array.shape[1]):
                pixel = img_array[y, x]
                threshold = dither_matrix[y % 4, x % 4]
                value = pixel + threshold - 32
                if value < 64:
                    output_array[y, x] = 0  # Negro
                elif value < 128:
                    output_array[y, x] = 1  # Gris oscuro
                elif value < 192:
                    output_array[y, x] = 2  # Gris claro
                else:
                    output_array[y, x] = 3  # Blanco
        return output_array

    def compress_2bpp(self, image_array):
        """Compresión 4 píxeles por byte (2 bits/píxel)."""
        compressed = []
        for y in range(self.target_height):
            for x in range(0, self.target_width, 4):
                byte_val = 0
                for n in range(4):
                    if x + n < self.target_width:
                        color_idx = image_array[y, x + n] & 0x03
                        byte_val |= color_idx << (n * 2)
                compressed.append(byte_val)
        return compressed


def generate_c_file(compressed_data, width, height, filename="attiny85_image.c"):
    """Genera el archivo C con la imagen comprimida."""
    palette_rgb565 = [
        0x0000,  # Negro
        0x52AA,  # Gris oscuro
        0xAD55,  # Gris claro
        0xFFFF   # Blanco
    ]

    c_code = f"""#include <avr/pgmspace.h>

const uint16_t PROGMEM palette[] = {{
    {', '.join([f'0x{c:04X}' for c in palette_rgb565])}
}};

const uint8_t PROGMEM image_data[] = {{
    {', '.join([f'0x{b:02X}' for b in compressed_data])}
}};

const uint8_t IMAGE_WIDTH = {width};
const uint8_t IMAGE_HEIGHT = {height};
"""
    with open(filename, "w") as f:
        f.write(c_code)

    total_bytes = len(compressed_data)
    print(f"✅ Archivo generado: {filename}")
    print(f"📊 Tamaño comprimido: {total_bytes} bytes ({total_bytes/1024:.1f} KB)")


# ===== USO =====
if __name__ == "__main__":
    image_path = "input_image.jpg"
    generator = GrayscaleATtiny85(80, 160)

    print("🔄 Procesando imagen...")
    img = generator.prepare_image(image_path)
    gray = generator.convert_to_grayscale(img)
    dithered = generator.apply_dithering(gray)
    compressed = generator.compress_2bpp(dithered)
    generate_c_file(compressed, 80, 160)
