#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int potPin = 3;    // 10k Potentiometer connected to GPIO 0
const int ledPin = 20;   
const int buttonPin = 2; 

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize I2C for ESP32-C3 Super Mini
  Wire.begin(8, 9);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED failed"));
    for(;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  // 1. Read Analog Data
  int potValue = analogRead(potPin);            // Raw value (0 to 4095)
  int percentage = map(potValue, 0, 4095, 0, 100); // Convert to 0-100%

  // 2. Read Button/LED status
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // 3. Update OLED Display
  display.clearDisplay();
  
  // Draw a simple header
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("--- SENSOR DATA ---");

  // Show Potentiometer Data
  display.setCursor(0, 20);
  display.setTextSize(2);
  display.print("POT: ");
  display.print(percentage);
  display.println("%");

  display.setTextSize(1);
  display.print("Raw Value: ");
  display.println(potValue);

  // Show LED Status
  display.setCursor(0, 50);
  display.print("LED Status: ");
  display.println(buttonState == LOW ? "ON" : "OFF");

  display.display();
  
  delay(100); // Smooth refresh rate
}