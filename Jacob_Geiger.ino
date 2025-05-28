/*
 * Geiger.ino
 * 
 * This code interacts with the Alibaba RadiationD-v1.1 (CAJOE) Geiger counter board 
 * and reports readings in CPM (Counts Per Minute).
 * 
 * Author: Mark A. Heckler (@MkHeck, mark.heckler@gmail.com)
 * 
 * License: MIT License
 * 
 * Please use freely with attribution. Thank you!
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LOG_PERIOD 5000 //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000 //Maximum logging period


// Chinnese OLED 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_SDA 6
#define OLED_SCL 7

#define OLED_ADDRESS 0x3C

// BLINK
#define LED 8  

volatile unsigned long counts = 0;                       // GM Tube events
unsigned long cpm = 0;                                   // CPM
const unsigned int multiplier = MAX_PERIOD / LOG_PERIOD; // Calculates/stores CPM
unsigned long previousMillis;                            // Time measurement
const int pin = 5;

void tube_impulse() { // Captures count of events from Geiger counter board
  counts++;
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void Led_Blink_Test(){
  for(int i=0;i<10;i++){
    pinMode(LED,OUTPUT);
    delay(200);
    pinMode(LED,INPUT);
    delay(200);  
  }
}

void setup() {
  Led_Blink_Test();

  Serial.begin(115200);                                               // Start serial monitor

  // Rad setup
  pinMode(pin, INPUT);                                                // Set pin to input for capturing GM Tube events
  interrupts();                                                       // Enable interrupts (in case they were previously disabled)
  attachInterrupt(digitalPinToInterrupt(pin), tube_impulse, FALLING); // Define external interrupts

  // Oled setup and test
  Wire.begin(OLED_SDA, OLED_SCL);
    // Initialize display
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don’t proceed, loop forever
    }
  Led_Blink_Test();

  display.clearDisplay();
  display.setTextSize(2);             // Text size multiplier
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("1234");
  display.display(); 
}

void loop() {
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis > LOG_PERIOD) {
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    Serial.println(cpm);
    counts = 0;

    // ogarnać przelicznie msv

    display.clearDisplay();
    display.setTextSize(4);
    display.setCursor(0, 0);
    display.println(cpm);
    display.display();
  }
}

