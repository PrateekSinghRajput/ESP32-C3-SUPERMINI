#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 Configuration
#define DHTPIN 10       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // Specify the sensor type
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT sensor
  dht.begin();

  // Initialize I2C for ESP32-C3 (SDA=8, SCL=9)
  Wire.begin(8, 9);

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("OLED fail"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Sensor Error!");
    display.println("Check Wiring...");
    display.display();
    return;
  }

  // --- Display UI ---
  display.clearDisplay();
  
  // Draw Title
  display.setTextSize(1);
  display.setCursor(20, 0);
  display.println("ENVIRONMENT");
  display.drawLine(0, 12, 128, 12, SSD1306_WHITE);

  // Display Temperature
  display.setCursor(0, 22);
  display.setTextSize(1);
  display.print("TEMP:");
  display.setCursor(45, 20);
  display.setTextSize(2);
  display.print(t, 1); // 1 decimal place
  display.setTextSize(1);
  display.print(" C");

  // Display Humidity
  display.setCursor(0, 47);
  display.setTextSize(1);
  display.print("HUMI:");
  display.setCursor(45, 45);
  display.setTextSize(2);
  display.print(h, 1); // 1 decimal place
  display.setTextSize(1);
  display.print(" %");

  display.display();
  
  delay(2000); // DHT11 is slow, only read every 2 seconds
}