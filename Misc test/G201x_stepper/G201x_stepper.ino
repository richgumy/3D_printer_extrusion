// G201x_stepper.file
// Gecko G201x Stepper Motor drive with Arduino Nano
// Author: Richie Ellingham
// Date: 31 Jul 2020
// v1.0

// To do:
// - make steps all interrupt based
// - set intrpt clk for steper speed (max. 200kHz)
// - define dirs
// - Make G201x_stepper.h

#include "HX711.h" 

#define calibration_factor -20790.0 //This loadcell calibration value for measurement in Newtons

#define LOADCELL_DOUT_PIN  5
#define LOADCELL_SCK_PIN  4

#define DIR 3
#define STEP 2

long motor_steps_per_rev = 200;

long usteps_per_step = 10;

long linear_travel = 30; //[mm]

long pitch = 8; //[mm/rev]

long total_steps = motor_steps_per_rev * usteps_per_step * linear_travel / pitch;

int in_out = 0; //1 = out/up, 0 = in/down

HX711 scale;

void setup() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
  
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(DIR, in_out);
  digitalWrite(STEP, 0);
  Serial.begin(9600);
  Serial.println(total_steps);
  delay(1000);
}

void loop() {
  for (int i=0;i<total_steps;i++){
    pulse_IO(STEP, 4, 1);
    if (!(i%1000)){
      delayMicroseconds(996); // rot speed: 0.18deg / 0.0001 = 1800deg/s; linear travel 2mm/360deg -> 10mm/s
      Serial.print("Reading: ");
      Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
      Serial.println("N");
    }

  }
  delay(100);
  in_out = !in_out;
  digitalWrite(DIR, in_out);
  delay(100);
;
}


void pulse_IO(int pin, int pulse_width_us, int high_low){
  // pin:Digital IO pin to pulse, pulse_width_us:Pulse width in microseconds, high_low:Hold line high and pulse low(=0) or vice versa(=1)
  digitalWrite(pin, 1);
  delayMicroseconds(pulse_width_us);
  digitalWrite(pin, 0);
  delayMicroseconds(pulse_width_us);
}
