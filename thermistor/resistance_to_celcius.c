#include <Arduino.h>

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

