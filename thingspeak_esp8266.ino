#include <SoftwareSerial.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define DHTPIN 22
#define DHTTYPE DHT11
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DHT dht(DHTPIN, DHTTYPE);
#define ag_ismi "Melo"
#define ag_sifresi "12345678"
const int analogPin = A0;
const int digitalPin = 22;
int gaz = 0;

String apiKey = "ECC6SP0AVG7W2FM3";     // replace with your channel's thingspeak WRITE API key

boolean thingSpeakWrite(float value1, float value2, int value3){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  Serial.println(cmd);
  if(Serial.find("Error")){
    Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  getStr +="&field3=";
  getStr += String(value3);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);
  Serial.println(getStr);
  
  if(Serial.find(">")){
    Serial.print(getStr);
    Serial.print("VERİ GÖNDERİLDİ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
  else{
    Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}

void setup() {   
  lcd.begin(16, 2);             
  Serial.begin(115200); 
  Serial.println("AT");
  delay(5000);
    if(Serial.find("OK")){
       Serial.println("AT+CWMODE=1");
       delay(2000);
        //Serial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");
        //String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\"";
        String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
        Serial.println(baglantiKomutu);
        delay(3000);
        Serial.println("AT+CIPMUX=1");
        delay(5000);
        Serial.println("Setup completed");
    }
        
}


void loop() {  
   float t = dht.readTemperature();
   float h = dht.readHumidity();
   gaz = analogRead(analogPin);
   Serial.println("SICAKLIK= "+String(t)+" *C");
   Serial.println("NEM= "+String(h)+" %");
   Serial.println("GAZ= "+String(gaz)+" PPM");
   lcd.setCursor(0, 0);
   lcd.print("Sicaklik:");
   lcd.print(t + String("*C"));
   lcd.setCursor(0, 1);
   lcd.print("Nem : ");
   lcd.print(h + String("%"));
   delay(5000);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Gaz : ");
   lcd.print(gaz + String("PPM"));
   thingSpeakWrite(t, h, gaz);                                    
   delay(7000);  
}




