#if defined(ESP8266)
#include <ESP8266WiFi.h> // ESP8266 Core WiFi Library
#else
#include <WiFi.h> // ESP32 Core WiFi Library
#endif

#include <Arduino.h>
#include <SNMP_Agent.h>
#include <DHTesp.h>

#define WIFI_SSID "MilhoRasp"
#define WIFI_PSWD "MilhoNerfado"

#define STATIC_IP 10, 42, 0, 200
#define GATEWAY 10, 42, 0, 1
#define SUBNET 255, 255, 255, 0
#define PRIMARYDNS 8, 8, 8, 8
#define SECONDARYDNS 8, 8, 4, 4


#define SNMP_COMMUNITY "server_ROOM"

#define TEMPERATURE_OID ".1.3.6.1.4.1.1234.1.2"
#define HUMIDITY_OID ".1.3.6.1.4.1.1234.1.1"
#define DHT22_STATE ".1.3.6.1.4.1.1234.1.0"
#define TIMESTAMP_OID ".1.3.6.1.2.1.1.3.0"

#define LED_GPIO_PIN GPIO_NUM_22

#define DHT_GPIO_PIN GPIO_NUM_15