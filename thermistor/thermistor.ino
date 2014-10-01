#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BLUE 3
#define GREEN 5
#define RED 6

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// borrowed from
// https://github.com/fornellas/sketchbook/blob/master/libraries/Thermistor/Thermistor.cpp
// Constants for Vishay part NTCLE100E3103JB0:
// http://www.sparkfun.com/products/250
// If you have a different part, please refer to the datasheet:
// http://www.sparkfun.com/datasheets/Sensors/Thermistor23816403-1.pdf
// and adjust them as needed.
#define RREF 10000.0
#define TA1 0.003354016
#define TB1 0.0002569850
#define TC1 0.000002620131
#define TD1 0.00000006383091

double resistance_to_celcius(double res) {
  double RT;
  double temperature;
  double logRtRref;
  double logRtRref2;

  logRtRref=log(res/RREF);
  logRtRref2=logRtRref * logRtRref;
  temperature=1.0 /
    (
      TA1 +
      TB1 * logRtRref +
      TC1 * logRtRref2 +
      TD1 * logRtRref2 * logRtRref
    ) - 273.15;

  return temperature;
}

void setup(void) {
  Serial.begin(9600);
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);

  pinMode(BLUE,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}

double read_therm(uint8_t pin, int numsamples) {
  uint8_t i;
  double average;
  int samples[numsamples];

  analogRead(pin);
  delay(100);

  // take N samples in a row, with a slight delay
  for (i=0; i< numsamples; i++) {
    samples[i] = analogRead(pin);
    delay(10);
  }

  // average all the samples out
  average = 0;
  for (i=0; i< numsamples; i++) {
    average += samples[i];
  }
  average /= numsamples;
  average = 1023 / average - 1;
  average = RREF / average;

  return average;
}

void report_hot() {
  digitalWrite(BLUE,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(RED,HIGH);
}
void report_warm() {
  digitalWrite(BLUE,LOW);
  digitalWrite(GREEN,HIGH);
  digitalWrite(RED,HIGH);
}
void report_ok() {
  digitalWrite(BLUE,LOW);
  digitalWrite(GREEN,HIGH);
  digitalWrite(RED,LOW);
}
void report_chilly() {
  digitalWrite(BLUE,HIGH);
  digitalWrite(GREEN,HIGH);
  digitalWrite(RED,LOW);
}
void report_cold() {
  digitalWrite(BLUE,HIGH);
  digitalWrite(GREEN,LOW);
  digitalWrite(RED,LOW);
}


void loop(void) {

  report_cold();delay(1000);report_chilly();delay(1000);report_ok();delay(1000);report_warm();delay(1000);report_hot();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(resistance_to_celcius(read_therm(A0,5)));display.println(" C");
  display.print(resistance_to_celcius(read_therm(A1,5)));display.println(" C");
  display.print(resistance_to_celcius(read_therm(A2,5)));display.println(" C");
  display.display();

  delay(1000);
}
