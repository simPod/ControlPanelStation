#include <LowPower.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS3231 rtc;

void setup()
{
    Serial.begin(9600);
    rtc.adjust(DateTime(2021, 2, 10, 17, 49, 10));
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.display();
    delay(15);
}

void goodNight()
{
    delay(100);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void displayData(uint8_t minut, uint8_t uhr)
{
    display.setTextSize(2);
    display.clearDisplay();
    display.setCursor(35, 0);
    display.print(uhr);
    display.print(":");
    display.print(minut);
    display.display();
}


void loop()
{
    slip:
    DateTime now = rtc.now();
    if (now.hour() > 20  || now.hour() < 5) {
        DateTime now = rtc.now();
        display.clearDisplay();
        display.setCursor(0, 0);
        display.display();
        goodNight();
        goodNight();
        goodNight();
        delay(100);
        goto slip;
    }

    else {
        display.setTextSize(2);
        DateTime now = rtc.now();

        displayData(now.minute(), now.hour());
    }
}