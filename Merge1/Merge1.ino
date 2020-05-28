/*Acà vamos a mergear el final de carrera con el stepper. Arranca el programa,
 * El stepper busca el final de carrera. Cuando lo encuentra le pide al usuario que ingrese un valor, 
 * va hasta ese valor (que en teoría cuando esté armado sería el final) y vuelve.
 */

#include <Stepper.h>      // incluye libreria stepper
Stepper motor1(2048, 8, 10, 9, 11);   // pasos completos, pines de salida.
  
  int val = 0;
  int con = 0;
  int pasos = 0;

        
void setup() {

  motor1.setSpeed(11);       // en RPM (valores de 1, 2 o 3 para 28BYJ-48)
  pinMode(LED_BUILTIN, OUTPUT); //Led del arduino como chivo.
  pinMode(4,INPUT);             //Pin 4 entrada del Final de carrera
  

  Serial.begin(9600);
  
  

}

void loop() {

  pasos= -24; //Le doy un valor a pasos para que vuelva mientras busca el antirebote
  val = digitalRead(4); //leo el FC
  delay(2);           //Antirebote
  val = digitalRead(4); //leo el FC
  while(val == 0 && con == 0 ) // el con es mi flag, cuando toque el antirebote val sera 1 y con se pone a 1,
                               //asì cuando vuelve a pasar por el while, con nunca mas será 0.
  {  
  motor1.step(pasos); //arranca girando el motor hacia la izquierda en busca del FC.
  val = digitalRead(4); //leo el FC
  delay(2);           //Antirebote
  val = digitalRead(4); //leo el FC
  Serial.println("Buscando el antirebote");
  if(digitalRead(4)== 1 && con == 0)
    {
      Serial.println("Antirebote encontrado");
    }
  }

  
  con=1;
  
  digitalWrite(LED_BUILTIN, HIGH); //si sale del while es porque se pulsó.
  pasos= 0; 
  pasos = Serial.parseInt();  // obtiene valor introducido en monitor serie y asigna
                              // a variable pasos. Cuando sepamos la distancia del riel,
                              //este valor estará fijo para que vaya y venga.
  Serial.println("Ingrese valor"); //Acá le pido al usuario que me ingrese un valor para que vaya y vuelva.
  
  if(pasos!=0)
  {
  Serial.println("Moviendose hacia adelante ");    //  
  Serial.println(pasos);    // imprime el valor ingresado
  motor1.step(pasos);     // envia comando para dar la cantidad de pasos
    
    
    digitalWrite(8, LOW);     // desenergiza todas las bobinas
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    
  Serial.println("Volviendo ");  
  pasos = -pasos;           //seteo la vuelta
  motor1.step(pasos);

    digitalWrite(8, LOW);     // desenergiza todas las bobinas
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);

  Serial.println("Motor Detenido"); //Fin 
  }
}
