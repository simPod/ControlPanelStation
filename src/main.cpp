#include <LowPower.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include "RTClib.h"
#include <IRremote.h>
#include "Sleep.h"
#include "Display.h"
#include "Time.h"


RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class DisplayClass

{
public:
    DisplayClass()
    = default;

    explicit DisplayClass(String initial)
    {
        init();
        display.setTextColor(SSD1306_WHITE);
        size(2);
        cursor(0,0);
        clear();
        update();
    }

    void init()
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;);
        }
            display.print("hello ma frent");
            delay(1000);
        }

    void displayData(uint8_t uhr, uint8_t minut)
    {
        clear();
        cursor(30,0);
        display.print(uhr);
        display.print(":");
        display.print(minut);
        update();
        delay(1000);
    }

    void size(uint8_t textSize)
    {
        display.clearDisplay();
        display.setTextSize(textSize);
    }

    void clear()
    {
        display.clearDisplay();
    }

    void update()
    {
        display.display();
    }

    void cursor(uint8_t x, uint8_t y)
    {
        display.setCursor(x,y);
    }
};

class RtcClass
{
    public:
    void setup() {
        if (!rtc.begin()) {
            Serial.println("Couldn't find RTC");
            Serial.flush();
            abort();
        }
        if (rtc.lostPower()) {
            Serial.println("RTC lost power, let's set the time!");
            //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            //rtc.adjust(DateTime(2021, 2, 25, 12, 30, 0));
        }
    }

    void customAdjust(uint8_t hour, uint8_t min)
    {
        rtc.adjust(DateTime(2021, 2, 25, hour, min, 40));
    }

    DateTime getTime(){
        return rtc.now();
    }

    bool isReadingTime(){
        DateTime now = getTime();

        return now.hour() == 20 && now.minute() == 40;
    }

    bool isSleepTime(){
        DateTime now = getTime();

        return now.hour() == 21 && now.minute() == 28;
    }
};

void setup()
{
    DisplayClass disp("init");
    RtcClass clock;
    clock.setup();
    Serial.begin(9600);
    delay(15);
    clock.customAdjust(20,39);
}

void loop()
{
    DisplayClass disp;
    RtcClass clock;
    DateTime now = clock.getTime();
    disp.displayData(now.hour(), now.minute());

    if (clock.isReadingTime()){

    }

    else if (clock.isSleepTime()){

    }



    /*
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

    }*/
}