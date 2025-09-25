# ATTINY85

Instrucciones para [ATTINY85](https://uelectronics.com/producto/attiny85-tarjeta-de-desarrollo-kickstarter/). Es necesario seguir las instrucciones de la siguiente sección. 

## IDE de Arduino 

Requerirás los siguientes driver para poder usar la tarjeta de desarrollo ATtiny85, descargarlos [AQUI](https://uelectronics.com/wp-content/uploads/2018/12/Digistump.Drivers.zip), ya sea para 32/64 bits. Después procederemos a configurar en la suite IDE Arduino como a continuación se señala:

- Seleccionamos «Archivos/preferencias» en el IDE de Arduino.
- Encontramos el campo  “Gestor de URLs adicionales de tarjetas» .
- Pegamos la siguiente URL en el campo (usaremos una coma si hay alguna otra URL en la lista): http://digistump.com/package_digistump_index.json
- Clic OK para guardar.
- Seleccionamos «Herramientas/Placa/Gestor de tarjetas» y escribimos Digistump AVR Boards en el campo de búsqueda.
- Luego hacemos clic en Digistump AVR Boards by Digistump y seleccionamos instalar.
- Culminada la instalación seleccionamos cerrar.
- Regresa al menú Herramientas -> Placa y selecciona la placa «Digistump (Default – 16.5MHz)
- Ya puedes comenzar a programar en la tarjeta de desarrollo ATtiny85!!.

(Tomado de uelectronics)

NOTA IMPORTANTE: Para programar la placa primero es necesario dejar desconectada la placa y cuando la consola lo indique, conectar para subir el programa. 

NOTA IMPORTANTE 2: Estos códigos fueron probados en Windows, en Linux casi no sirven y en MacOS no hay información. 

## Primera prueba 

Para revisar que funciona podemos usar este [ejemplo](./example/example.ino)

En Digispark LED_BUILTIN no está definido y se define en 1.

## Pantalla ST7735

[Instrucciones](./st7735/ST7735.md) para usar la pantalla ST7735

## LEDs

[Código](./ledsPWM/ledsPWM.ino) para activar un pin analógico y hasta 3 pines PWM con ATTINY85. 
[Código](./ledsDigitales/ledsDigitales.ino) para activar un pin analógico y hasta 5 pines digitales con ATTINY85. 

## Neopixel

IMPORTANTE: Es necesaria una resistencia para el pin de datos e instalar la biblioteca. 

[Código](./neopixel/neopixel.ino) para usar un led neopixel con ATTINY85.

Este LED es RGB ¿En qué otras variaciones de color pueden pensar?