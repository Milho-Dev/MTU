#include "header.h"

/***** DHT stuff ******/

DHTesp dht;

TaskHandle_t taskHandle;

int temperature = 0;
int humidity = 0;

/****** WIFI stuff *****/

const char *ssid = WIFI_SSID;
const char *pswd = WIFI_PSWD;

WiFiUDP udp;

/***** SNMP stuff ******/

SNMPAgent snmp = SNMPAgent("public");

ValueCallback *temperatureOID;
ValueCallback *humidityOID;

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
        1);

    // Connect WiFI
    WiFi.begin(ssid, pswd);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    Serial.print("Connected, Got IP:");
    Serial.println(WiFi.localIP());

    // Setup SNMP pre-requirements
    snmp.setUDP(&udp);
    snmp.begin();

    // Buffer registering for SNMP
    temperatureOID = snmp.addIntegerHandler(TEMPERATURE_OID, &temperature);
    humidityOID = snmp.addIntegerHandler(HUMIDITY_OID, &humidity);

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
    while (true)
    {
        delay(dht.getMinimumSamplingPeriod());
        Serial.println("READING");

        humidity = (dht.getHumidity() * 10);
        temperature = (dht.getTemperature() * 10);

        Serial.printf("Humidity: %i\n", humidity);
        Serial.printf("Temperature: %i\n", temperature);
    }
}