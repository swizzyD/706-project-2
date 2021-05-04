
void fuzzify_ir_1() {
  double fuzzy, obstacle, clear;
  double reading = get_ir_1();
  //IR obstacle detection
  if (reading > 100) {
    obstacle = 0;
    clear = 1;
  }
  else if (reading < 50) {
    obstacle = 1;
    clear = 0;
  }
  else {
    clear = 1 - (reading - 50) / 50;
    obstacle = 1 - clear;
  }

  if (obstacle > clear) {
    ir_1_fuzzy.set = "obstacle";
    ir_1_fuzzy.value = obstacle;
  }
  else {
    ir_1_fuzzy.set = "clear";
    ir_1_fuzzy.value = clear;
  }
}

//--------------------------------------------------------------------

void fuzzify_ir_2() {
  double fuzzy, obstacle, clear;
  double reading = get_ir_2();
  //IR obstacle detection
  if (reading > 100) {
    obstacle = 0;
    clear = 1;
  }
  else if (reading < 50) {
    obstacle = 1;
    clear = 0;
  }
  else {
    clear = 1 - (reading - 50) / 50;
    obstacle = 1 - clear;
  }

  if (obstacle > clear) {
    ir_2_fuzzy.set = "obstacle";
    ir_2_fuzzy.value = obstacle;
  }
  else {
    ir_2_fuzzy.set = "clear";
    ir_2_fuzzy.value = clear;
  }
}

//--------------------------------------------------------------

void fuzzify_ultrasonic() {
  double fuzzy, obstacle, clear;
  double reading = get_ultrasonic_distance();
  //ultrasonic obstacle detection
  if (reading > 100) {
    obstacle = 0;
    clear = 1;
  }
  else if (reading < 50) {
    obstacle = 1;
    clear = 0;
  }
  else {
    clear = 1 - (reading - 50) / 50;
    obstacle = 1 - clear;
  }

  if (obstacle > clear) {
    ultrasonic_fuzzy.set = "obstacle";
    ultrasonic_fuzzy.value = obstacle;
  }
  else {
    ultrasonic_fuzzy.set = "clear";
    ultrasonic_fuzzy.value = clear;
  }
}

//-----------------------------------------------------

void run_inference() {
  
  //obstacle avoidance
  if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "clear") {
    strafe_right(ir_2_fuzzy.value);
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "obstacle") {
    strafe_left(ir_1_fuzzy.value);
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "clear") {
    reverse(200);
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "obstacle") {
    stop();
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "obstacle") {
    ccw(ir_1_fuzzy.value);
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "clear") {
    cw(ir_2_fuzzy.value);
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "obstacle") {
    stop();
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "clear") {
    forward(ultrasonic_fuzzy.value);
  }

  //light detection

  
}
