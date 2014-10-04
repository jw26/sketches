#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


static const unsigned char PROGMEM img_bmp[] = {
};

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

void display_init () {
  pinMode(BLUE,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("oh hai");
  display.display();
}

void display_update() {
  if (state == OFF_STATE) {
    display_off();
  } else {
    sensor s = sensors[state];
    display_sensor(&s);
  }
}

void display_sensor (sensor* s) {
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(s->name);display.println("");display.println("");
  display.setTextSize(2);

  if (s->plugged_in == 0) {
    display.print(" d/c");
    led_on(HOT);
  } else {
    display.print(s->t);display.print("c");
    led_on(s->h);
  }

    // this bit should get split out
  display.setTextSize(1);

  for (int i=0; i<NUMSENSORS; i++) {
    display.setCursor(110,i*8);
    display.print( (sensors[i].plugged_in && !sensors[i].alarm) ? "+" : "x" );
    display.print( s->name == sensors[i].name ? "<-" : " " );
  }

  display.display();
}

void display_off() {
  led_off();
  display.ssd1306_command(SSD1306_DISPLAYOFF);
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
