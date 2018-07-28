#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// screen - SDA - A4, SCL - A5
// TEMP - middle to A2

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);
const int trigPin = 9;
const int echoPin = 10;

float temp;
int tempPin = 2; // analog input pin 

const int buttonPin = 2;
const int tempButtonPin = 4;
int buttonState = 0;
int tempBtnState = 0;
char distanceBuf[10];
char tempBuf[10];
long duration, cm, inches;
String getDistanceInCm();
bool flag = false;
bool tempFlag = false;

void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Hello");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication 
  pinMode(buttonPin, INPUT); 
  pinMode(tempButtonPin, INPUT);
}
 
 
void loop()
{
  buttonState = digitalRead(buttonPin);
  tempBtnState = digitalRead(tempButtonPin);

  
  // Check if the temperature button pressed - if it does the temperature will appear on the LCD: 
  if (tempBtnState == HIGH && tempFlag == false) {
    Serial.println("Got one");
    tempFlag=true;
    }
    if(tempBtnState == LOW && tempFlag==true){
      Serial.println("got in");
      tempFlag = false;
      String str = getTemperature();
      Serial.println(str);
      printOnLcd(str);
      }


  
  // Check if the distance button pressed - if it does the distance will appear on the LCD:
  if (buttonState == HIGH && flag == false) {
    Serial.println("Got one");
    flag=true;
    }
    if(buttonState == LOW && flag==true){
      Serial.println("got in");
      flag = false;
      String str = getDistanceInCm();
      Serial.println(str);
      printOnLcd(str);
      }
      delay(500);
}

void printOnLcd(String str){
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println(str +"got");
  lcd.print(str);  
}

String getTemperature(){
  temp = analogRead(tempPin);
  Serial.print("RAW DATA: ");
  Serial.print (temp);
  Serial.println(" ");
  //converts raw data into degrees celsius and prints it out
  // 500mV/1024=.48828125
  temp = temp * 0.48828125;
  Serial.print("CELSIUS: ");
  Serial.print(temp);
  Serial.println("*C ");
  dtostrf(temp, 4, 2, tempBuf); 
  String myStr = "Temp: ";
  myStr+=tempBuf;
  myStr+="*C";
  Serial.println(myStr);
  return String(myStr);
  
}

String getDistanceInCm()
{
    digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 cm = duration*0.034/2-147;
  sprintf(distanceBuf,"cm: %u",cm); 
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  return (distanceBuf);
}
