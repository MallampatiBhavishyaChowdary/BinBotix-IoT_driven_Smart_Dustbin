#include <Servo.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <SoftwareSerial.h>
#include <Servo.h>

int numdata;
int value;
boolean started=false;
char smsbuffer[160];
char n[20];
int PIN = A0;
int sensor = 450;
int LED1 = 11;

Servo myservo; //creates a servo object
int pos = 0; //variable to store servo position

int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 5000;
boolean lockLow = true;
boolean takeLowTime;
int pirPin = 12;
int pirPos = 13;

void setup(){
pinMode(LED1, OUTPUT);
pinMode (PIN,INPUT);
value = analogRead (PIN);
Serial.begin(9600);
Serial.println(value);
Serial.println("ESP8266 Shield testing.");

if (value > sensor){
digitalWrite(LED1, HIGH);
}

WiFi.begin("Your_SSID", "Your_PASSWORD");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

if (value > sensor){
digitalWrite(LED1, HIGH);
Serial.println("Pollution Level Exceeding , Attention Required");
}

ESP8266.begin(9600);
Serial.begin(9600);
pinMode(pirPin, INPUT);
pinMode(pirPos, OUTPUT);
digitalWrite(pirPos, HIGH);

Serial.println("calibrating sensor ");
for(int i = 0; i < calibrationTime; i++){
Serial.print(calibrationTime - i);
Serial.print("-");
delay(1000);
}
Serial.println();
Serial.println("done");

while (digitalRead(pirPin) == HIGH) {
delay(500);
Serial.print(".");
}
Serial.print("SENSOR ACTIVE");
}

void loop(){

if(digitalRead(pirPin) == HIGH){

for(pos = 0; pos < 180; pos += 1)
{
myservo.write(pos);
delay(5);
}

for(pos = 180; pos>=1; pos-=1)
{
myservo.write(pos);
delay(5);
}

if(lockLow){
lockLow = false;
Serial.println("---");
Serial.print("motion detected at ");
Serial.print(millis()/1000);
Serial.println(" sec");
delay(50);
}

takeLowTime = true;
}

if(digitalRead(pirPin) == LOW){
if(takeLowTime){
lowIn = millis();
takeLowTime = false;
}

if(!lockLow && millis() - lowIn > pause){
lockLow = true;
Serial.print("motion ended at ");
Serial.print((millis() - pause)/1000);
Serial.println(" sec");
delay(50);
}
}
}
