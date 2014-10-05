#include <stddef.h>
#include <Arduino.h>

enum How_hot {
  COLD,
  CHILLY,
  OK,
  WARM,
  HOT
};


struct sensor_t {
  char* name;
  double t;
  How_hot h;
  unsigned short plugged_in;
  unsigned short alarm;
  const uint8_t input;
  struct sensor_t *next;

  sensor_t(char* n, const uint8_t i, struct sensor_t* ne):
    name(n), t(0.0), h(OK), plugged_in(1), alarm(0), input(i),
    next(ne) {}

  public:
    How_hot check_temp (double temp);
    unsigned int is_alarm (struct sensor_t* s);
    void update(double temp);

};

void sensor_t::update(double temp) {
  this->t = temp;


  if (temp < -50.0 || temp > 150.00) { // thats probably an error
    this->plugged_in = 0;
  } else {
    this->plugged_in = 1;
  }

  this->h = check_temp(temp);
  this->alarm = is_alarm(this);
}

How_hot sensor_t::check_temp(double temp) {
  How_hot heat = OK;
  if (temp < 17.00) {
    heat = CHILLY;
    if (temp < 15.00) {
      heat = COLD;
    }
  } else if (temp > 20.00) {
    heat = WARM;
    if (temp > 24.00) {
      heat = HOT;
    }
  }
  return heat;
}


unsigned int sensor_t::is_alarm (struct sensor_t* s) { // mixing things a bit, needs another look
  return s->h < CHILLY || s->h > WARM;
}


