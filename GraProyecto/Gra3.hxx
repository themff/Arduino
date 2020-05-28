#include <DHT.h>

const int ventilador=3;
const int sensorDHT=2;


int temp, humedad;
DHT dht (sensorDHT,DHT11);

void setup(){
  Serial.begin(9600);
  pinMode(ventilador,OUTPUT);
  dht.begin();
}

void loop (){
 
  humedad= dht.readHumidity();
  temp= dht.readTemperature();
 

  Serial.print("temperatura: ");
  Serial.print(temp);
  Serial.println("C");
  Serial.print("humedad: ");
  Serial.print(humedad);
  Serial.print("%");
  Serial.println(" ");
  Serial.println(" ");

  delay(1000);

 
 

  if(temp>=23)
    {
    Serial.println("ventilador encendido");
    digitalWrite(ventilador,HIGH);
    temp= dht.readTemperature();
    delay(1000);
    }
    else
    {
    Serial.println("ventilador Apagado");    
    digitalWrite(ventilador,LOW);
    }
        
 
}
