#ifndef F_CPU
#define F_CPU 9600000  /* Clock in Hz */
#endif

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

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
  OCR0A = 119; // should be (0.1/(1/(9600/8)))-1 = 119

  // knock interrupt
  DDRB &= ~(1<<PB1);
  PORTB |= (1<<PB1); // enable the pull-up
  GIMSK |= (1<<INT0); // interrupt on pb1
  ADCSRA &= ~(1<<ADEN); // adc off
  ACSR = (1<<ACD); // comparitor off
  //MCUCR = (1<<ISC01) | (0<<ISC00); // falling edge

  // pb2 to power switch servo
  DDRB |= (1<<PB2);
  PORTB &= ~(1<<PB2); // servo off

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  sei();

  DDRB |= (1<<PB3);
  pulse = 1;
  _delay_ms(250);
  DDRB &= ~(1<<PB3);

  DDRB |= (1<<PB4); // led on 4
  DDRB |= (1<<PB0); // led on and 0
  PORTB &= ~(1<<PB4);
  PORTB &= ~(1<<PB0);

}

void sleep_now() {
  //sleep_enable();
  GIMSK |= (1<<INT0); // interrupt on pb1
  sleep_mode(); // do it
  GIMSK &= ~(1<<INT0); // disable interrupt on pb1
  sleep_disable(); // wake up
}

void loop() {
  //_delay_ms(2000);
  if (state == HIGH) {
    PORTB |= (1<<PB4); // lights on
    PORTB |= (1<<PB0); // lights on
    _delay_ms(500);

    PORTB |= (1<<PB2); // servo on

    // 2 stage on the way up
    DDRB |= (1<<PB3);
    for(pulse=3; pulse<25; pulse++) {
      _delay_ms(60);
    }
    DDRB &= ~(1<<PB3);
    _delay_ms(1000);

    DDRB |= (1<<PB3);
    for(; pulse>2; pulse--) {
      _delay_ms(30);
    }
    DDRB &= ~(1<<PB3);

    _delay_ms(1000);
    PORTB &= ~(1<<PB4);
    PORTB &= ~(1<<PB0);

    PORTB &= ~(1<<PB2); // servo off

    state = LOW;
    sleep_now();
  }
}

ISR(INT0_vect) {
  //_delay_ms(240); // pulse is ~230ms
  state = HIGH;
}


ISR(TIM0_COMPA_vect) {
  static unsigned int tick = 0;

  // tick is about 0.1ms
  if (tick >= 400) { // 40ms window seems to work mostly
    tick = 0;
  }

  if (tick < pulse) { // (tick = 0.1ms (100us))
    PORTB |= (1<<PB3);
  } else {
    PORTB &= ~(1<<PB3);
  }
  tick = tick + 1;
}

