#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_PIN 2

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

void setup()
{
    // Start UART communication
    Serial.begin(9600);
    // Start DS18B20 sensor
    sensors.begin();
    Serial.println("Setup done.");
}

void loop()
{
    sensors.requestTemperatures();
    Serial.print("Temp: ");
    Serial.println(sensors.getTempCByIndex(0));
    delay(1000);
}
