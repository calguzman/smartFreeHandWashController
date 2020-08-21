#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// Inicialización de Radio PinOut 7 CE - 8 CS  Control Waiting Mode. 

const int CS = 6;
const int CE = 7;
RF24 radio(CE, CS);
const int timeOperationValvePin = A0; // 
int timeOperationValue; 
int timeElectroValveOperation;
const int electroValvePin=2;
// Configuración de Dirección.
const byte transmitterID[6]="00001";

void setup() {
  // Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0,transmitterID);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  // Serial.println("Init Receiver");
  // Define Electro Valve Pin
  pinMode(electroValvePin,OUTPUT);
  digitalWrite(electroValvePin,0);
}

void loop() {
  timeOperationValue = analogRead(timeOperationValvePin);
  timeElectroValveOperation = map(timeOperationValue, 0,1023,15000,1000);
  // Serial.println(timeElectroValveOperation);
  int handWashMode=0; 

  if(radio.available()){
    char message[32]="";
    radio.read(&message,sizeof(message));
    handWashMode = atoi(message);
    radio.stopListening();
    if((handWashMode==1)){
     // Turn On Electro Valve.
    //  Serial.println("Electro Valve TurnOn");
     digitalWrite(electroValvePin,HIGH);
     delay(timeElectroValveOperation);
    }
  } 
  // Serial.println("ElectroValve Turn Off");
  digitalWrite(electroValvePin,LOW);
  radio.startListening();
}