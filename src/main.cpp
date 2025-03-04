#include <Arduino.h>
#include <Wire.h>
#include "DHT20.h"

// Task handles
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t Task3Handle = NULL;

// DHT20 Sensor
DHT20 DHT;

void Task1(void *pvParameters) {
    while (1) {
        Serial.println("Hello from Task1");
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay 1000ms
    }
}

void Task2(void *pvParameters) {
    while (1) {
        Serial.println("Hello from Task2");
        vTaskDelay(pdMS_TO_TICKS(1500));  // Delay 1500ms
    }
}

// Task 3: Read DHT20 Temperature & Humidity
void Task3(void *pvParameters) {
    while (1) {
        if (millis() - DHT.lastRead() >= 2000) {
            int status = DHT.read();

            Serial.print("DHT20 Temperature: ");
            Serial.print(DHT.getTemperature(), 1);
            Serial.println(" °C");

            Serial.print("DHT20 Humidity: ");
            Serial.print(DHT.getHumidity(), 1);
            Serial.println(" %");

            Serial.print("Status: ");
            switch (status) {
                case DHT20_OK:
                    Serial.println("OK");
                    break;
                case DHT20_ERROR_CHECKSUM:
                    Serial.println("Checksum error");
                    break;
                case DHT20_ERROR_CONNECT:
                    Serial.println("Connect error");
                    break;
                case DHT20_MISSING_BYTES:
                    Serial.println("Missing bytes");
                    break;
                case DHT20_ERROR_BYTES_ALL_ZERO:
                    Serial.println("All bytes read zero");
                    break;
                case DHT20_ERROR_READ_TIMEOUT:
                    Serial.println("Read time out");
                    break;
                case DHT20_ERROR_LASTREAD:
                    Serial.println("Read too fast");
                    break;
                default:
                    Serial.println("Unknown error");
                    break;
            }
            Serial.println();
        }
        vTaskDelay(pdMS_TO_TICKS(2000));  // Delay 2000ms
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();  // Initialize I2C

    // Initialize DHT20
    if (!DHT.begin()) {
        Serial.println("Failed to initialize DHT20 sensor!");
        while (1);
    }
    Serial.println("DHT20 sensor initialized.");

    // Create Tasks
    xTaskCreate(Task1, "Task1", 1000, NULL, 1, &Task1Handle);
    xTaskCreate(Task2, "Task2", 1000, NULL, 1, &Task2Handle);
    xTaskCreate(Task3, "DHT20Task", 2000, NULL, 1, &Task3Handle);
}

void loop() {
    // Empty - FreeRTOS handles tasks
}
