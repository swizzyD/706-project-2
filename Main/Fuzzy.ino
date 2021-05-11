
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
  if (reading > 250) {
    obstacle = 0;
    clear = 1;
  }
  else if (reading < 150) {
    obstacle = 1;
    clear = 0;
  }
  else {
    clear = 1 - (reading - 150) / 100;
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

void fuzzify_pt_left(){
  double fuzzy, light, clear;
  double reading = PT_LEFT_READING;
  //ultrasonic obstacle detection
  if (reading > 700) {
    light = 0;
    clear = 1;
  }
  else if (reading < 50) {
    light = 1;
    clear = 0;
  }
  else {
    clear = 1 - (reading - 50) / 650;
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

void fuzzify_pt_mid(){
  double fuzzy, light, clear;
  double reading = PT_MID_READING;
  //ultrasonic obstacle detection
  if (reading > 800) {
    light = 0;
    clear = 1;
  }
  else if (reading < 50) {
    light = 1;
    clear = 0;
  }
  else {
    clear = 1 - (reading - 50) / 750;
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

void run_inference() {
  
  //obstacle avoidance
  if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "clear") {
    strafe_right(ir_2_fuzzy.value *100);
    SerialCom->println("strafe right");
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "obstacle") {
    strafe_left(ir_1_fuzzy.value*100);
    SerialCom->println("strafe left");
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "clear" && PT_mid_fuzzy.set == "clear") {
    reverse(200);
    SerialCom->println("reverse");
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "clear" && ir_2_fuzzy.set == "obstacle") {
    stop();
    SerialCom->println("stop");
  }
  else if (ir_1_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "obstacle") {
    ccw(ir_1_fuzzy.value*100);
    SerialCom->println("ccw");
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "clear") {
    cw(ir_2_fuzzy.value*100);
    SerialCom->println("cw");
  }
  else if (ir_1_fuzzy.set == "obstacle" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "obstacle") {
    stop();
    SerialCom->println("stop");
  }


  //light detection
else if(PT_mid_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "clear"){
  cw(100);
  SerialCom->println("spin");
}

else if(PT_mid_fuzzy.set == "light" && ultrasonic_fuzzy.set == "clear"){
  forward(ultrasonic_fuzzy.value * 200);
  SerialCom->println("forward");
}

  //robot in range fpr fire distingush
else if(PT_mid_fuzzy.set == "light" && ultrasonic_fuzzy.set == "obstacle"){
  stop();
  SerialCom->println("stop");
}
else{
  stop();
  SerialCom->println("no operation");
}
  
}
