/*******************ARDUINO-BASED GAS LEAKAGE DEVICE*****************************
 
Note:    I've only used this project as a research project, focusing on creating a gas leak detector and temperature monitor using Arduino. 
If you're considering using this code, please reach me out and ask for permission as I can provide the research paper for this study.
I am  open to any discussions and questions regarding this matter. 

Important note: Careful calibration, especially for the MQ2 gas sensor, is really important for accurate operation in industrial settings. More detailed research is needed in this area.
                                          
************************************************************************************/
// Libraries used here
#include <Wire.h> 
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

// Set your calibrated sensor value here
#define Threshold 400  
#define MQ2pin 0

float sensorValue; 

const int buzzerPin = 4;    // Define which analog input channel you are going to use
const int tempHumidPin = 2;
const int sim900aRxPin = 8;
const int sim900aTxPin = 9;

const float LPG_THRESHOLD = 400.0;
const float SMOKE_THRESHOLD = 200.0;
const float GAS_THRESHOLD = 200.0;

SoftwareSerial sim800l(7, 8);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(tempHumidPin, DHT22);


void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(MQ2pin, INPUT);
  pinMode(tempHumidPin, INPUT);
  sim800l.begin(9600);
  lcd.begin();
  lcd.backlight();
  dht.begin();

  lcd.setCursor(1, 0);
  lcd.print("Flammable Gas Detector");
  delay(5000);
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("Initializing...");
  lcd.scrollDisplayLeft();
  delay(5000);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("MQ2 warming up!");
  delay(10000); // Required MQ Sensor Warm up
  lcd.clear();
}

void loop() {
  sensorValue = analogRead(MQ2pin);
  
  int tempHumidValue = digitalRead(tempHumidPin);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.println("");
     delay(2000);

  if (sensorValue > Threshold) {
    SendMessage(sensorValue, temperature, humidity);
    digitalWrite(buzzerPin, HIGH);
    lcd.setCursor(0 , 0);
    lcd.print("GAS DETECTED!");
    delay(1000);
    lcd.setCursor(0,1);
    lcd.print("SMS Sent");
    delay(5000);
    
  } else {
    digitalWrite(buzzerPin, LOW);
  }
lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print(" Hum: ");
  lcd.print(humidity);
  lcd.print("%");
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(2, 0);
  lcd.print("SensVal: ");
  lcd.print(sensorValue);
  delay(2000);
  lcd.clear(); 
  
  lcd.setCursor(2, 1);
  lcd.print("Gas Level Normal");
  delay(3000);
  lcd.clear();
}
void SendMessage(float sensorValue, float temperature, float humidity) {
  sim800l.println("I am in send");
  sim800l.println("AT+CMGF=1");
  delay(1000);
  sim800l.println("AT+CMGS=\"YOUR NUMBER HERE\"\r"); // Input the device's phone number here
  delay(2000);
  sim800l.println("Warning: Gas Leakage Detected!");
  sim800l.print("Gas Value: ");
  sim800l.println(sensorValue);
  sim800l.print("temperature: ");
  sim800l.print(temperature);
  sim800l.print("C\n");
  sim800l.print("humidity: ");
  sim800l.print(humidity);
  sim800l.println("%");
  sim800l.println((char)26);
delay(1000);
}
