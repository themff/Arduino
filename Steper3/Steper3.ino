#include <Stepper.h>      // incluye libreria stepper
 
Stepper motor1(2048, 8, 10, 9, 11);   // pasos completos
 
void setup() {
  motor1.setSpeed(11);       // en RPM (valores de 1, 2 o 3 para 28BYJ-48)
  Serial.begin(9600);     // incializacion de comunicacion serie a 9600 bps
}
 
void loop() {
  if (Serial.available())
  {      // espera por datos disponibles en monitor serie  
    int pasos = Serial.parseInt();  // obtiene valor introducido en monitor serie y asigna
          // a variable pasos
    Serial.println(pasos);    // imprime el valor ingresado
    motor1.step(pasos);     // envia comando para dar la cantidad de pasos
    
    digitalWrite(8, LOW);     // desenergiza todas las bobinas
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }
}
