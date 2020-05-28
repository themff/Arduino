#include <Stepper.h>      // incluye libreria stepper
 
Stepper motor1(2048, 8, 10, 9, 11);   // pasos completos
 
void setup() {
  motor1.setSpeed(15);       // en RPM (valores de 1, 2 o 3 para 28BYJ-48)
}
 
void loop() {
  motor1.step(2048);         // cantidad de pasos
  delay(2000);        // demora de 2 seg. por cuestiones practicas
  motor1.step(-2048);        // signo menos indica giro en sentido opuesto
  delay(2000);        // demora de 2 seg. por cuestiones practicas
}
