#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BLUE 3
#define GREEN 11
#define RED 10

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK    8
#define OLED_DC     7
#define OLED_CS     5
#define OLED_RESET  6
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

char name_buffer[20];

void display_init () {
  pinMode(BLUE,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("hello");
  display.display();
  led_cycle();
}

void display_update(Sensor* current) {
  if (current == NULL) {
    display_off();
  } else {
    display_sensor(current);
  }
}

void debug(char* s, Sensor* c) {
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(s);
  if (c != NULL) {
    display.print(c->t);
  }
  display.display();
}

void display_sensor (Sensor* s) {
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(s->display_name());display.println("");display.println("");
  display.setTextSize(2);

  if (s->plugged_in == 0) {
    display.print(" d/c");
    led_on(HOT);
  } else {
    display.print(s->t);
    display.print("c");
    led_on(s->h);
  }

    // this bit should get split out
  display.setTextSize(1);

  Sensor* c = head;
  int i = 0;
  while (c != NULL) {
    display.setCursor(110,i*8);
    display.print( (c->plugged_in && !c->alarm) ? "+" : "x" );
    display.print( s->name == c->name ? "<-" : " " );
    c = c->next;
    i++;
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
void led_on(Heat hot) {
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

// borrowed from https://gist.github.com/jamesotron/766994
void led_cycle() {
  unsigned int rgbColour[3];

  led_setRGB(0,0,0);

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;

      led_setRGB(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(10);
    }
  }
}

void led_setRGB(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(BLUE, blue);
  analogWrite(GREEN, green);
  analogWrite(RED, red);
}
