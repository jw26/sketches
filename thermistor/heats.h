
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
};

How_hot check_temp(double temp) {
  How_hot heat = OK;
  if (temp < 18.00) {
    heat = CHILLY;
    if (temp < 17.00) {
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

unsigned int is_alarm (sensor* s) {
  return s->h != OK;
}


