#include <Servo.h>

const int trigPin = 3;
const int echoPin = 2;
const int ledPin = 13;
const int servoPin = 5;

// Настройки
const int distanceMin = 5;    // Минимальная дистанция (см)
const int distanceMax = 20;   // Максимальная дистанция (см)
const unsigned long closeDelay = 5000; // Задержка перед закрытием (мс)

Servo myServo;
bool objectDetected = false;
unsigned long lastDetectionTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  myServo.attach(servoPin);
  myServo.write(153); // Стартовая позиция — "закрыто"
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

  // Проверка расстояния
  if (distance > distanceMin && distance < distanceMax) {
    // Объект обнаружен
    if (!myServo.attached()) {
    myServo.attach(servoPin);
    }

    digitalWrite(ledPin, HIGH);
    myServo.write(5); // Открыть
    objectDetected = true;
  } else {
    // Объект не обнаружен
    if (objectDetected) {
      // Только что потеряли объект — запоминаем время
      lastDetectionTime = millis();
      objectDetected = false;
    }

    // Проверяем, прошло ли 3 секунды с момента пропадания объекта
    if (millis() - lastDetectionTime >= closeDelay) {
      digitalWrite(ledPin, LOW);
      myServo.write(153); // Закрыть
      delay(300);         // Дать серво время доехать в закрытое положение
      myServo.detach();   // Отключить серво после закрытия
    }
  }

  delay(50); // Небольшая задержка перед следующим измерением
}
