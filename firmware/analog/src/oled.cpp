#include <Arduino.h>
#include <Wire.h>
//#13
#include <Adafruit_GFX.h>
//#135
#include <Adafruit_SSD1306.h>

#include <globals.h>
#include <oled.h>

Adafruit_SSD1306 ssd1306(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

OLED::OLED()
    : m_fontSize(1)
{
}

OLED::~OLED()
{
}

void OLED::begin()
{
    ssd1306.begin(SSD1306_SWITCHCAPVCC, I2CADDR_OLED);
    ssd1306.clearDisplay();
    ssd1306.setTextSize(2);
    ssd1306.setTextColor(WHITE);
    ssd1306.setCursor(0,0);
    ssd1306.cp437(true);
    ssd1306.println("MsyKBD 0.1");
    ssd1306.println("AnalogKeys");
    ssd1306.display();
}

void OLED::end()
{
}

void OLED::display()
{
    ssd1306.display();
    delay(33);
}

void OLED::clear()
{
    ssd1306.clearDisplay();
}

void OLED::setSize(int size)
{
    ssd1306.setTextSize(size);
}

void OLED::locate(int x, int y)
{
    ssd1306.setCursor(x, y);
}

void OLED::print(char *data)
{
    ssd1306.println(data);
    ssd1306.display();
}

void OLED::printLayer(int layer)
{
    ssd1306.setTextSize(2);
    // ssd1306.setTextColor(WHITE);
    ssd1306.setCursor(0,0);
    // ssd1306.cp437(true);
    switch (layer) {
        case LAYER_DEFAULT:
            ssd1306.println("[ PSO2 ]");
            break;
        case LAYER_RAISE:
            ssd1306.println("[ RAISE ]");
            break;
        case LAYER_LOWER:
            ssd1306.println("[ LOWER ]");
            break;
        case LAYER_MEDIA:
            ssd1306.println("[ Fusion ]");
            break;
        default:
            ssd1306.println("[ Unkown ]");
            break;
    }
    ssd1306.display();
}
