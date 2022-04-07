#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 20(SDA), 21(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // scan for address

const char* ssid = "SSID";
const char* password = "wifi_pwd";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long lastIntervallTemp = 0;

void PrintTime()
{
  struct tm local_time;
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.clearDisplay();

  if (!getLocalTime(&local_time)) {
    display.println("Can't connect to ntp server");
    display.display();
    return;
  }
  display.println(&local_time, "%A, %B %d %Y %H:%M:%S");
  display.display();
}

void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  //connect to WiFi
  display.printf("Connecting to %s ", ssid);
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }
  display.println(" CONNECTED!");
  display.display();

  display.setCursor(0, 30);
  display.println("Get time from ntp server ...");
  display.display();
  delay(1000);
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  display.println(" done!");
  display.display();
  delay(1000);
  PrintTime();
}

void loop()
{
  if (millis() - lastIntervallTemp > 1000)
  {
    lastIntervallTemp = millis();
    PrintTime();
  }

}
