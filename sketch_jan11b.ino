  //koneksi joystick
#include <SPI.h>
  //Servo untuk capit
#include <PWMServo.h> //servo biasa conflict kalo pake radiohead
//#include <Servo.h>
//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t stickman [] = {0xe,0xe,0xe,0x15,0xe,0x4,0xa,0x11};  

#define revLeft 42
#define fwdLeft 44
#define revRight 4
#define fwdRight 5
#define ENAPin 6
#define ENBPin 3
#define speedMotorRight 100
#define speedMotorLeft 100

PWMServo capit; // object Servo
PWMServo arm;
//Servo capit;
char capitState = '0';
int pos = 0; // servo position

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>


#define CLIENT_ADDRESS 78
#define SERVER_ADDRESS 89

RH_NRF24 driver(7,8);

RHReliableDatagram manager(driver, SERVER_ADDRESS);

void setup() 
{
  pinMode(revLeft, OUTPUT); 
  pinMode(fwdLeft, OUTPUT);
  pinMode(revRight, OUTPUT);
  pinMode(fwdRight, OUTPUT);
    pinMode(ENAPin, OUTPUT);
  pinMode(ENBPin, OUTPUT);
  analogWrite(ENAPin, speedMotorRight);
  analogWrite(ENBPin, speedMotorLeft);
  
  
  lcd.begin();
  lcd.createChar (0,stickman);
 
  capit.attach(11);
  arm.attach(12);
  
  berhenti();
  
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
  else Serial.println("init success");

  lcd.setCursor(0,0);
  lcd.print("--ROBOTIC UAJY--");
  lcd.setCursor(0,1);
  lcd.print("ATMA JAYA YOGYA");
  lcd.write(byte(0));
}

uint8_t data[] = "iya bang";
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void loop()
{

  if (manager.available())
  {
//    Serial.println("manager available");
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.println((char*)buf); //ini diterima
      switch (buf[0]){
        case 'F':
          maju();
          break;
        case 'B':
          mundur();
          break;
         case 'R':
          belokKanan();
          break;
         case 'L':
          belokKiri();
          break;
         case 'S':
          berhenti();
          break;
         
      }
      
      switch (buf[1]){
        case '1':
            if(capitState == ((char*)buf[1])){ 
              Serial.print("Kondisi Sama");
              break; }
            else{
            bukaCapit();  
            capitState='1';
            }
            break;
        case '0':
          if(capitState == (char*)buf[1]){break;}
          else{
            tutupCapit();
            capitState='0'  ;
          }
          break;
      }

//      Serial.println(((char*)buf)[2]);
//      Serial.println(((char*)buf)[3]);
//      Serial.println(((char*)buf)[4]);
      String servoPosArm(((char*)buf)[2]);
      servoPosArm.concat(((char*)buf)[3]);
      servoPosArm.concat(((char*)buf)[4]);
//      int iservoPosArm;
      arm.write((servoPosArm.toInt())/2);
//      iservoPosArm = servoPosArm.toInt;
      Serial.println((servoPosArm));
      
//
//        Serial.print("buf 1");
//        Serial.println((char*)buf[1]);
//        Serial.print("capitState");
//        Serial.println(capitState);
      if (!manager.sendtoWait(data, sizeof(data), from)){
        Serial.println("sendtoWait failed");
            berhenti();
            if (!manager.init())Serial.println("init failed");
            else Serial.println("init success");
      }
    }
  }
}


void tutupCapit(){
  for(pos= 0;pos <= 360;pos+=10){
  capit.write(pos);
  Serial.println(pos);
  }
//  Serial.println("Mantap boss capit dah gerak sampai posisi");
//  capit.write(360);
//  capit.write(360);
//  capit.write(360);
//  capit.write(360);
  Serial.write("Debbug : tutup Capit");
}
  

void bukaCapit(){
//  for( pos = 360; pos = 0;pos-=5/*kecepatan membuka capit */ ){
//  capit.write(pos); 
//  }
//    capit.write(0);
    capit.write(0);
//    capit.write(0);
//    capit.write(0);
    Serial.write("Debbug : buka Capit");

}

void mundur(){
  digitalWrite(fwdRight, HIGH);
  digitalWrite(revRight, LOW);
  digitalWrite(fwdLeft, HIGH);
  digitalWrite(revLeft, LOW);
}

void belokKiri(){
  digitalWrite(fwdRight, HIGH);
  digitalWrite(revRight, LOW);
  digitalWrite(fwdLeft, LOW);
  digitalWrite(revLeft, LOW);
}

void belokKanan(){
  digitalWrite(fwdRight, LOW);
  digitalWrite(revRight, LOW);
  digitalWrite(fwdLeft, HIGH);
  digitalWrite(revLeft, LOW);
}

void berhenti(){
  digitalWrite(fwdRight, LOW);
  digitalWrite(revRight, LOW);
  digitalWrite(fwdLeft, LOW);
  digitalWrite(revLeft, LOW);
}

void maju(){
  digitalWrite(fwdRight, LOW);
  digitalWrite(revRight, HIGH);
  digitalWrite(fwdLeft, LOW);
  digitalWrite(revLeft, HIGH);
}
