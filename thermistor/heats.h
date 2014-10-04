
enum How_hot {
  COLD,
  CHILLY,
  OK,
  WARM,
  HOT
};

struct sensor {
  char *name;
  double t = 0.0;
  How_hot h = OK;
  unsigned short plugged_in = 1;
  unsigned short alarm = 0;
};

How_hot check_temp(double temp) {
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

unsigned int is_alarm (sensor* s) { // mixing things a bit, needs another look
  if (s->t < -50.0 || s->t > 150.00) { // thats probably an error
    s->plugged_in = 0;
  } else {
    s->plugged_in = 1;
  }
  return s->h < CHILLY || s->h > WARM;
}


