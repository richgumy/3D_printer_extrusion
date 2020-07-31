// G201x_Stepper.h

void pulse(uint_8 STEP_pin, uint_8 pulse_width_us, bool norm_low) {
  // Stepper step IO pin; Pulse width in microseconds; Normally logic low step IO pin = 1, normally high = 0
}

bool set_rot_speed(uint_8 rot_speed) {
  // Set the stepper motor rotational speed
  // output: 0 if rot_speed beyond limits, else 1
}
