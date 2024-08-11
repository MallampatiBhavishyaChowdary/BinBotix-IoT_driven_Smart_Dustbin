#include "SIM900.h"
#include <SoftwareSerial.h>
#include <Servo.h>

// SMS and GSM objects
SMSGSM sms;
boolean started = false;
char smsbuffer[160];
char n[20];

// Pin definitions
int PIN = A0;
int sensorThreshold = 450;
int LED1 = 11;
int pirPin = 12;
int pirPos = 13;

// Servo object
Servo myservo;
int pos = 0;  // Variable to store the servo position

// Timing variables for the PIR sensor
int calibrationTime = 30;  // Calibration time for the PIR sensor
long unsigned int lowIn;
long unsigned int pause = 5000;
boolean lockLow = true;
boolean takeLowTime;

void setup() {
  // Initialize pins
  pinMode(LED1, OUTPUT);
  pinMode(PIN, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(pirPos, OUTPUT);
  digitalWrite(pirPos, HIGH);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("GSM Shield testing.");

  // Read analog sensor value
  int value = analogRead(PIN);
  Serial.println(value);

  // Check if the sensor value exceeds the threshold
  if (value > sensorThreshold) {
    digitalWrite(LED1, HIGH);
  }

  // Start GSM module
  if (gsm.begin(2400)) {
    Serial.println("\nstatus=READY");
    started = true;
  } else {
    Serial.println("\nstatus=IDLE");
  }

  // Send SMS if the sensor value exceeds the threshold
  if (value > sensorThreshold) {
    digitalWrite(LED1, HIGH);
    sms.SendSMS("+917530001174", "RJ27 CB 3**9 Pollution Level Exceeding, Attention Required");
    Serial.println("\nSMS sent OK");
  }

  // PIR sensor calibration
  Serial.println("Calibrating sensor...");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(calibrationTime - i);
    Serial.print("-");
    delay(1000);
  }
  Serial.println();
  Serial.println("Done");

  // Wait for PIR sensor to settle
  while (digitalRead(pirPin) == HIGH) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("SENSOR ACTIVE");
}

void loop() {
  // Check if GSM module started
  if (started) {
    if (gsm.readSMS(smsbuffer, 160, n, 20)) {
      Serial.println(n);
      Serial.println(smsbuffer);
    }
  }

  // Check for motion detected by PIR sensor
  if (digitalRead(pirPin) == HIGH) {
    // Move servo from 0 to 180 degrees and back
    for (pos = 0; pos < 180; pos += 1) {
      myservo.write(pos);
      delay(5);
    }
    for (pos = 180; pos >= 1; pos -= 1) {
      myservo.write(pos);
      delay(5);
    }

    // Lock low and print motion detected message
    if (lockLow) {
      lockLow = false;
      Serial.println("---");
      Serial.print("Motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      delay(50);
    }
    takeLowTime = true;
  }

  // Check for motion ended
  if (digitalRead(pirPin) == LOW) {
    if (takeLowTime) {
      lowIn = millis();
      takeLowTime = false;
    }

    if (!lockLow && millis() - lowIn > pause) {
      lockLow = true;
      Serial.print("Motion ended at ");
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      delay(50);
    }
  }
}
