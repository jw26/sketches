
#include "Sensor.h"
#include "Gecko.h"
#include "Chameleon.h"
#include "resistance_to_celcius.c"

#define POINTONE_SECOND 100
#define TEN_SECONDS 10000
#define THIRTY_SECONDS 30000

Sensor* head = NULL;

void setup(void) {
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(0,button_press,FALLING);
  randomSeed(analogRead(A5));

  display_init();

  //Sensor* s4 = new sensor_t( "Finger", A3, NULL );
  Sensor* s3 = new Chameleon( "chameleon", A2, NULL );
  Sensor* s2 = new Gecko( "Mr and Mrs Gecko", A1, s3 );
  head = new Sensor( "Room", A3, s2 );

}

void sensors_poll() {
  Sensor* c = head;
  while (c != NULL) {
    c->update(resistance_to_celcius(read_therm(c->input,5)));

    c = c->next;
  }
}

volatile uint8_t press = 0;

void button_press() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    press += 1;
  }
  last_interrupt_time = interrupt_time;
}

void loop (void) {
  unsigned long m = millis();
  static unsigned long button_ticks = 0;
  static unsigned long display_ticks = 0;
  static unsigned long sensor_ticks = 0;
  static Sensor* current = NULL;

  // read temps update sensors every 10 seconds
  if (sensor_ticks == 0 || m-sensor_ticks > long(TEN_SECONDS)) {
    sensors_poll();

    sensor_ticks = m;
  }

  // if the display has been on for 10 seconds since the
  // last button, turn it off
  if (current != NULL && m-button_ticks > long(TEN_SECONDS)) {
    current = NULL;
    button_ticks = 0;
  }

  // if off, but alarms, shift to the busted one
  if (current == NULL) {
    Sensor* c = head;
    while (c) {
      if (c->alarm == 1) {
        current = c;
        break;
      }
      c = c->next;
    }
  }

  // check for button presses every 20ms
  if(m-button_ticks > long(20) && press > 0 ) {
    if (current == NULL) {
      current = head;
    } else {
      current = current->next == NULL ? head : current->next;
    }
    press = 0;
    button_ticks = m;
  }

  // update display 10 times a second
  if (m-display_ticks > long(POINTONE_SECOND)) {
    display_update(current);
    display_ticks = m;
  }

}

