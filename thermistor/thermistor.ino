
#include "heats.h"
#include "resistance_to_celcius.c"

#define NUMSENSORS 3
sensor sensors[3];
boolean alarms[] = {false,false,false};

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
}

volatile uint8_t press = 0;
#define OFF_STATE 3
unsigned int state = OFF_STATE;
unsigned long button_ticks = 0;
unsigned long display_ticks = 0;

unsigned long sensor_ticks = 0;
#define POINTONE_SECOND 100
#define TEN_SECONDS 10000
#define THIRTY_SECONDS 30000

void button_press() {
  press += 1;
}

void loop (void) {
  unsigned long m = millis();

  // read temps update sensors every 10 seconds
  if (m-sensor_ticks > long(TEN_SECONDS)) {
    sensors[0].t = resistance_to_celcius(read_therm(A0,5));
    sensors[1].t = resistance_to_celcius(read_therm(A1,5));
    sensors[2].t = resistance_to_celcius(read_therm(A2,5));

    for (int i=0; i<NUMSENSORS; i++) {
      sensors[i].h = check_temp(sensors[i].t);
    }

    for (int i=0; i<NUMSENSORS; i++) {
      alarms[i] = is_alarm(&sensors[i]);
    }

    sensor_ticks = m;
  }

  // if the display has been on for 10 seconds since the
  // last button, turn it off
  if (state < OFF_STATE && m-button_ticks > long(TEN_SECONDS)) {
    state = OFF_STATE;
    button_ticks = 0;
  }

  // check for button presses every 25ms
  if(m-button_ticks > long(25) && press > 0 ) {
    button_ticks = m;
    state = (state+1)%(OFF_STATE);//+1);
   // if (state == OFF_STATE) {
     // button_ticks = 0;
    //}
    press = 0;
  }

  // update display 10 times a second
  if (m-display_ticks > long(POINTONE_SECOND)) {
    display_update();
    display_ticks = m;
  }

}
