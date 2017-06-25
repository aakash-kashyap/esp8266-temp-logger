#include <stdlib.h>
#include<SoftwareSerial.h>
#include <LiquidCrystal.h>
SoftwareSerial mySerial(8,9);
/*-----------------ESP8266 Serial WiFi Module---------------*/
#define SSID "Xperia"     // "SSID-WiFiname"
#define PASS "12345678"       // "password" 
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=P16XGEAUQ9OZWBIJ"; //change it with your key...
/*-----------------------------------------------------------*/

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

float temp;
String tempC;
int error;
void setup()
{
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Temp. Logger");
  Serial.begin(115200);
  mySerial.begin(115200); //or use default 115200.

  // set the cursor to column 0, line 1
 
  lcd.setCursor(0, 1);
  delay(1000);
  lcd.clear();
  lcd.print("Connecting");

  mySerial.println("AT");
  
  delay(5000);
  if(mySerial.find("OK"))
  {
    connectWiFi();
  }
 
}

void loop()
{
  start: //label 
  error=0;
  temp = analogRead(A0)*0.4882;
  char buffer[10];
  // there is a useful c function called dtostrf() which will convert a float to a char array 
  //so it can then be printed easily.  The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  tempC = dtostrf(temp, 4, 1, buffer); 

  updateTemp();
  if (error==1)
  {
    goto start; //go to label "start"
  }
 // delay(100000); //Update every 1 hour
 delay(2000);
}

void updateTemp()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  mySerial.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if(mySerial.find("Error"))
  {
    return;
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature
  cmd += tempC;
  cmd += "\r\n";
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(mySerial.find(">"))
  {
    Serial.println("Sending");
    mySerial.print(cmd);
    Serial.print(cmd);
    lcd.setCursor(0, 1);
    delay(500);
    lcd.clear();
    lcd.print(temp); 
  }
  else
  {
    mySerial.println("AT+CIPCLOSE");
    Serial.println("AT+CIPCLOSE");
    //Resend...
    error=1;
  }
}

 
boolean connectWiFi()
{
  mySerial.println("AT+CWMODE=1");
  Serial.println("AT+CWMODE=1");
  delay(5000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  mySerial.println(cmd);
  Serial.println(cmd);
  delay(5000);
  if(mySerial.find("OK"))
  {
   
    Serial.println("connected");
    lcd.setCursor(0, 1);
    delay(500);
    lcd.clear();
    lcd.print("Connected"); 
    return true;
  }
  else
  { Serial.println("Not connected");
    lcd.setCursor(0, 1);
    delay(500);
    lcd.clear();
    lcd.print("Not Connected"); 
    return false;   
  }
}
