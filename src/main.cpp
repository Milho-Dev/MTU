#include "header.h"

/***** DHT stuff ******/

DHTesp dht;

TaskHandle_t taskHandle;

int temperature = 0;
int humidity = 0;

int dht22_state = DHTesp::ERROR_NONE;

/****** WIFI stuff *****/

const char *ssid = WIFI_SSID;
const char *pswd = WIFI_PSWD;

WiFiUDP udp;

// Set your Static IP address
IPAddress local_IP(STATIC_IP);
// Set your Gateway IP address
IPAddress gateway(GATEWAY);

IPAddress subnet(SUBNET);
IPAddress primaryDNS(PRIMARYDNS);     // optional
IPAddress secondaryDNS(SECONDARYDNS); // optional

/***** SNMP stuff ******/

SNMPAgent snmp = SNMPAgent(SNMP_COMMUNITY);

ValueCallback *temperatureOID;
ValueCallback *humidityOID;

ValueCallback *dht22_stateOID;

TimestampCallback *timestampCallbackOID;
uint32_t tensOfMillisCounter = 0;

void readSensor(void *pvParameters);

void setup()
{
    Serial.begin(9600);
    Serial.println("Start!");

    // Setup DHT22
    dht.setup(DHT_GPIO_PIN, DHTesp::DHT22);

    xTaskCreatePinnedToCore(
        readSensor,        /* Function to implement the task */
        "readSensor task", /* Name of the task */
        2000,              /* Stack size in words */
        NULL,              /* Task input parameter */
        tskIDLE_PRIORITY,  /* Priority of the task */
        &taskHandle,       /* Task handle. */
        0);

    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }

    WiFi.begin(ssid, pswd);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Setup SNMP pre-requirements
    snmp.setUDP(&udp);
    snmp.begin();

    // Buffer registering for SNMP
    temperatureOID = snmp.addIntegerHandler(TEMPERATURE_OID, &temperature);
    humidityOID = snmp.addIntegerHandler(HUMIDITY_OID, &humidity);

    dht22_stateOID = snmp.addIntegerHandler(DHT22_STATE, &dht22_state);

    timestampCallbackOID = (TimestampCallback *)snmp.addTimestampHandler(TIMESTAMP_OID, &tensOfMillisCounter);

    snmp.sortHandlers();
}

void loop()
{
    snmp.loop();

    // Timestamp counting
    tensOfMillisCounter = millis() / 10;
}

// Read DHT22
void readSensor(void *pvParameters)
{
    DHTesp::DHT_ERROR_t err;
    while (true)
    {
        delay(dht.getMinimumSamplingPeriod());
        Serial.println("READING");

        err = dht.getStatus();

        humidity = err == DHTesp::ERROR_NONE ? (dht.getHumidity() * 10) : 0;
        temperature = err == DHTesp::ERROR_NONE ? (dht.getTemperature() * 10) : 0;
        dht22_state = err;

        Serial.printf("Sensor STATE: %s\n", dht22_state == DHTesp::ERROR_NONE ? "OK" : "FAIL");
        Serial.printf("Humidity: %i\n", humidity);
        Serial.printf("Temperature: %i\n", temperature);
    }
}