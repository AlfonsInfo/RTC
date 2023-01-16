#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>

Servo myServo;
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "78789";

const int x = A0; // analog pin connected to X output
const int y = A1; // analog pin connected to Y output
const int z = A7;
const int switchPin = 11; // analog pin connected to Y output


char carDir = 'S';
char clawState = '1';
//char sendiState = '0';

int xValue, yValue, claw , potensioValue , servoPos = 0;

void setup() {
  setupRadio();
  pinMode(switchPin, INPUT);
  Serial.begin(9600);
//  myServo.attach(4);
}

void loop() {
  potensioValue = analogRead(z);
  xValue = analogRead(x);
  yValue = analogRead(y);
  claw = digitalRead(switchPin);
    
  if(xValue >= 505 && xValue<=515 && yValue >= 490 && yValue<=500)
    carDir = 'S'; //stop
  else if (xValue > 518 && yValue >= 254 && yValue <= 700)
    carDir = 'F'; //maju
  else if (xValue < 518 && yValue >= 254 && yValue <= 700)
    carDir = 'B'; //mundur
  else if (xValue >= 254 && xValue <= 700 && yValue < 518)
    carDir = 'L'; //kiri
  else if (xValue >= 254 && xValue <= 700 && yValue > 518)
    carDir = 'R'; //kanan

  if(claw == HIGH) clawState = '1';
  else clawState = '0';

  sendiState = potensioValue/3;
  const char data[6] = {carDir,clawState,sendiState,'\0'};
//  Serial.print(claw);
  Serial.println(data);
//  Serial.println(potensioValue/3);
  radio.write(&data, sizeof(data));
//  myServo.write(potensioValue/3);  
  delay(20);
}

void setupRadio(){
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
