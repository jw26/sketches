#include <stddef.h>
#include <Arduino.h>

enum Heat {
  COLD,
  CHILLY,
  OK,
  WARM,
  HOT
};


class Sensor {

  public:
    Sensor(char* n, const uint8_t i, Sensor* ne):
      name(n), t(0.0), h(OK), plugged_in(1), alarm(0), input(i),
      next(ne) {}

    virtual char* display_name() { return name; }
    virtual Heat check_temp (double temp);
    char* name;
    double t;
    Heat h;
    unsigned short plugged_in;
    unsigned short alarm;
    const uint8_t input;
    Sensor* next;
    unsigned int is_alarm (Sensor* s);
    void update(double temp);

};

void Sensor::update(double temp) {
  this->t = temp;


  if (temp < -50.0 || temp > 150.00) { // thats probably an error
    this->plugged_in = 0;
  } else {
    this->plugged_in = 1;
  }

  this->h = check_temp(temp);
  this->alarm = is_alarm(this);
}

Heat Sensor::check_temp(double temp) { // room temp for us
  Heat heat = OK;
  if (temp < 16.00) {
    heat = CHILLY;
    if (temp < 12.00) {
      heat = COLD;
    }
  } else if (temp > 22.00) {
    heat = WARM;
    if (temp > 26.00) {
      heat = HOT;
    }
  }
  return heat;
}


unsigned int Sensor::is_alarm (Sensor* s) { // mixing things a bit, needs another look
  return s->h < CHILLY || s->h > WARM;
}

