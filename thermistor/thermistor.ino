
#include "heats.h"
#include "resistance_to_celcius.c"

#define OFF_STATE 3
#define NUMSENSORS 3
sensor sensors[NUMSENSORS];
uint8_t inputs[] = {A0,A1,A2,A3};

void setup(void) {
  Serial.begin(9600);
  // connect AREF to 3.3V and use that as VCC, less noisy!
  analogReference(EXTERNAL);

  pinMode(2,INPUT_PULLUP);
  attachInterrupt(0,button_press,FALLING);

  display_init();

  sensors[0].name = "Mr Snake        ";
  sensors[1].name = "Mr and Mrs Gecko";
  sensors[2].name = "Phyllis";
  sensors[3].name = "Room";
}

volatile uint8_t press = 0;
unsigned int state = OFF_STATE;
unsigned long button_ticks = 0;
unsigned long display_ticks = 0;

unsigned long sensor_ticks = 0;
#define POINTONE_SECOND 100
#define TEN_SECONDS 10000
#define THIRTY_SECONDS 30000

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

  // read temps update sensors every 10 seconds
  if (m-sensor_ticks > long(TEN_SECONDS)) {
    for (int i=0; i<NUMSENSORS; i++) {
      sensors[i].t = resistance_to_celcius(read_therm(inputs[i],5));
      sensors[i].h = check_temp(sensors[i].t);
      sensors[i].alarm = is_alarm(&sensors[i]);
    }

    sensor_ticks = m;
  }

  // if the display has been on for 10 seconds since the
  // last button, turn it off
  if (state < OFF_STATE && m-button_ticks > long(TEN_SECONDS)) {
    state = OFF_STATE;
    button_ticks = 0;
  }

  // if off, but alarms, shift to the busted one
  if (state == OFF_STATE) {
    for (int i=0; i<NUMSENSORS; i++) {
      if (sensors[i].alarm == 1) {
        state = i;
        break;
      }
    }
  }

  // check for button presses every 20ms
  if(m-button_ticks > long(20) && press > 0 ) {
    state = (state+1)%(NUMSENSORS+1);
    press = 0;
    button_ticks = m;
  }

  // update display 10 times a second
  if (m-display_ticks > long(POINTONE_SECOND)) {
    display_update();
    display_ticks = m;
  }

}
