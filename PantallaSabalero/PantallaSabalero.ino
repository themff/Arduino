//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display




int boton = 0;

void setup()
{


  pinMode(4,INPUT);
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();

}  
void loop()

{
  boton = digitalRead(4);
  if(boton==1)
  {  


  lcd.setCursor(0,0);
  lcd.print("No hace falta que ");
  lcd.setCursor(0,1);
  lcd.print("les digan que soy ");
  lcd.setCursor(0,2);
  lcd.print("raza");
  delay(3550);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Por mi piel ");
  lcd.setCursor(0,1);
  lcd.print("escapa el alma ");
  lcd.setCursor(0,2);
  lcd.print("sabalera");
  delay(3600);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Sangre y luto ");
  lcd.setCursor(0,1);
  lcd.print("es el color de");
  lcd.setCursor(0,2);
  lcd.print("mi bandera");
  delay(3600);
  lcd.clear();

  
  lcd.setCursor(0,0);
  lcd.print("Y a los negros ");
  lcd.setCursor(0,1);
  lcd.print("yo los llevo aqui");
  lcd.setCursor(0,2);
  lcd.print("en el alma.");
  delay(3640);
  lcd.clear();

  
  lcd.setCursor(0,0);
  lcd.print("Es el negro un ");
  lcd.setCursor(0,1);
  lcd.print("sentimiento que ");
  lcd.setCursor(0,2);
  lcd.print("se lleva de");
  lcd.setCursor(0,3);
  lcd.print("verdad");
  delay(4000);
  lcd.clear();

  
  lcd.setCursor(0,0);
  lcd.print("Y que late al ");
  lcd.setCursor(0,1);
  lcd.print("compas del");
  lcd.setCursor(0,2);
  lcd.print("corazon");
  delay(3240);
  lcd.clear();

  
  lcd.setCursor(0,0);
  lcd.print("Si ganara yo al  ");
  lcd.setCursor(0,1);
  lcd.print("infierno por ser");
  lcd.setCursor(0,2);
  lcd.print("negro y nada mas");
  delay(4000);
  lcd.clear();

  
  lcd.setCursor(0,0);
  lcd.print("Morire llevando ");
  lcd.setCursor(0,1);
  lcd.print("negro el corazooon");
  delay(3200);
  lcd.clear();


    
  lcd.setCursor(0,0);
  lcd.print("Sabale, Sabale");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("La culpa es de este");
  lcd.setCursor(0,1);
  lcd.print("pueblo sabalero");
  delay(2200);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sabale, Sabale");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Que todo el mundo");
  lcd.setCursor(0,1);
  lcd.print("grite DALE NEGRO!");
  delay(2600);
  lcd.clear();
  lcd.setCursor(7,1);
  lcd.print("AEEEEEA");
  delay(1200);
  lcd.clear();


  
  lcd.setCursor(7,1);
  lcd.print("YO SOY  ");
  lcd.setCursor(6,2);
  lcd.print("SABALERO"); 
  delay(2200);
  lcd.clear();

  lcd.setCursor(7,1);
  lcd.print("AEEEEEA");
  delay(1600);
  lcd.clear();

  lcd.setCursor(6,1);
  lcd.print("SABALERO");
  delay(1000);
  lcd.setCursor(6,2);
  lcd.print("SABALERO");
  delay(1000);
  lcd.clear();


  
  }


}
