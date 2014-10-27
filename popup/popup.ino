#ifndef F_CPU
#define F_CPU 9600000  /* Clock in Hz */
#endif

#include <avr/interrupt.h>
#include <util/delay.h>

volatile int state = LOW;
volatile uint8_t pulse;

void setup()
{

  cli();
  // Set up the interrupt that will refresh the servo for us automagically
  TCCR0A = 0; TCCR0B = 0; // because i think the arduino bits set stuff before we get here.
  TIMSK0 |= (1<<OCIE0A);    // Turn on the compare interrupt
  TCCR0B |= (1<<CS00); // no prescaler
  TCCR0A |= (1<<WGM01);     // CTC
  OCR0A = 95; // (0.1/((1/(9600))))-1 think this is wrong. forgot about clk/8 i think
              // should be (0.1/(1/(9600/8)))-1 = 119.. might that explain the drift on 20ms?

  // knock interrupt
  DDRB &= ~(1<<PB1);
  PORTB |= (1<<PB1); // enable the pull-up
  GIMSK = 1<<INT0; // interrupt on pb1
  MCUCR = (1<<ISC01) | (0<<ISC00); // falling edge

  sei();

  DDRB |= (1<<PB3);
  pulse = 3;
  _delay_ms(150);
  DDRB &= ~(1<<PB3);

  DDRB |= (1<<PB4); // led on 4
  DDRB |= (1<<PB0); // led on and 0
  PORTB &= ~(1<<PB4);
  PORTB &= ~(1<<PB0);

}

void loop() {
  if (state == HIGH) {
    PORTB |= (1<<PB4); // lights on
    PORTB |= (1<<PB0); // lights on
    _delay_ms(500);

    // 2 stage on the way up
    for(int i=3; i<=10; i++) {
      DDRB |= (1<<PB3);
      pulse = pulse + 1;
      _delay_ms(10);
      DDRB &= ~(1<<PB3);
    }
    for(int i=10; i<=28; i++) {
      DDRB |= (1<<PB3);
      pulse = pulse + 1;
      _delay_ms(60);
      DDRB &= ~(1<<PB3);
    }
    pulse = 28;
    _delay_ms(1000);


    for(int i=3; i<=28; i++) {
      DDRB |= (1<<PB3);
      pulse = pulse - 1;
      _delay_ms(10);
      DDRB &= ~(1<<PB3);
    }
    pulse = 3;

    _delay_ms(500);
    PORTB &= ~(1<<PB4);
    PORTB &= ~(1<<PB0);

    state = LOW;
  }
}

ISR(INT0_vect) {
  _delay_ms(240); // pulse is ~230ms
  state = HIGH;
}


ISR(TIM0_COMPA_vect) {
  static unsigned int tick = 0;

  // tick is about 0.1ms
  if (tick >= 220) { // every 20ms is 250?!?!?, must be loosing cycles somewhere
    tick = 0;
  }

  if (tick < pulse) { // (tick = 0.1ms (100us))
    PORTB |= (1<<PB3);
  } else {
    PORTB &= ~(1<<PB3);
  }
  tick = tick + 1;
}

