#include <Arduino.h>

void setup() {
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
  
  // Set pin 2 as an output
  pinMode(2, OUTPUT);
}

void loop() {
  // Print "Hello, World!" to the serial monitor
  Serial.println("Hello, World!");
  
  // Blink pin 2
  digitalWrite(2, HIGH); // Turn the LED on
  delay(500);             // Wait for half a second
  digitalWrite(2, LOW);  // Turn the LED off
  delay(500);             // Wait for half a second
}
