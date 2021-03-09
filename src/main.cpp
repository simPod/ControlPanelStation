#include <Arduino.h>
#include <LowPower.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include "RTClib.h"
#include <IRremote.h>
#include <Servo.h>
#include "Sleep.h"
#include "Display.h"
#include "Time.h"

RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo servo;

//defined IR out at pin 3

class PinClass
{
public:
    const byte relayPin = 5;
    const byte servoPin = 6;

    explicit PinClass(String init)
    {
        pinMode(relayPin, OUTPUT);
        digitalWrite(relayPin, HIGH);
        servo.attach(servoPin);
    }

    PinClass()
    = default;

    void high(uint8_t pinNum)
    {
        digitalWrite(pinNum, HIGH);
    }

    void low(uint8_t pinNum)
    {
        digitalWrite(pinNum, LOW);
    }
};

class DisplayClass

{
public:
    DisplayClass()
    = default;

    explicit DisplayClass(String init)
    {
        setup();
        display.setTextColor(SSD1306_WHITE);
        size(2);
        cursor(0,0);
        clear();
        update();
    }

    void setup()
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;);
        }

    }

    void adjustHoursMinutes(uint8_t uhr, uint8_t minut)
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
    explicit RtcClass(String init) {
        setup();
    }

    RtcClass()
    = default;

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

    void customAdjust(uint8_t hour, uint8_t min) {
        rtc.adjust(DateTime(2021, 3, 8, hour, min, 10));
    }

    DateTime getTime() {
        return rtc.now();
    }

    bool isReadingTime() {
        DateTime now = getTime();
        return now.hour() == 20 && now.minute() == 40;
    }

    bool isSleepTime() {
        DateTime now = getTime();
        return now.hour() == 21 && now.minute() == 28;
    }

    bool isNightMode() {
        DateTime now = getTime();
        return (now.hour() >= 21 && now.minute() >= 29) || now.hour() < 5;
    }

    bool isDaylight() {
        DateTime now = getTime();
        return now.hour() == 6 && now.minute() == 20;
    }

    bool isNightlight()
    {
        DateTime now = getTime();
        return now.hour() == 19 && now.minute() == 30;
    }
};

class IrClass
{
private:
    uint8_t address = 0x0;
    uint8_t numOfRepeats = 0;
public:
    explicit IrClass(String init)
    {
        setup();
    }
    IrClass()
    = default;

    uint8_t red = 0x19;
    uint8_t dim = 0x1D;
    uint8_t lightUp = 0x9;
    uint8_t on = 0xD;
    uint8_t off = 0x1F;

    void setup() //maly pismenko
    {
        IrSender.begin(true);
    }

    void send(uint8_t action) //maly pismenko
    {
        IrSender.sendNEC(address, action, numOfRepeats);
        delay(200);
    }
};

void setup()
{
    PinClass pin("init");
    IrClass ir("init");
    DisplayClass disp("init");
    RtcClass clock("init");
    Serial.begin(9600);
    delay(15);
    clock.customAdjust(18,18);
    pin.low(pin.relayPin);
}

void loop()
{
    IrClass ir;
    DisplayClass disp;
    RtcClass clock;
    PinClass pin;
    DateTime now = clock.getTime();
    disp.adjustHoursMinutes(now.hour(), now.minute());

    if (clock.isReadingTime()){
        ir.send(ir.on);
        ir.send(ir.red);
    }

    else if (clock.isSleepTime()){
        ir.send(ir.off);
    }

    else if (clock.isDaylight())
    {
        pin.low(pin.relayPin);//use register control instead
    }

    else if (clock.isNightlight())
    {
        pin.high(pin.relayPin); //register control instead
        //code for servo moving turning on lampLight
    }

    while (clock.isNightMode()) {
        goodNight();
    }
}