#include <WiFi.h>
#include <ESP32Servo.h>

// ------------------ Pin Definitions ------------------
#define LDR_PIN       34
#define RAIN_PIN      36
#define SOUND_PIN     39   // LM393 Siren Sound Sensor
#define TRIG_PIN      18
#define ECHO_PIN      19
#define SERVO_PIN     14   // SERVO ON PIN 14
#define LED_PIN       25
#define BUZZER_PIN    26

// ------------------ Variables ------------------
long duration;
int distance;
int ldrValue;
int rainValue;
int soundValue;

bool isRain = false;
bool isSiren = false;
bool vehicleDetected = false;

// Servo
Servo myServo;
int servoAngle = 15;
int servoDir = 1;
int lockedServoAngle = -1;

// Speed detection
float Speed = 0;
int flag1 = 0, flag2 = 0;
unsigned long t1 = 0, t2 = 0;

const float SPEED_THRESHOLD = 40.0;

const int DIST1 = 30;
const int DIST2 = 20;

// Thresholds
const int NIGHT_LDR_THRESHOLD = 1500;
const int RAIN_THRESHOLD = 1800;
const int SIREN_THRESHOLD = 2200;   // SOUND sensor threshold

// -------------------------------------------------
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 200;

  return duration * 0.034 / 2;
}

// -------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Smart Street Light System — SERVO FIXED ON PIN 14");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Servo setup
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(servoAngle);
}

// -------------------------------------------------
void loop() {

  // 1. LDR
  ldrValue = analogRead(LDR_PIN);
  bool isNight = (ldrValue > NIGHT_LDR_THRESHOLD);

  // 2. Rain
  rainValue = analogRead(RAIN_PIN);
  isRain = (rainValue < RAIN_THRESHOLD);

  // 3. Sound Sensor (Siren detection)
  soundValue = analogRead(SOUND_PIN);
  isSiren = (soundValue > SIREN_THRESHOLD);

  // 4. Ultrasonic Distance
  distance = getDistance();

  if (distance < 100 && distance > 5) {
    vehicleDetected = true;

    if (distance < DIST1 + 5) {
      if (distance <= DIST1 && flag1 == 0) {
        t1 = millis();
        flag1 = 1;
      }
      if (distance <= DIST2 && flag2 == 0) {
        t2 = millis();
        flag2 = 1;
      }
      if (flag1 && flag2) {
        float Time = fabs((float)t2 - (float)t1) / 1000.0;
        if (Time > 0.005) {
          Speed = (DIST1 - DIST2) / Time * 0.036;
        }
        flag1 = flag2 = 0;
      }
    }
  } else {
    vehicleDetected = false;
    Speed = 0;
    flag1 = 0;
    flag2 = 0;
  }

  // 5. LED Control
  if (isNight) {
    if (isRain || isSiren || vehicleDetected)
      analogWrite(LED_PIN, 1023);
    else
      analogWrite(LED_PIN, 700);
  } else {
    analogWrite(LED_PIN, 0);
  }

  // 6. Buzzer Control
  if (Speed > SPEED_THRESHOLD || isRain || isSiren)
    tone(BUZZER_PIN, 1500);
  else
    noTone(BUZZER_PIN);

  // 7. Servo Movement
  if (vehicleDetected) {
    if (lockedServoAngle == -1)
      lockedServoAngle = servoAngle;

    myServo.write(lockedServoAngle);
  } else {
    lockedServoAngle = -1;
    servoAngle += servoDir * 5;

    if (servoAngle >= 165 || servoAngle <= 15)
      servoDir *= -1;

    myServo.write(servoAngle);
  }

  // ---------------- SERIAL PRINT ----------------
  Serial.print("Dist: "); Serial.print(distance);
  Serial.print(" cm | Vehicle: "); Serial.print(vehicleDetected ? "YES" : "NO");
  Serial.print(" | Speed: "); Serial.print(Speed, 2); Serial.print(" Km/Hr");

  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" ("); Serial.print(isNight ? "NIGHT" : "DAY"); Serial.print(")");

  Serial.print(" | Rain: "); Serial.print(isRain ? "YES" : "NO");

  Serial.print(" | Siren: "); Serial.print(isSiren ? "YES" : "NO");
  Serial.print(" | SoundVal: "); Serial.print(soundValue);

  Serial.print(" | LED: "); Serial.println(isNight ? "ON" : "OFF");

  // Overspeed alert message
  if (Speed > SPEED_THRESHOLD) {
    Serial.println(">>> 🚨 OVERSPEED ALERT! Speed Limit Exceeded! 🚨 <<<");
  }

  delay(150);
}
