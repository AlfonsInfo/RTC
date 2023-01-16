//koneksi joystick
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//Servo untuk capit
#include <Servo.h>
//LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t stickman [] = {0xe,0xe,0xe,0x15,0xe,0x4,0xa,0x11};  

#define revLeft 42
#define fwdLeft 44
#define revRight 4
#define fwdRight 5

RF24 radio(7, 8); // CE, CSN

Servo capit; // object Servo
Servo sendiCapit; // sendi naik turun capit
int pos = 0; // servo position
int count=0, count1=0;
const byte address[6] = "78789";

void setup() {
  Serial.begin(9600);

  pinMode(revLeft, OUTPUT); 
  pinMode(fwdLeft, OUTPUT);
  pinMode(revRight, OUTPUT);
  pinMode(fwdRight, OUTPUT);
  
  lcd.begin();
  lcd.createChar (0,stickman);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  capit.attach(3);
  sendiCapit.attach(2);
  maju();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("--ROBOTIC UAJY--");
  lcd.setCursor(0,1);
  lcd.print("ATMA JAYA YOGYA");
  lcd.write(byte(0));
  bukaCapit();
//  tutupCapit();
//  maju();
if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    switch (text[0]){
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

    switch (text[1]){
      case '1':
          count1=0;
          if(count==0){
            bukaCapit();
            count++;
            break;
          }else{
            break;
          }
        
      case '0':
        count=0;
        if(count1==0){
          tutupCapit();
          count1++;
          break;          
        }else{
          break;
        }
        
      default :
        break;
    }
    
//    sendiCapit.write();
  }
}

void bukaCapit(){
  for(pos= 0;pos <= 360;pos+=5/*kecepatan mencapit/tentuin sudutnya ntar coba" dlu */){
  capit.write(pos);
  }
  Serial.write("Debbug : buka Capit");
}


void tutupCapit(){
  for( pos = 360; pos = 0;pos-=5/*kecepatan membuka capit */ ){
  capit.write(pos); 
  }
  Serial.write("Debbug : tutup Capit");
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
