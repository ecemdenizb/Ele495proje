
 #include <LiquidCrystal_I2C.h>
#include <Wire.h>

//adr, en, rw,rs, d4, d5, d6, d7, b1, b1pol

LiquidCrystal_I2C lcd(0x27, 16, 2);
//I2C adres bilgisi

#include <SoftwareSerial.h>
#define tx 2
#define rx 3
SoftwareSerial configBt(rx, tx); // RX, TX
void setup() 
{
  Serial.begin(9600);
  configBt.begin(9600);
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);

        lcd.init();
  lcd.backlight();
  lcd.print("X ekseni = ");

   lcd.setCursor(0,1);
 lcd.print("Y ekseni =");
 lcd.setCursor(12,1);
}
int i;
String Kordinatlar[2];
void loop() 
{
  if(configBt.available()) //if the bluetooth module is sending something...
  {
    Kordinatlar[i] = configBt.readString();
    i++;
    
  }
   

  if(i == 2)
  { 
    i = 0;

  ChangeStr(Kordinatlar[0]);
  ChangeStr(Kordinatlar[1]);
  
  lcd.setCursor(11,0);

  lcd.print(Kordinatlar[0]);





  lcd.setCursor(11,1);



 lcd.print(Kordinatlar[1]);
 

  }
}

void ChangeStr(String str)
{
  if(str.length() < 4)
  {
    for(int i = str.length();i < 4;i++)
    {
      str[i] = ' ';
    }
  }
  else if(str[1] == ' ')
    for(int i =1;i < 4;i++)
    {
      str[i] = ' ';
    }
  else if(str[2] == ' ')
    for(int i =2;i < 4;i++)
    {
      str[i] = ' ';
    }
  else if(str[3] == ' ')
    for(int i =3;i < 4;i++)
    {
      str[i] = ' ';
    }
}
 
