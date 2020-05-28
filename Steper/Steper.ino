#include <Stepper.h>
/*#define STEPS 2048*/
 
Stepper stepper(2048, 2, 3, 4, 5);
 
void setup() {
  stepper.setSpeed(10);
}
 
void loop() {
  stepper.step(2048);
  delay(2000);
  stepper.step(-512);
  delay(2000);


  
}
