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

// OLED display resolution in pixels
#define OLED_SCREEN_WIDTH 128
#define OLED_SCREEN_HEIGHT 64

// Declaration for SSD1306 display connected using software 4-wire SPI (default case):
#define OLED_MOSI_PIN 11
#define OLED_SCLK_PIN 13
#define OLED_CS_PIN 10
#define OLED_DC_PIN 7
#define OLED_RST_PIN 8

Adafruit_SSD1306 display(
    OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, OLED_MOSI_PIN, OLED_SCLK_PIN, OLED_DC_PIN, OLED_RST_PIN, OLED_CS_PIN);

// Unique (64-bit) temperature sensor ID (on 1-Wire)
DeviceAddress device_address = {};
char device_address_str[20] = {};

void setup()
{
    // Start UART baud (communication speed)
    Serial.begin(9600);

    // Start DS18B20 sensor
    temp_sensor.begin();
    Serial.println("Temperature sensor setup done.");

    // Read unique (64-bit) temperature sensor ID (on 1-Wire)
    bool result = temp_sensor.getAddress(device_address, DEVICE_INDEX);
    if (result) {
        device_address_to_string();
        Serial.print("Temperature sensor ID: ");
        Serial.println(device_address_str);
    } else {
        Serial.println("Can't read temperature sensor ID on 1-Wire.");
    }

    // Start SSD1306 display.
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println(F("SSD1306 allocation failed."));
        // Don't proceed after that
        for (;;) { }
    }

    // Rotate display by 180
    display.setRotation(2);
    // Clear display screen
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
    Serial.println("Temp: " + String(temperature) + "C");

    // Display temperature value in Celsius
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.println(String(temperature) + "C");

    // Print unique (64-bit) temperature sensor ID (on 1-Wire)
    display.setTextSize(1);
    display.setCursor(0, 48);
    display.print("Dev" + String(DEVICE_INDEX) + ":");
    display.println(device_address_str);

    display.display();
    delay(500);
}

void device_address_to_string()
{
    for (size_t i = 0; i < sizeof(device_address); i++) {
        sprintf(&device_address_str[i * 2], "%02X", device_address[i]);
    }
}