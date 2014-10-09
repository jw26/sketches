
#include "names.cpp"

class Chameleon: public Sensor {
  public:
    Chameleon(char* n, const uint8_t i, Sensor* ne):
      Sensor(n, i, ne) {
        ticks = 0;
        m = 0;
        strcpy_P(name_buffer, (char*)pgm_read_word(&(names[random(NUM_NAMES)])));
      }

    Heat check_temp (double temp);

    char name_buffer[20];
    unsigned long ticks;
    unsigned long m;

    char* display_name() { // sillyness, random name every 12 hours
      m = millis();

      if (m-ticks > long(43200000)) { // 12 hours
        strcpy_P(name_buffer, (char*)pgm_read_word(&(names[random(NUM_NAMES)])));
        ticks = m;
      }

      return name_buffer;
    }

};

Heat Chameleon::check_temp(double temp) { // 19 (at night) - 32
  Heat heat = OK;
  if (temp < 24.00) {
    heat = CHILLY;
    if (temp < 20.00) {
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
