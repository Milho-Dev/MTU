#if defined(ESP8266)
#include <ESP8266WiFi.h> // ESP8266 Core WiFi Library
#else
#include <WiFi.h> // ESP32 Core WiFi Library
#endif

#include <Arduino.h>
#include <SNMP_Agent.h>
#include <DHTesp.h>

#define WIFI_SSID "SSID"
#define WIFI_PSWD "PASSWORD"

#define TEMPERATURE_OID ".1.3.6.1.4.1.1234.1.0"
#define HUMIDITY_OID ".1.3.6.1.4.1.1234.1.1"
#define TIMESTAMP_OID ".1.3.6.1.2.1.1.3.0"

#define LED_GPIO_PIN GPIO_NUM_22

#define DHT_GPIO_PIN GPIO_NUM_15