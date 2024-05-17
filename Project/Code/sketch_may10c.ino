#define BLYNK_TEMPLATE_ID "TMPL3GCDace8O"
#define BLYNK_TEMPLATE_NAME "gas"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define ANALOG_PIN 34 
#define SERVO_PIN 18
#define BUZZER_PIN 12 
#define FAN_PIN 21
#define LPG_THRESHOLD 20 
#define BLYNK_TOKEN "8Vt_thWCzMfF1PxIxs0D1f5sAepocpj3" // Replace "YourAuthToken" with your Blynk authentication token
#define WIFI_SSID "Rupesh12" // Replace "YourSSID" with your Wi-Fi SSID
#define WIFI_PASSWORD "123456789" // Replace "YourPassword" with your Wi-Fi password

BlynkTimer timer;

void sendLpgEvent() {
  Blynk.virtualWrite(V0, "LPG Gas detected!");
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_TOKEN, WIFI_SSID, WIFI_PASSWORD);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  timer.setInterval(1000L, []() {
    int value = analogRead(ANALOG_PIN);
    value = map(value, 0, 4095, 0, 100);
    Blynk.virtualWrite(V1, value);
    
    if (value > LPG_THRESHOLD) {
      Serial.println(" LPG Gas detected!");
      Serial.print(value);
      moveServo(0);
      activateBuzzer();
      activateFan();
      sendLpgEvent();
      delay(1000);
    } else {
      moveServo(180);
      deactivateBuzzer();
      deactivateFan();
      Serial.println(" No LPG Gas detected!");
      Serial.print(value);
      delay(500);
    }
  });
}

void loop() {
  Blynk.run();
  timer.run();
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

