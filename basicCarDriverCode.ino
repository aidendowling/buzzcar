#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Direction LEDs
const int LED_LEFT  = 12;
const int LED_RIGHT   = 13;

// PWM
const int pwmPin = 4;

// Motor 1 (Left motor)
const int motor1Forward  = 5;
const int motor1Backward = 6;

// Motor 2 (Right motor)
const int motor2Forward  = 20;
const int motor2Backward = 7;

// Photoresistor
const int PHOTO_PIN = 2;

// ✅ Power Button
const int BUTTON_PIN = 19;
bool systemOn = false;
bool lastButtonState = HIGH;

void showMsg(const char* msg, int sensorValue) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(msg);

  display.setTextSize(1);
  display.print("Light: ");
  display.println(sensorValue);

  display.display();
}

void stopMotors() {
  digitalWrite(motor1Forward, LOW);
  digitalWrite(motor1Backward, LOW);
  digitalWrite(motor2Forward, LOW);
  digitalWrite(motor2Backward, LOW);
}

void moveForward() {
  digitalWrite(motor1Forward, HIGH);
  digitalWrite(motor1Backward, LOW);
  digitalWrite(motor2Forward, HIGH);
  digitalWrite(motor2Backward, LOW);
}

void turnLeft() {
  digitalWrite(motor1Forward, LOW);
  digitalWrite(motor1Backward, LOW);
  digitalWrite(motor2Backward, LOW);
  digitalWrite(motor2Forward, HIGH);
}

void turnRight() {
  digitalWrite(motor1Forward, HIGH);
  digitalWrite(motor1Backward, LOW);
  digitalWrite(motor2Backward, LOW);
  digitalWrite(motor2Forward, LOW);
}

void setup() {
  Serial.begin(115200);

  Wire.begin(22, 21);

  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);

  pinMode(pwmPin, OUTPUT);

  pinMode(motor1Forward, OUTPUT);
  pinMode(motor1Backward, OUTPUT);
  pinMode(motor2Forward, OUTPUT);
  pinMode(motor2Backward, OUTPUT);

  pinMode(PHOTO_PIN, INPUT);

  // ✅ Button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) { delay(10); }
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  // ✅ Button toggle logic
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    systemOn = !systemOn;   // toggle state
    delay(200);             // simple debounce
  }

  lastButtonState = currentButtonState;

  // ✅ If system is OFF
  if (!systemOn) {
    stopMotors();
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_RIGHT, LOW);
    showMsg("OFF", 0);
    return;
  }

  // --- Your original code below remains untouched ---

  int sensorValue = analogRead(PHOTO_PIN);
  Serial.println(sensorValue);

  stopMotors();

  digitalWrite(LED_LEFT, LOW);
  digitalWrite(LED_RIGHT, LOW);

  if (sensorValue < 1300) {
    showMsg("LEFT", sensorValue);
    digitalWrite(LED_LEFT, HIGH);
    turnLeft();
  }
  else if (sensorValue < 2600) {
    showMsg("FORWARD", sensorValue);
    moveForward();
  }
  else {
    showMsg("RIGHT", sensorValue);
    digitalWrite(LED_RIGHT, HIGH);
    turnRight();
  }

  delay(200);
}
