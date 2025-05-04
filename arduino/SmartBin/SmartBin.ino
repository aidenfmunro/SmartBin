#include <Servo.h>

const int trigPin = 3;
const int echoPin = 2;
const int ledPin = 13;
const int servoPin = 5;

// Настройки
const int distanceMin = 5;    // Минимальная дистанция (см)
const int distanceMax = 40;   // Максимальная дистанция (см)
const unsigned long closeDelay = 5000; // Задержка перед закрытием (мс)

Servo myServo;
bool objectDetected = false;
unsigned long lastDetectionTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);     // Attach once at the start
  myServo.write(151);           // Стартовая позиция — "закрыто"
  Serial.begin(9600);
  Serial.println("Setup complete.");
}

void loop() {
  long duration;
  float distance;

  // Генерация ультразвукового импульса
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Измерение длительности отражённого сигнала
  duration = pulseIn(echoPin, HIGH);

  // Перевод времени в расстояние (см)
  distance = duration * 0.034 / 2;

  // DEBUG output:
  Serial.print("Measured distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Проверка расстояния
  if (distance > distanceMin && distance < distanceMax) {
    Serial.println("Object detected within range.");

    digitalWrite(ledPin, HIGH);
    myServo.write(75); // Открыть
  
    objectDetected = true;
  } else {
    Serial.println("No object detected in range.");

    if (objectDetected) {
      lastDetectionTime = millis();
      objectDetected = false;
      Serial.println("Object just lost. Starting closeDelay timer.");
    }

    if (millis() - lastDetectionTime >= closeDelay) {
      Serial.println("closeDelay passed. Closing servo.");
      digitalWrite(ledPin, LOW);
      myServo.write(151); // Закрыть
      delay(300);         // Дать серво время доехать в закрытое положение
      Serial.println("Servo closed.");
    }
  }

  delay(50); // Небольшая задержка перед следующим измерением
}


