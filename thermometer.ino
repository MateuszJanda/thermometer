#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>
#include <Wire.h>

#define ONE_WIRE_PIN 2
#define DEVICE_INDEX 0

OneWire one_wire(ONE_WIRE_PIN);
DallasTemperature temp_sensor(&one_wire);

// OLED display width, in pixels
#define OLED_SCREEN_WIDTH 128
// OLED display height, in pixels
#define OLED_SCREEN_HEIGHT 64

// Declaration for SSD1306 display connected using software 4-wire SPI (default case):
#define OLED_MOSI_PIN 11
#define OLED_SCLK_PIN 13
#define OLED_CS_PIN 10
#define OLED_DC_PIN 7
#define OLED_RST_PIN 8

Adafruit_SSD1306 display(
    OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, OLED_MOSI_PIN, OLED_SCLK_PIN, OLED_DC_PIN, OLED_RST_PIN, OLED_CS_PIN);

void setup()
{
    // Start UART communication
    Serial.begin(9600);

    // Start DS18B20 sensor
    temp_sensor.begin();
    Serial.println("Temperature sensor setup done.");

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println(F("SSD1306 allocation failed"));
        // Don't proceed after that
        for (;;) { }
    }

    // Rotate display by 180
    display.setRotation(2);
    // Clear the buffer
    display.clearDisplay();
    // Set white font color
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void loop()
{
    // Read temperature from sensor
    temp_sensor.requestTemperatures();
    float temperature = temp_sensor.getTempCByIndex(DEVICE_INDEX);
    Serial.println("Temp: " + String(temperature));

    // Display temperature value in Celsius
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.println(String(temperature) + "C");

    // Print unique 64bit temperature sensor id (on 1-Wire)
    DeviceAddress device_address = {};
    bool result = temp_sensor.getAddress(device_address, DEVICE_INDEX);
    if (result) {
        Serial.println("Device address OK");
        display.setTextSize(1);
        display.setCursor(0, 48);

        char strBuf[50] = {};
        for (size_t i = 0; i < 8; i++) {
            Serial.println(device_address[i], HEX);
            sprintf(&strBuf[i * 2], "%02X", device_address[i]);
        }
        display.print("Dev" + String(DEVICE_INDEX) + ":");
        display.println(strBuf);
    }

    display.display();
    delay(500);
}
