

#include <SPI.h>
#include <mcp2515.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 10
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(128, 64, &Wire, -1);
MCP2515 mcp2515(7);
struct can_frame canMsgTx, canMsgRx;

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  SPI.begin(5, 4, 6, 7);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  // --- SENDING SECTION ---
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000) {
    int t = dht.readTemperature();
    int h = dht.readHumidity();
    
    canMsgTx.can_id = 0x123;
    canMsgTx.can_dlc = 2;
    canMsgTx.data[0] = t;
    canMsgTx.data[1] = h;
    mcp2515.sendMessage(&canMsgTx);
    
    // UI Update for Sending
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    display.printf("T:%dC H:%d%%", t, h);
    
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print("TX PKT: ID 0x123");
    display.setCursor(0, 55);
    display.print("Status: SENT OK");
    display.display();
    
    lastSend = millis();
  }

  // --- RECEIVING SECTION ---
  if (mcp2515.readMessage(&canMsgRx) == MCP2515::ERROR_OK) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("BTN: ");
    display.println(canMsgRx.data[0] == 1 ? "PRESSED" : "OPEN");
    
    // Packet Info
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.printf("RX PKT ID: 0x%X", canMsgRx.can_id);
    display.setCursor(0, 50);
    display.printf("DLC: %d  DATA: %d", canMsgRx.can_dlc, canMsgRx.data[0]);
    display.display();
  }
}






/*

#include <SPI.h>
#include <mcp2515.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 4
Adafruit_SSD1306 display(128, 64, &Wire, -1);
MCP2515 mcp2515(5);
struct can_frame canMsgTx, canMsgRx;

bool lastBtn = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {
  // --- SENDING SECTION (Button) ---
  bool currentBtn = digitalRead(BUTTON_PIN);
  if (currentBtn != lastBtn) {
    canMsgTx.can_id = 0x124;
    canMsgTx.can_dlc = 1;
    canMsgTx.data[0] = (currentBtn == LOW) ? 1 : 0;
    mcp2515.sendMessage(&canMsgTx);
    lastBtn = currentBtn;
  }

  // --- RECEIVING SECTION (DHT Data) ---
  if (mcp2515.readMessage(&canMsgRx) == MCP2515::ERROR_OK) {
    int t = canMsgRx.data[0];
    int h = canMsgRx.data[1];

    display.clearDisplay();
    display.setTextColor(WHITE);
    
    // Main Data
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.printf("TEMP: %d C", t);
    display.setCursor(0, 20);
    display.printf("HUMI: %d %%", h);
    
    // Packet Info at bottom
    display.setTextSize(1);
    display.drawFastHLine(0, 40, 128, WHITE); // Divider line
    display.setCursor(0, 45);
    display.printf("RX ID: 0x%X  DLC: %d", canMsgRx.can_id, canMsgRx.can_dlc);
    display.setCursor(0, 55);
    display.printf("RAW: %02X %02X", canMsgRx.data[0], canMsgRx.data[1]);
    
    display.display();
    
    // Serial Monitor Output
    Serial.printf("Packet RX ID: 0x%X | DLC: %d | Data: %d, %d\n", canMsgRx.can_id, canMsgRx.can_dlc, t, h);
  }
}

*/