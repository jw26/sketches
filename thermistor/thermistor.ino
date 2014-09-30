
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// which analog pin to connect
#define THERMISTORPIN A0
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5

int samples[NUMSAMPLES];

void setup(void) {
  Serial.begin(9600);
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}

void loop(void) {
  uint8_t i;
  float average;

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
    samples[i] = analogRead(THERMISTORPIN);
    delay(10);
  }

  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
    average += samples[i];
  }
  average /= NUMSAMPLES;

  Serial.print("Average analog reading ");
  Serial.println(average);
  // convert the value to resistance
  average = 1023 / average - 1;
  average = RREF / average;

  Serial.print("Thermistor resistance ");
  Serial.println(average);

  Serial.print("temp : ");
  Serial.print(resistance_to_celcius(double(average)));
  Serial.println(" *C");

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(resistance_to_celcius(double(average)));display.println(" C");
  display.display();

  delay(1000);
}
