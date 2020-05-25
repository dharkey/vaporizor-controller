/* 
 Vaporizor Controller Application
 By Dave Harkey
 -- based on HTU21D Humidity Sensor Example Code by Nathan Seidle (public domain) --

 Some software-related notes:
 + Uses the HTU21D library to display the current humidity and temperature
 + Open serial monitor at 9600 baud to see readings. Errors 998 if not sensor is detected. Error 999 if CRC is bad.
 + Hardware Connections (Breakoutboard to Arduino):
    -VCC (sensor) = 3.3V
    -GND = GND
    -SDA = A4 (use inline 330 ohm resistor if your board is 5V -- didn't do)
    -SCL = A5 (use inline 330 ohm resistor if your board is 5V -- didn't do)
    -IN4 (relay) = DIO8
    -VCC (relay) = 5V
 */

#include <Wire.h>
#include "SparkFunHTU21D.h"

const int RELAY_PIN = 8;
HTU21D myHumidity;
int g_print_mesage;

const uint8_t STATUS_ON = 1;
const uint8_t STATUS_OFF = 0;
const int TURN_OFF_RELAY = HIGH;
const int TURN_ON_RELAY = LOW;
const int CHECK_DELAY = 1000; // 1 second

uint8_t g_vaporizor_status = STATUS_ON;

const uint8_t LOWER_THRESHOLD = 45; // in % humidity
const uint8_t UPPER_THRESHOLD = 55; // in % humidity

void print_status(float temperature, float humidity_lvl){
    Serial.print("Time:");
    Serial.print(millis());
    Serial.print(" Humidity:");
    Serial.print(humidity_lvl, 1);
    Serial.print("%");
    Serial.print(" Temperature:");
    Serial.print(temperature, 1);
    Serial.print("C");
    Serial.print(" Vaporizor status: ");
    Serial.print(g_vaporizor_status);//0 for off, 1 for on
    Serial.println();
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");
  
    g_print_mesage = 1;
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, TURN_ON_RELAY);
    myHumidity.begin();
}

void loop()
{
    float humidity_lvl = myHumidity.readHumidity();
    
    float temperature = myHumidity.readTemperature();
  
    print_status(temperature, humidity_lvl);
    
    if(humidity_lvl < LOWER_THRESHOLD){
        digitalWrite(RELAY_PIN, TURN_ON_RELAY);
        g_vaporizor_status = STATUS_ON;
        if(g_print_mesage){
            Serial.print("Humidity below ");
            Serial.print(LOWER_THRESHOLD);
            Serial.print(": Vaporizor turned on.");
            Serial.println();
            g_print_mesage = STATUS_OFF;
        }
    }
    else if(humidity_lvl > UPPER_THRESHOLD){
        digitalWrite(RELAY_PIN, TURN_OFF_RELAY);
        if(g_print_mesage){
            g_vaporizor_status = STATUS_OFF;
            Serial.print("Humidity above ");
            Serial.print(UPPER_THRESHOLD);
            Serial.print(": Vaporizor turned off.");
            Serial.println();
            g_print_mesage = STATUS_OFF;
        }
    } else
        g_print_mesage = STATUS_ON;
    
    delay(CHECK_DELAY);
}
