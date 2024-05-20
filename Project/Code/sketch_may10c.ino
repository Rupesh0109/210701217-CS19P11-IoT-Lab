#define BLYNK_TEMPLATE_ID "TMPL3GCDace8O"
#define BLYNK_TEMPLATE_NAME "gas"
#include <Arduino.h>
#include <BlynkSimpleEsp32.h> // Include Blynk library

#define ANALOG_PIN 34
#define SERVO_PIN 4
#define BUZZER_PIN 12
#define FAN_PIN 21
#define LPG_THRESHOLD 20

// Blynk app credentials (replace with your own)
char auth[] = "8Vt_thWCzMfF1PxIxs0D1f5sAepocpj3";
char ssid[] = "Rupesh12";
char pass[] = "123456789";

unsigned long previousMillis = 0;
const long interval = 2000; // Interval to check LPG gas (milliseconds)

BLYNK_WRITE(V1) // Define virtual pin for receiving commands
{
  int angle = param[0].asInt(); // Read angle value from app
  moveServo(angle);
}

void setup() {
  Serial.begin(9600);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  Blynk.begin(auth, ssid, pass); // Initialize Blynk connection
}

void loop() {
  Blynk.run(); // Run Blynk library

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int value = analogRead(ANALOG_PIN);
    value = map(value, 0, 4095, 0, 100);

    if (value > LPG_THRESHOLD) {
      Serial.println("LPG Gas detected!");
      Serial.print("Value: ");
      Serial.println(value);
      Blynk.virtualWrite(V2, value); 
      moveServo(0);
      activateBuzzer();
      activateFan();
      delay(1000);
    } else {
      Serial.println("No LPG Gas detected!");
      Serial.print("Value: ");
      Serial.println(value);
      Blynk.virtualWrite(V2, value); // Send LPG value to app
      moveServo(180);
      deactivateBuzzer();
      deactivateFan();
      delay(500);
    }
  }
}

void moveServo(int angle) {
  int pulseWidth = map(angle, 0, 180, 1000, 2000);
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(SERVO_PIN, LOW);
  delay(20);
}

void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
}

void deactivateBuzzer() {
  digitalWrite(BUZZER_PIN, LOW);
}

void activateFan() {
  digitalWrite(FAN_PIN, HIGH);
}

void deactivateFan() {
  digitalWrite(FAN_PIN, LOW);
}