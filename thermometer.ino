#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ONE_WIRE_PIN 2
#define DEVICE_INDEX 0

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature temp_sensor(&oneWire);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  11
#define OLED_CLK   13
#define OLED_CS    10
#define OLED_DC     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup()
{
    // Start UART communication
    Serial.begin(9600);

    // Start DS18B20 sensor
    temp_sensor.begin();
    Serial.println("Temperature sensor setup done.");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    // Don't proceed after thath
    for(;;); 
  }

  display.setRotation(90);

    // Clear the buffer
  display.clearDisplay();

    // display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  // display.setCursor(0,0);             // Start at top-left corner
  // display.println(F("Hello, world!"));
  display.display();
}

void loop()
{
    // Read temperature from sensor
    temp_sensor.requestTemperatures();
    float temperature = temp_sensor.getTempCByIndex(DEVICE_INDEX);
    Serial.println("Temp: " + String(temperature));

    // Print temperature on display
    display.clearDisplay();

    
        display.setTextSize(3);             // Normal 1:1 pixel scale
  // display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  // display.println(F(String(temperature).c_str()));
  // display.println("aaa");
  // display.println(F("aaa"));
  // temperature *= -10;
  display.println(String(temperature) + "C"  );
  
    
  // Print temperature sensor id
  DeviceAddress device_address = {};
    bool result = temp_sensor.getAddress(device_address, DEVICE_INDEX);
    if (result)
    {
      Serial.println("Device address OK");
      display.setTextSize(1);  
      display.setCursor(0,48);
// display.println(String(address));

  Serial.println();
char strBuf[50] = {};
    for (size_t i = 0; i < 8; i++)
    {
      Serial.println(device_address[i], HEX);
            sprintf(&strBuf[i * 2], "%02X", device_address[i]);
      // sprintf(strBuf + (i * 2), "%02X", 9);

      

    }
    Serial.println();
    display.print("Dev" + String(DEVICE_INDEX) + ":");
    display.println(strBuf);
    // display.println("Here");

  }

display.display();
    delay(500);
}
