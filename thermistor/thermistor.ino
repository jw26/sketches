#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "heats.h"
#include "resistance_to_celcius.c"

#define BLUE 4
#define GREEN 7
#define RED 8

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


void setup(void) {
  Serial.begin(9600);
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);

  pinMode(BLUE,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);

  attachInterrupt(0,button_press,FALLING);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("oh hai");
  display.display();
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

void led_off() {
  digitalWrite(BLUE,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(RED,LOW);
}
void led_on(How_hot hot) {
  switch (hot) {
    case COLD:
      digitalWrite(BLUE,HIGH);
      digitalWrite(GREEN,LOW);
      digitalWrite(RED,LOW);
      break;
    case CHILLY:
      digitalWrite(BLUE,HIGH);
      digitalWrite(GREEN,HIGH);
      digitalWrite(RED,LOW);
      break;
    case HOT:
      digitalWrite(BLUE,LOW);
      digitalWrite(GREEN,LOW);
      digitalWrite(RED,HIGH);
      break;
    case OK:
      digitalWrite(BLUE,LOW);
      digitalWrite(GREEN,HIGH);
      digitalWrite(RED,LOW);
      break;
    case WARM:
      digitalWrite(BLUE,LOW);
      digitalWrite(GREEN,HIGH);
      digitalWrite(RED,HIGH);
      break;
    default:
      digitalWrite(BLUE,LOW);
      digitalWrite(GREEN,LOW);
      digitalWrite(RED,LOW);
  }
}


volatile uint8_t press = 0;
unsigned int state = 0;
unsigned long ticks = 0;

unsigned long sensor_ticks = 0;
#define TEN_SECONDS 10000
#define THIRTY_SECONDS 30000

double temps[3] = {NULL,NULL,NULL};

void button_press() {
  press += 1;
}

How_hot check_temp(double temp) {
  How_hot heat = OK;
  if (temp < 19.00) {
    heat = CHILLY;
    if (temp < 18.00) {
      heat = COLD;
    }
  } else if (temp > 21.00) {
    heat = WARM;
    if (temp > 24.00) {
      heat = HOT;
    }
  }
  return heat;
}

void loop(void) {
  unsigned long m = millis();

  if (temps[0] == NULL || m-sensor_ticks > long(TEN_SECONDS)) {
    temps[0] = resistance_to_celcius(read_therm(A0,5));
    temps[1] = resistance_to_celcius(read_therm(A1,5));
    temps[2] = resistance_to_celcius(read_therm(A2,5));

    How_hot hotness = check_temp(temps[0]);
  }

  if (state > 0 && m-ticks > long(TEN_SECONDS)) {
    state = 0; ticks = 0;
    led_off();
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }

  if( press > 0 ) {
    ticks = m;
    state = (state+1)%4;
    if (state != 0) {
      double t = temps[state-1];
      How_hot hotness = check_temp(t);
      led_on(hotness);
      display.ssd1306_command(SSD1306_DISPLAYON);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print(t);display.print("* C");
      display.setCursor(0,20);
      display.display();
    } else {
      display.ssd1306_command(SSD1306_DISPLAYOFF);
      led_off();
      ticks = 0;
    }
    press = 0;
  } else {
    //report_ok();
  }

}
