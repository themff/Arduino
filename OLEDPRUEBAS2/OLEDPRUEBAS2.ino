/*
  Capitulo 47 de Arduino desde cero en Español.
  Programa que muestra texto en la zona amarilla de la pantalla OLED y en la zona
  cyan los segundos transcurridos desde iniciado el programa
  Requiere instalar librerias Adafruit GFX y Adafruit SSD1306

  Autor: bitwiseAr  

*/

#include <Wire.h>     // libreria para bus I2C
#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
 
#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 64       // reemplaza ocurrencia de ALTO por 64

#define OLED_RESET -1      // necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto

void setup() {
  Wire.begin();         // inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C
}
 
void loop() {
  oled.clearDisplay();      // limpia pantalla
  oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  oled.setCursor(4, 0);     // ubica cursor en inicio de coordenadas 0,0
  oled.setTextSize(2);      // establece tamano de texto en 1
  oled.print("INC");  // escribe en pantalla el texto
  oled.setCursor(72, 0);     // ubica cursor en inicio de coordenadas 0,0
  oled.setTextSize(2);      // establece tamano de texto en 1
  oled.print("FWE"); 
  oled.setCursor (1, 17);    // ubica cursor en coordenas 10,30
  oled.setTextSize(5);      // establece tamano de texto en 2
  oled.print(millis() / 1000);    // escribe valor de millis() dividido por 1000
  oled.setCursor (66,17);
  oled.print(millis() / 1000);
  //oled.print(" seg.");      // escribe texto
  oled.display();     // muestra en pantalla todo lo establecido anteriormente
}
