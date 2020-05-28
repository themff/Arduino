//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display



byte heart[] = {
  0x00,
  0x0A,
  0x1F,
  0x1F,
  0x0E,
  0x04,
  0x00,
  0x00
};





void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.createChar(1, heart);
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Celeste!!");
  lcd.setCursor(0,1);
  lcd.print("Me haces mal");
   lcd.setCursor(0,2);
  lcd.print("Pero me haces bien");
   lcd.setCursor(10,3);
  lcd.write(1);
   lcd.setCursor(11,3);
  lcd.print(""); 
}


void loop()
{
}
