
String dir = "cw";


void fuzzify_ir_1() {
  double fuzzy, obstacle, clear;
  double reading = IR_1.get_dist();
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
    clear = (reading - 50) / 50;
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
  double reading = IR_2.get_dist();
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
    clear = (reading - 50) / 50;
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

//--------------------------------------------------------------------

void fuzzify_ir_3() {
  double fuzzy, obstacle, clear;
  double reading = IR_3.get_dist();
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
    ir_3_fuzzy.set = "obstacle";
    ir_3_fuzzy.value = obstacle;
  }
  else {
    ir_3_fuzzy.set = "clear";
    ir_3_fuzzy.value = clear;
  }
}
//--------------------------------------------------------------

void fuzzify_ultrasonic() {
  double fuzzy, obstacle, clear;
  double reading = Ultrasonic.get_dist();
  //ultrasonic obstacle detection
  if (reading > 250) {
    obstacle = 0;
    clear = 1;
  }
  else if (reading < 150) {
    obstacle = 1;
    clear = 0;
  }
  else {
    clear = (reading - 150) / 100;
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

void fuzzify_pt_left() {
  double fuzzy, light, clear;
  double reading = PT_Left.get_raw_reading();
  //ultrasonic obstacle detection
  if (reading > 600) {
    light = 0;
    clear = 1;
  }
  else if (reading < 50) {
    light = 1;
    clear = 0;
  }
  else {
    clear = (reading - 50) / 550;
    light = 1 - clear;
  }

  if (light > clear) {
    PT_left_fuzzy.set = "light";
    PT_left_fuzzy.value = light;
  }
  else {
    PT_left_fuzzy.set = "clear";
    PT_left_fuzzy.value = clear;
  }
}

//------------------------------------------------------

void fuzzify_pt_mid() {
  double fuzzy, light, clear;
  double reading = PT_Mid.get_raw_reading();
  //ultrasonic obstacle detection
  if (reading > 900) {
    light = 0;
    clear = 1;
  }
  else if (reading < 700) {
    light = 1;
    clear = 0;
  }
  else {
    clear = (reading - 700) / 200;
    light = 1 - clear;
  }

  if (light > clear) {
    PT_mid_fuzzy.set = "light";
    PT_mid_fuzzy.value = light;
  }
  else {
    PT_mid_fuzzy.set = "clear";
    PT_mid_fuzzy.value = clear;
  }
}

//------------------------------------------------------

void fuzzify_pt_top() {
  double fuzzy, light, clear;
  double reading = PT_Top.get_raw_reading();
  //ultrasonic obstacle detection
  if (reading < 700) {
    light = 1;
    clear = 0;
  }
  else {
    clear = 1;
    light = 0;
  }

  if (light > clear) {
    PT_top_fuzzy.set = "light";
    PT_top_fuzzy.value = light;
  }
  else {
    PT_top_fuzzy.set = "clear";
    PT_top_fuzzy.value = clear;
  }
}

//------------------------------------------------------

void fuzzify_pt_right() {
  double fuzzy, light, clear;
  double reading = PT_Right.get_raw_reading();
  //ultrasonic obstacle detection
  if (reading > 600) {
    light = 0;
    clear = 1;
  }
  else if (reading < 50) {
    light = 1;
    clear = 0;
  }
  else {
    clear = (reading - 50) / 550;
    light = 1 - clear;
  }

  if (light > clear) {
    PT_right_fuzzy.set = "light";
    PT_right_fuzzy.value = light;
  }
  else {
    PT_right_fuzzy.set = "clear";
    PT_right_fuzzy.value = clear;
  }
}

//------------------------------------------------------

void run_inference() {
  //obstacle avoidance
  if (ultrasonic_fuzzy.set == "obstacle" && PT_mid_fuzzy.set == "clear" && ir_3_fuzzy.set == "obstacle") {
    stop();
  }
  else if (ultrasonic_fuzzy.set == "obstacle" && PT_mid_fuzzy.set == "clear" && ir_3_fuzzy.set == "clear") {
    toggle = false;
    turret_motor.write(80);
    reverse(ultrasonic_fuzzy.value * 150);
  }
  else if (ir_1_fuzzy.set == "obstacle") {
    strafe_right(ir_1_fuzzy.value * 150);
  }
  else if (ir_2_fuzzy.set == "obstacle") {
    strafe_left(ir_2_fuzzy.value * 150);
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "obstacle") {
    cw(ir_1_fuzzy.value * 150);
  }
  else if (ir_2_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "obstacle") {
    ccw(ir_2_fuzzy.value * 150);
  }

  //light detect
  else if (PT_top_fuzzy.set == "light" && (PT_mid_fuzzy.set == "light" || PT_left_fuzzy.set == "light" || PT_right_fuzzy.set == "light")) {
    stop();
    servo_sweep();
    toggle = true;
  }
  else if (PT_mid_fuzzy.set == "light" && PT_top_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" ) {
    strafe_left(ultrasonic_fuzzy.value * 150);
  }
  else if (PT_mid_fuzzy.set == "light" && PT_top_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "clear") {
    forward(150);
  }

  else if (PT_left_fuzzy.set == "light" && ultrasonic_fuzzy.set == "clear") {
    ccw(PT_left_fuzzy.value * 150);
    dir = "ccw";
  }
  else if (PT_right_fuzzy.set == "light" && ultrasonic_fuzzy.set == "clear") {
    cw(PT_right_fuzzy.value * 150);
    dir = "cw";
  }

  else {
    if (dir == "ccw") {
      ccw(120);
    }
    else {
      cw(120);
    }
  }

}

void servo_sweep() {
  if (PT_mid_fuzzy.set == "light" && ultrasonic_fuzzy.set == "obstacle") {
    turret_motor.write(80);
  }
  else if (PT_right_fuzzy.set == "light" && ultrasonic_fuzzy.set == "obstacle") {
    turret_motor.write(35);
  }
  else if (PT_left_fuzzy.set == "light" && ultrasonic_fuzzy.set == "obstacle") {
    turret_motor.write(135);
  }
  else {
    turret_motor.write(80);
  }
}
