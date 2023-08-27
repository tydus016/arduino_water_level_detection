/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// set variables for stop lights
int GREEN = 13;  // green
int RED = 12;    // red
int buzzerPin = 11;

// Sensor pins
#define sensorPower 10
#define sensorPin A3

// Value for storing water level
int val = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("test");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;  // Don't proceed, loop forever
  } else {
    Serial.println("123");
  }

  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(sensorPower, OUTPUT);

  // Set to LOW so no power flows through the pins
  digitalWrite(sensorPower, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);

  tone(buzzerPin, 1000, 1000);

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Set text size, color, and position
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // digitalWrite(GREEN, HIGH);
  // digitalWrite(RED, HIGH);

  //get the reading from the function below and print it
  int level = readSensor();

  Serial.print("Water level: ");
  Serial.println(level);

  String text = F("Water level: ");
  int textWidth = text.length() * 6;
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = (SCREEN_HEIGHT - 2 * 8) / 2;

  text += level;

  display.setCursor(x, y);
  display.println(text);
  display.display();

  indicator_(level);

  delay(1000);
  display.clearDisplay();
}

void indicator_(int level) {
  if (level < 149) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
  } else if (level >= 150 && level <= 170) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    tone(buzzerPin, 1000, 1000);
    delay(1000);
  } else {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    tone(buzzerPin, 1000, 1000);
    delay(1000);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
  }
  noTone(buzzerPin);
}

//This is a function used to get the reading
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);                        // wait 10 milliseconds
  val = analogRead(sensorPin);      // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;                       // send current reading
}
