# Desplegar imagen en ATtiny85 con ST7735S

## Navegador (más facil)

Instrucciones para comprimir la imagen desde el navegador: 

- Abre la herramienta en GitHub Pages
- Selecciona o arrastra una imagen
- Visualiza las previsualizaciones original y procesada
- Genera el archivo C
- Descarga el archivo attiny85_image.c

Enlace: https://emilioocelotl.github.io/conversor-imagenes-attiny85/

## Python

Instrucciones para comprimir una imagen en Python: 

### Requisitos

* Python 3.8 o superior.
* Arduino IDE con soporte para ATtiny85.
* Librerías de Python: `Pillow` y `numpy`.

---

### 1. Crear y activar entorno virtual

Desde la carpeta raíz de imagen (`imagen/`):

1. Crear el entorno virtual:

```bash
python -m venv venv
```

2. Activar el entorno:

* Linux/Mac:

```bash
source venv/bin/activate
```

* Windows:

```bash
venv\Scripts\activate
```

3. Instalar dependencias:

```bash
pip install Pillow numpy
```

---

### 2. Preparar la imagen

1. Coloca la imagen que quieres desplegar dentro de la carpeta `python/`.
2. Asegúrate de que tenga proporción similar a la pantalla (idealmente 80x160 píxeles).

---

### 3. Generar archivo `.c` para Arduino

Ejecuta el script desde la carpeta raíz:

```bash
python python/generate_image.py
```

El script hará lo siguiente:

* Redimensionar la imagen a 80x160 píxeles.
* Convertirla a escala de grises.
* Aplicar dithering a 4 tonos.
* Comprimirla en 2 bits por píxel (4 píxeles por byte).
* Generar `attiny85_image.c` dentro de la carpeta `python/`.

---

## Integrar con Arduino

Copia `python/attiny85_image.c` a la carpeta `arduino/` donde está tu sketch (Con Python o el navegador)

### Subir proyecto al ATtiny85

1. Conecta tu ATtiny85 al programador.
2. Selecciona la placa y programador correctos en Arduino IDE.
3. Sube el sketch.
4. La imagen debería desplegarse correctamente en la pantalla ST7735S.
