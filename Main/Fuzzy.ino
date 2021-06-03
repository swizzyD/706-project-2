
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
  if (reading > 300) {
    obstacle = 0;
    clear = 1;
  }
  else if (reading < 150) {

    obstacle = 1;
    clear = 0;
  }
  else {
    clear = (reading - 100) / 150;
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
//  else if (reading < 50) {
  else if (reading < 50){
    light = 1;
    clear = 0;
  }
  else {
//    clear = (reading - 50) / 550;
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
//  else if (reading < 700) {
  else if (reading < 80){
    light = 1;
    clear = 0;
  }
  else {
    clear = (reading - 80) / 720;
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
  if (reading < 40) {
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

void fuzzify_pt_right() {
  double fuzzy, light, clear;
  double reading = PT_Right.get_raw_reading();
    if (reading > 600){
    light = 0;
    clear = 1;
  }
    else if (reading < 50){
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
    cw(150);
    delay(400);
    SerialCom->println("front and back obstacle");
  }
  else if (ultrasonic_fuzzy.set == "obstacle" && PT_mid_fuzzy.set == "clear" && ir_3_fuzzy.set == "clear" && PT_top_fuzzy.set == "clear") {
    turret_motor.Write(80); 
    if (toggle == true){
      count++;
    }
    toggle = false;
    reverse(ultrasonic_fuzzy.value * 150);
    delay(400);
    SerialCom->println("front obstacle");
  }
  else if (ultrasonic_fuzzy.set == "clear" && ir_1_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "clear") {
    strafe_right(ir_1_fuzzy.value * 250);
    delay(400);
    SerialCom->println("left obstacle");
  }
  else if (ultrasonic_fuzzy.set == "clear" && ir_1_fuzzy.set == "clear" && ir_2_fuzzy.set == "obstacle") {
    strafe_left(ir_2_fuzzy.value * 250);
    delay(400);
    SerialCom->println("right obstacle");
  }
  else if (ultrasonic_fuzzy.set == "obstacle" && ir_1_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "clear") {
    cw(ir_1_fuzzy.value * 250);
    delay(400);
    SerialCom->println("left & front obstacle");
  }
  else if (ultrasonic_fuzzy.set == "obstacle" && ir_1_fuzzy.set == "clear" && ir_2_fuzzy.set == "obstacle") {
    ccw(ir_2_fuzzy.value * 250);
    delay(400);
    SerialCom->println("right & front obstacle");
  }

  //light detect
  else if (PT_top_fuzzy.set == "light" && (PT_mid_fuzzy.set == "light" || PT_left_fuzzy.set == "light" || PT_right_fuzzy.set == "light") && ultrasonic_fuzzy.set == "obstacle") {
    //stop();
    int pos = turret_motor.Track(PT_Mid.get_raw_reading());
    forward(300*(1 - ultrasonic_fuzzy.value));
    SerialCom->print("top light & servo track ");
    SerialCom->println(pos);
    toggle = true;
  }
  else if (PT_mid_fuzzy.set == "light" && PT_top_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_2_fuzzy.set == "obstacle") {
    strafe_left(ultrasonic_fuzzy.value * 250);
  }
  else if (PT_mid_fuzzy.set == "light" && PT_top_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_1_fuzzy.set == "obstacle") {
    strafe_right(ultrasonic_fuzzy.value * 250);
  }
  else if (PT_mid_fuzzy.set == "light" && PT_top_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "clear" && ir_1_fuzzy.set == "clear" && ir_2_fuzzy.set == "clear") {
    forward(150);
    SerialCom->println("mid light");
  }
  else if (PT_mid_fuzzy.set == "light" && PT_top_fuzzy.set == "clear" && ultrasonic_fuzzy.set == "obstacle" && ir_1_fuzzy.set == "clear"  && ir_2_fuzzy.set == "clear") {
    strafe_left(250);
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
    SerialCom->println("FUCKED UP");
    if (dir == "ccw") {
      ccw(100);
    }
    else {
      cw(100);
    }
  }

}
void lillian(){
  if (count == 2){
    stop();
  }
}
