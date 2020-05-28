#include <Stepper.h>      // incluye libreria stepper

#define inputCLK 4
#define inputDT 5
#define inputSW 3

 // LED Outputs
 #define ledCW 12
 #define ledCCW 13
 
 int counter = 0; 
 int currentStateCLK;
 int previousStateCLK; 
 int buttonState = 0;
 
 String encdir ="";
 
Stepper motor1(2048, 8, 10, 9, 11);   // pasos completos
 
void setup() {
  motor1.setSpeed(11);       // en RPM (valores de 1, 2 o 3 para 28BYJ-48)
  Serial.begin(9600);     // incializacion de comunicacion serie a 9600 bps

  // Set encoder pins as inputs  
   pinMode (inputCLK,INPUT);
   pinMode (inputDT,INPUT);
   pinMode (inputSW,INPUT);
   
   // Set LED pins as outputs
   pinMode (ledCW,OUTPUT);
   pinMode (ledCCW,OUTPUT);
   
   // Setup Serial Monitor
   Serial.begin (9600);
   
   // Read the initial state of inputCLK
   // Assign to previousStateCLK variable
   previousStateCLK = digitalRead(inputCLK);
}
 
void loop() {
  // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);
    
   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){ 
       
     // If the inputDT state is different than the inputCLK state then 
     // the encoder is rotating counterclockwise
     if (digitalRead(inputDT) != currentStateCLK) { 
       counter --;
       encdir ="CCW";
       digitalWrite(ledCW, LOW);
       digitalWrite(ledCCW, HIGH);
       
     } else {
       // Encoder is rotating clockwise
       counter ++;
       encdir ="CW";
       digitalWrite(ledCW, HIGH);
       digitalWrite(ledCCW, LOW);
       
     }
     Serial.print("Direction: ");
     Serial.print(encdir);
     Serial.print(" -- Value: ");
     Serial.println(counter);
   } 
   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK; 

  buttonState = digitalRead(inputSW); 
  delay
  if (buttonState==LOW)
  {      // espera por datos disponibles en monitor serie  
    int pasos = counter;  // obtiene valor introducido en monitor serie y asigna
          // a variable pasos
    Serial.println(pasos);    // imprime el valor ingresado
    motor1.step(pasos);     // envia comando para dar la cantidad de pasos
    
    digitalWrite(8, LOW);     // desenergiza todas las bobinas
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    
  }
    
}
