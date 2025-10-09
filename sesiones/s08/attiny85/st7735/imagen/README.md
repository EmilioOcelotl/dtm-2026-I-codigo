# Imagen 

Instrucciones para comprimir una imagen a color para que quepa en un ATtiny85, usando técnicas risográficas:

1. Reducción de la resolución: 160x80 → 80x40 (4x menos píxeles)
2. Reducción de la paleta: RGB → 2-4 colores planos  
3. Aplicar diethering: Para simular tonos intermedios
4. Comprmir: 2 bits/píxel o planos de color

Para generar una imagen en formato digital que dé una idea aproximada del resultado, podemos usar [estas instrucciones](./python/README.md)

