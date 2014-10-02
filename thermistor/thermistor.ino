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

sensor sensors[3];

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

  sensors[0].name = "one ";
  sensors[1].name = "two ";
  sensors[2].name = "thre";
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

void display_sensor (sensor s) {
  led_on(s.h);
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(s.name);display.print(". ");
  display.print(s.t);display.print("* C");
  display.setCursor(0,20);
  display.display();
}

void display_off() {
  led_off();
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

byte sensor_set_alarm (How_hot h) {
  return h == HOT;
}

void loop (void) {
  unsigned long m = millis();

  if (m-sensor_ticks > long(TEN_SECONDS)) {
    sensors[0].t = resistance_to_celcius(read_therm(A0,5));
    sensors[1].t = resistance_to_celcius(read_therm(A1,5));
    sensors[2].t = resistance_to_celcius(read_therm(A2,5));

    How_hot hotness;
    for (int i=0; i<3; i++) {
      sensors[i].h = check_temp(sensors[i].t);
      byte nval = sensor_set_alarm(sensors[i].h);
      if (nval != sensors[i].alarm) {
        // reset alarm
        display_off();
        sensors[i].alarm = nval;
      }
    }

    sensor_ticks = m;
  }

  if (state > 0 && m-ticks > long(TEN_SECONDS)) {
    state = 0; ticks = 0;
    display_off();
  }

  if( press > 0 ) {
    ticks = m;
    state = (state+1)%4;
    if (state != 0) {
      sensor s = sensors[state-1];
      display_sensor(s);
    } else {
      ticks = 0;
      display_off();
    }
    press = 0;
  } else {
    // alarm states
    for (int i=0; i<3; i++) {
      if (sensors[i].alarm > 0) {
        display_sensor(sensors[i]);
      }
    }
  }

}
