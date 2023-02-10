#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define CLIENT_ADDRESS 78
#define SERVER_ADDRESS 89

RH_NRF24 driver(7,8);

RHReliableDatagram manager(driver, CLIENT_ADDRESS); 

const int PIN_X = A0; // analog pin connected to X output
const int PIN_Y = A1; // analog pin connected to Y output
const int PIN_POTENSIO_FOR_ARM = A7; // Pin for potensio Meter
const int PIN_SWITCH_FOR_CLAW = 11; // analog pin connected to Y output

char carDir = 'S';
char clawState = '1';
char armState[16];
char mappingPotensioValue = 3;

int xValue, yValue, claw , potensioValue;

void setup() 
{
  pinMode(PIN_SWITCH_FOR_CLAW, INPUT);
  Serial.begin(9600);
  if (!manager.init())Serial.println("init failed");
  else Serial.println("init success");
}

uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void loop()
{
  potensioValue = analogRead(PIN_POTENSIO_FOR_ARM);
  xValue = analogRead(PIN_X);
  yValue = analogRead(PIN_Y);
  claw = digitalRead(PIN_SWITCH_FOR_CLAW);

  Serial.println(xValue);
  Serial.println(yValue);
    
  if(xValue >= 505 && xValue<=530 && yValue >= 490 && yValue<=530)
    carDir = 'S'; //stop
  else if (xValue > 518 && yValue >= 254 && yValue <= 700)
    carDir = 'B'; //maju
  else if (xValue < 518 && yValue >= 254 && yValue <= 700)
    carDir = 'F'; //mundur
  else if (xValue >= 254 && xValue <= 700 && yValue < 518)
    carDir = 'L'; //kiri
  else if (xValue >= 254 && xValue <= 700 && yValue > 518)
    carDir = 'R'; //kanan
  
  if(claw == HIGH) clawState = '1';
  else clawState = '0';

  potensioValue = potensioValue/mappingPotensioValue;
  itoa(potensioValue, armState,10);
  const char data[7] = {carDir,clawState,armState[0],armState[1],armState[2],'\0'};

  Serial.print("Arm State : ");
  Serial.println(armState);
  
  Serial.print(claw);
  Serial.println(data);
  
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.print("dibales sama : 0x");
      Serial.print(from, HEX);
      Serial.print(", katanya: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("No reply, is nrf24_reliable_datagram_server running?");
    }
  }
  else{
    Serial.println("sendtoWait failed");
    if (!manager.init())Serial.println("init failed");
    else Serial.println("init success");

  }
 
  delay(10);
}
