#include <SPI.h> //koneksi joystick
#include <PWMServo.h> //Library Servo biasa , conflit radio Head
#include <LiquidCrystal_I2C.h>
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t stickman [] = {0xe,0xe,0xe,0x15,0xe,0x4,0xa,0x11};  

//Konstanta Untuk Gerakan Roda / Motor
#define revLeft 42
#define fwdLeft 44
#define revRight 4
#define fwdRight 5
#define ENAPin 6
#define ENBPin 3
#define speedMotorRight 80
#define speedMotorLeft 80


//Servo
PWMServo capit; 
PWMServo arm;
char capitState = '0';
int armPosition = '0';
int pos = 0; // servo position

//Komunikasi
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

  //SET UP LCD
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
        case '0' : 
          if(capitState == (char*)buf[1]){break;}
          else{
            tutupCapit();
            capitState='0'  ;
          }
          break;
      }
      
      String servoPosArm(((char*)buf)[2]);
      servoPosArm.concat(((char*)buf)[3]);
      servoPosArm.concat(((char*)buf)[4]);
      if(servoPosArm.toInt() !=  armPosition){
          arm.write((servoPosArm.toInt()/2));
          armPosition = servoPosArm.toInt();
      };
      Serial.println((servoPosArm));
      
      if (!manager.sendtoWait(data, sizeof(data), from)){
        Serial.println("sendtoWait failed");
            //Jika Sinyal Ilang, berhenti
            berhenti();
            if (!manager.init())Serial.println("init failed");
            else Serial.println("init success");
      }
    }
  }
}


void tutupCapit(){
  capit.write(180);
  Serial.write("Debbug : tutup Capit");
}
  

void bukaCapit(){
    capit.write(100);
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
  Serial.write("stop");
}

void maju(){
  digitalWrite(fwdRight, LOW);
  digitalWrite(revRight, HIGH);
  digitalWrite(fwdLeft, LOW);
  digitalWrite(revLeft, HIGH);
}
