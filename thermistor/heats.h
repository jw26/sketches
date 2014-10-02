
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
  byte alarm = 0x0;
};
