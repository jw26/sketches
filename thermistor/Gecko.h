
class Gecko: public Sensor {
  public:
    Gecko(char* n, const uint8_t i, Sensor* ne):
      Sensor(n, i, ne) {
      }

    Heat check_temp (double temp);

};

Heat Gecko::check_temp(double temp) { // 25 to 32
  Heat heat = OK;
  if (temp < 22.00) {
    heat = CHILLY;
    if (temp < 18.00) {
      heat = COLD;
    }
  } else if (temp > 30.00) {
    heat = WARM;
    if (temp > 32.00) {
      heat = HOT;
    }
  }
  return heat;
}
