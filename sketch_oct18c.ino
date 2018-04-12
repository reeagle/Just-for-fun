#include <Keypad.h>

const int redPin = 2;  //define the RGB LED ports
const int greenPin = 3;
const int bluePin = 4;
const int digitPins[4] = {25,24,23,22}; //define the display segment ports
const int clockPin = 8;
const int latchPin = 9;
const int dataPin = 10;
const int solenoidPin = 40;
const int password = 1234;
const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {28,29,30,31};
byte colPins[COLS] = {32,33,34,35};
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const byte digit[11] = {     
  B00111111, //0
  B00000110, //1
  B01011011, //2
  B01001111, //3
  B01100110, //4
  B01101101, //5
  B01111101, //6
  B00000111, //7
  B01111111, //8
  B01101111,  //9
  B00000000  //void
};
int ReadIndex = 0;
int DisplayIndex = 0;
int digitBuffer[4] = {};
unsigned long previousMillis = 0;  
const long interval = 3000;

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup()
{
  Serial.begin(9600);
  for(int i=0;i<4;i++)
    pinMode(digitPins[i],OUTPUT); 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(clockPin, OUTPUT); 
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(solenoidPin, OUTPUT); 
  digitalWrite(solenoidPin, HIGH);
}

void loop()
{
   char customKey = customKeypad.getKey();
   if (customKey) 
    {
      switch(customKey)
        {
        case '0':  
          digitBuffer[ReadIndex] = 0;
          break;
      
        case '1':  
          digitBuffer[ReadIndex] = 1;
          break;
      
        case '2':  
          digitBuffer[ReadIndex] = 2; 
          break;
      
        case '3':  
          digitBuffer[ReadIndex] = 3; 
          break;
      
        case '4':  
          digitBuffer[ReadIndex] = 4;
          break;
      
        case '5':  
          digitBuffer[ReadIndex] = 5;
          break;
      
        case '6':  
          digitBuffer[ReadIndex] = 6;
          break;
      
        case '7':  
          digitBuffer[ReadIndex] = 7;
          break;
      
        case '8':  
          digitBuffer[ReadIndex] = 8;
          break;
      
        case '9':  
          digitBuffer[ReadIndex] = 9;
          break;

        case '*':
          digitalWrite(solenoidPin, HIGH);
          break;

        case '#':
          clearPassword();
          break;

        default:
          break;
        }
      Serial.println(digitBuffer[ReadIndex]);
      if(customKey!='#')  
        ReadIndex++;  
      Serial.println(customKey);
    }
      
  updateDisp();
  delay(2);
  unsigned long currentMillis = millis();
    
  if(ReadIndex == 4)
  {
    int tempPassword = 1000*digitBuffer[0]+100*digitBuffer[1]+10*digitBuffer[2]+digitBuffer[3];
    if (tempPassword == password)
     {
      setColor(0,255,0);
      digitalWrite(solenoidPin, LOW);
     }
   else
      setColor(255,0,0);
   ReadIndex = 0;
  }
    
if(currentMillis - previousMillis >= interval)
   {
    previousMillis = currentMillis;
    setColor(0,0,0);
   }
}

void updateDisp()
{
  for(byte j=0; j<4; j++)  
    digitalWrite(digitPins[j], HIGH);
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin,clockPin,MSBFIRST,B00000000);
  digitalWrite(latchPin,HIGH);
  
  delayMicroseconds(100);
  digitalWrite(digitPins[DisplayIndex], LOW); 

  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, digit[digitBuffer[DisplayIndex]]);
  digitalWrite(latchPin, HIGH);

  DisplayIndex++;
  if(DisplayIndex>3)
    DisplayIndex = 0;     
}

void clearPassword()
{
  for(int i=0;i<4;i++)
    digitBuffer[i] = 0;
  ReadIndex = 0;
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}



