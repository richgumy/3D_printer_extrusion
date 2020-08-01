// Timer interrupt

#include "HX711.h" 

#define calibration_factor -20790.0 //This loadcell calibration value for measurement in Newtons

#define LOADCELL_DOUT_PIN  5
#define LOADCELL_SCK_PIN  4


#define DIR 3
#define STEP 2

long motor_steps_per_rev = 200;

long usteps_per_step = 10;

long linear_travel = 400; //[mm]

long pitch = 8; //[mm/rev]

long total_steps = motor_steps_per_rev * usteps_per_step * linear_travel / pitch;

long cur_step = 0;

long tick = 0;

int in_out = 0; //1 = out/up, 0 = in/down

HX711 scale;

void init_timer_int(){
  cli();//stop interrupts
  
  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 199;// = (16e6) / (10e3*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS21 bits for 8 prescaler
  TCCR0B |= (1 << CS21); 
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei();//allow interrupts
}

void pulse_IO(int pin, int pulse_width_us, int high_low){
  // pin:Digital IO pin to pulse, pulse_width_us:Pulse width in microseconds, high_low:Hold line high and pulse low(=0) or vice versa(=1)
  digitalWrite(pin, 1);
  delayMicroseconds(pulse_width_us);
  digitalWrite(pin, 0);
  delayMicroseconds(pulse_width_us);
}

void setup() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
  digitalWrite(DIR, in_out);
  digitalWrite(STEP, 0);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  init_timer_int();
  Serial.begin(9600);
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  // if step 'limit' reached change direction
  if (cur_step >= total_steps){
    in_out = !in_out;
    digitalWrite(DIR,in_out);
    cur_step = 0;
  }
  // Pulse stepper motor driver input high for 0.1ms
  if (!(tick%10)) {
    digitalWrite(STEP,1);
    digitalWrite(13,0);
  }
  else {
    digitalWrite(STEP,0);
    digitalWrite(13,1);
  }
  tick++;
  cur_step++;
}

void loop() {
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.println("N"); // or fluid pressure = force / pi*(11e-3)^2 -> force/2.630 = XX mPa
  delay(100);
}
