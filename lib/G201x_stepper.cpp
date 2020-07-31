// G201x_Stepper.cpp

void pulse(uint_8 STEP_pin, uint_8 pulse_width_us, bool norm_low) {
  // Stepper step IO pin; Pulse width in microseconds; Normally logic low step IO pin = 1, normally logic high = 0
  // start timer
  digitalWrite(STEP_pin, norm_low);
  // loop until current time - start time == pulse_width_us
  digitalWrite(STEP_pin, !norm_low);
}

bool set_rot_speed(uint_8 rot_speed) {
  // Set the stepper motor rotational speed
  // output: 0 if rot_speed beyond limits, else 1
}
