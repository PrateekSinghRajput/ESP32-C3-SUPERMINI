#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C 

// Explicitly define the I2C pins for ESP32-C3 Super Mini
#define I2C_SDA 8
#define I2C_SCL 9

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buttonPin = 2; 
const int ledPin = 20;   

void setup() {
  // Start Serial so you can check the "Serial Monitor" if it fails
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Force I2C pins
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Stop here if screen isn't found
  }

  Serial.println(F("Display Found!"));
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("System Online");
  display.display();
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(2);

  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
    display.println("LED: ON");
    display.setTextSize(1);
    display.println("Button: Pressed");
  } else {
    digitalWrite(ledPin, LOW);
    display.println("LED: OFF");
    display.setTextSize(1);
    display.println("Button: Released");
  }

  display.display();
  delay(100); 
}