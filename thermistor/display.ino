#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


static const unsigned char PROGMEM img_bmp[] = {
0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x18, 0x0, 0x18, 0x0, 0x20, 0xc, 0x0, 0x70, 0x0, 
0x1c, 0x4, 0x70, 0xc0, 0xe0, 0x3, 0x6, 0x88, 0x3, 0x80, 0x1, 0x80, 0x88, 0x1c, 0x4, 
0x0, 0xc0, 0x88, 0x70, 0x1c, 0x0, 0x20, 0xf8, 0x40, 0xc0, 0x0, 0x0, 0xf8, 0x3, 0x80, 
0x0, 0x0, 0xf8, 0xe, 0x0, 0x1c, 0x0, 0xf8, 0x18, 0x7, 0x7, 0xc0, 0xf8, 0x0, 0x3c, 
0x0, 0x0, 0xf8, 0x1, 0xe0, 0x0, 0x0, 0xf8, 0x3, 0x0, 0x0, 0x0, 0xf8, 0xc, 0x0, 
0x0, 0x0, 0xf8, 0x0, 0x0, 0x0, 0x0, 0xf8, 0x0, 0x0, 0x3, 0x80, 0xf8, 0x0, 0xe, 
0x1e, 0x0, 0xf8, 0x0, 0xf0, 0x30, 0x1, 0xfc, 0x7, 0x80, 0x0, 0x3, 0xfe, 0x0, 0x0, 
0x0, 0x7, 0xff, 0x0, 0x0, 0x0, 0x7, 0xff, 0x0, 0x0, 0x1, 0x87, 0xff, 0x1e, 0x0, 
0x3, 0x7, 0xff, 0x3, 0xf8, 0x2, 0x7, 0xff, 0x0, 0x0, 0x0, 0x3, 0xfe, 0x20, 0x0, 
0x0, 0x1, 0xfc, 0x38, 0x0, 0x0, 0x8, 0xf8, 0xc, 0x0, 0x0, 0x18, 0x0, 0x2, 0x0, 
0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
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
    boolean alarm = false;
      // just break on first for now
    for (int i=0; i<NUMSENSORS; i++) {
      if (alarms[i]) {
        display_sensor(&sensors[i]);
        alarm = true;
        break;
      }
    }
    if (!alarm) {
      display_off();
    }
  } else {
    sensor s = sensors[state];
    display_sensor(&s);
  }
}

void display_sensor (sensor* s) {
  led_on(s->h);
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(s->name);display.println("");display.println("");
  display.setTextSize(2);
  display.print(s->t);display.print(" *C");

  if (is_alarm(s)) {
    display.drawBitmap(90, 0,  img_bmp, 40, 32, 1);
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
