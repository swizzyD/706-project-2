/*
  Hardware:
    Arduino Mega2560 https://www.arduino.cc/en/Guide/ArduinoMega2560
    Ultrasonic Sensor - HC-SR04 https://www.sparkfun.com/products/13959
    Infrared Proximity Sensor - Sharp https://www.sparkfun.com/products/242
    Infrared Proximity Sensor Short Range - Sharp https://www.sparkfun.com/products/12728
    Servo - Generic (Sub-Micro Size) https://www.sparkfun.com/products/9065
    Vex Motor Controller 29 https://www.vexrobotics.com/276-2193.html
    Vex Motors https://www.vexrobotics.com/motors.html
    Turnigy nano-tech 2200mah 2S https://hobbyking.com/en_us/turnigy-nano-tech-2200mah-2s-25-50c-lipo-pack.html
*/

#include <SoftwareSerial.h>
#include <math.h>
#include <Servo.h>  //Need for Servo pulse output
#include "PID_class.h"
#include "Fuzzy_Output_Struct.h"
#include "Sensors_class.h"
#include "Turret_class.h"



//#define NO_BATTERY_V_OK //Uncomment of BATTERY_V_OK if you do not care about battery damage.
#define DISP_READINGS 1
#define BLUETOOTH 0
#define SAMPLING_TIME 50 //ms , operate at 20Hz
#define GYRO_READING analogRead(A3)
#define IR_1_READING analogRead(A4)
#define IR_2_READING analogRead(A6)
#define IR_3_READING analogRead(A7)
#define PT_TOPREADING analogRead(A12)
#define PT_LEFT_READING analogRead(A13)
#define PT_MID_READING analogRead(A14)
#define PT_RIGHT_READING analogRead(A15)

#define GYRO_TARGET_ANGLE 90
#define ULTRASONIC_MOVE_THRESH 100

static int count = 0;


//machine states
enum STATE {
  INITIALISING,
  RUNNING,
  STOPPED
};

//-------------------------------------------GYRO VARIABLES--------------------------------------------------------
static float gyroSupplyVoltage = 5;    // supply voltage for gyro
static float gyroZeroVoltage = 0;      // the value of voltage when gyro is zero
static float gyroSensitivity = 0.007;  // gyro sensitivity unit is (v/degree/second) get from datasheet
static float rotationThreshold = 1.5;  // because of gyro drifting, defining rotation angular velocity  less than                                                        // this value will not be ignored
static float gyroRate = 0;             // read out value of sensor in voltage
static float currentAngle = 0;         // current angle calculated by angular velocity integral on

//------------------Fuzzy outputs-------------------------
Fuzzy_output ir_1_fuzzy;
Fuzzy_output ir_2_fuzzy;
Fuzzy_output ir_3_fuzzy;
Fuzzy_output ultrasonic_fuzzy;
Fuzzy_output PT_left_fuzzy;
Fuzzy_output PT_mid_fuzzy;
Fuzzy_output PT_right_fuzzy;
Fuzzy_output PT_top_fuzzy;
//-----------------------------------------------------

//-----------------Default motor control pins--------------
const byte left_front = 46;
const byte left_rear = 47;
const byte right_rear = 50;
const byte right_front = 51;
//---------------------------------------------------------------------------------------------------------

//---------------Fan pins----------
boolean toggle = false;
int FAN_PIN = 4;
//------------------------------------

//-----------Ultrasonic pins--------------------------------------------------------
const int TRIG_PIN = 48;
const int ECHO_PIN = 49;

// Anything over 400 cm (23200 us pulse) is "out of range".
// Hint:If you decrease to this the ranging sensor but the timeout is short, you may not need to read up to 4meters.
const unsigned int MAX_DIST = 23200;
//--------------------------------------------------------------------------------------------------------------

//-----------------------------------PID objects------------------
PID PID_mid(0.3f, 0.0f, 0.0f, -500, 500);


//---------------------------------------------- SENSOR OBJECTS -------------------------------------------------------------
Infrared IR_1(A4, 25325, -1.048); //Infrared(pin,A,beta)
Infrared IR_2(A6, 25610, -1.032);
Infrared IR_3(A7, 2000000, -1.795);
Phototransistor PT_Mid(A14, 79.992, 156.79); //Phototransistor(pin,A,B)
Phototransistor PT_Left(A13, 79.992, 156.79);
Phototransistor PT_Right(A15, 79.992, 156.79);
Phototransistor PT_Top(A12, 79.992, 156.79);
Ultrasonic Ultrasonic(ECHO_PIN, TRIG_PIN);
//-----------------------------------------------------------------------------------------

//----------------------Servo Objects---------------------------------------------------------------------------
Servo left_font_motor;  // create servo object to control Vex Motor Controller 29
Servo left_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_rear_motor;  // create servo object to control Vex Motor Controller 29
Servo right_font_motor;  // create servo object to control Vex Motor Controller 29
//Servo turret_motor;
Turret turret_motor(25);
PID turret_pid(1,0,0
//-----------------------------------------------------------------------------------------------------------

//Serial pointer

#if BLUETOOTH
SoftwareSerial hc06(2, 3);
SoftwareSerial *SerialCom;
#else
HardwareSerial *SerialCom;
#endif

int pos = 0;
void setup(void)
{
  
  turret_motor.Attach(11);
  pinMode(LED_BUILTIN, OUTPUT);

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  
  //Trigger pin for fan
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
#if BLUETOOTH
  SerialCom = &hc06;
#else
  SerialCom = &Serial;
#endif
  SerialCom->begin(115200);
  SerialCom->println("Setup....");
  delay(1000); //settling time
}

void loop(void)
{
  static STATE machine_state = INITIALISING;

  switch (machine_state) {
    case INITIALISING:
      machine_state = initialising();
      break;
    case RUNNING: //includes Lipo Battery check
      machine_state =  running();
      break;
    case STOPPED: //Stop of Lipo Battery voltage is too low, to protect Battery
      machine_state =  stopped();
      break;
  };
}


//--------------- MACHINE STATES------------------------------

STATE initialising() {
  //initialising
  SerialCom->println("INITIALISING....");
  SerialCom->println("Enabling Motors...");
  enable_motors();
//  gyro_setup();
  turret_motor.Write(80);
  return RUNNING;
}


STATE running() {

  static unsigned long previous_millis_1;
  static unsigned long previous_millis_2;

  fast_flash_double_LED_builtin();

  //main loop
  if (millis() - previous_millis_1 > SAMPLING_TIME) {
    previous_millis_1 = millis();
    fuzzify_ir_1();
    fuzzify_ir_2();
    fuzzify_ir_3();
    fuzzify_ultrasonic();
    fuzzify_pt_mid();
    fuzzify_pt_left();
    fuzzify_pt_right();
    fuzzify_pt_top();
    run_inference();
    fan_control();
    turret_motor.Track(PT_mid_fuzzy.set,PT_right_fuzzy.set,PT_left_fuzzy.set);
    Serial.print(PT_left_fuzzy.set);
    Serial.print(", ");
    Serial.println(PT_right_fuzzy.set);

//    turret_motor.Write(80);
  }

  //debug loop
  if (millis() - previous_millis_2 > 500) {
    previous_millis_2 = millis();
#if DISP_READINGS
    SerialCom->print("ir_1_fuzzy = ");
    SerialCom->print(ir_1_fuzzy.set + ": ");
    SerialCom->println(ir_1_fuzzy.value);
    SerialCom->print("ir_2_fuzzy = ");
    SerialCom->print(ir_2_fuzzy.set + ": ");
    SerialCom->println(ir_2_fuzzy.value);
    SerialCom->print("ir_3_fuzzy = ");
    SerialCom->print(ir_3_fuzzy.set + ": ");
    SerialCom->println(ir_3_fuzzy.value);
    SerialCom->print("ultrasonic_fuzzy = ");
    SerialCom->print(ultrasonic_fuzzy.set + ": ");
    SerialCom->println(ultrasonic_fuzzy.value);
    SerialCom->println();
    SerialCom->print("mid_fuzzy = ");
    SerialCom->print(PT_mid_fuzzy.set + ": ");
    SerialCom->println(PT_mid_fuzzy.value);
    SerialCom->println();
    SerialCom->print("left_fuzzy = ");
    SerialCom->print(PT_left_fuzzy.set + ": ");
    SerialCom->println(PT_left_fuzzy.value);
    SerialCom->println();
    SerialCom->print("right_fuzzy = ");
    SerialCom->print(PT_right_fuzzy.set + ": ");
    SerialCom->println(PT_right_fuzzy.value);
    SerialCom->println();
    SerialCom->print("top_fuzzy = ");
    SerialCom->print(PT_top_fuzzy.set + ": ");
    SerialCom->println(PT_top_fuzzy.value);
    SerialCom->println();
#endif

#ifndef NO_BATTERY_V_OK
    if (!is_battery_voltage_OK()) return STOPPED;
#endif


  }

  return RUNNING;
}


//Stop of Lipo Battery voltage is too low, to protect Battery
STATE stopped() {
  static byte counter_lipo_voltage_ok;
  static unsigned long previous_millis;
  int Lipo_level_cal;
  count = 0;
  disable_motors();
  slow_flash_LED_builtin();

  if (millis() - previous_millis > SAMPLING_TIME) { //print massage every 500ms
    previous_millis = millis();
    SerialCom->println("STOPPED---------");

    //    int mid = PID_mid.PID_update(20, PT_Mid.get_raw_reading());
    //    int left_error = PT_Left.get_raw_reading() - 900;
    //    int right_error = PT_Right.get_raw_reading() - 800;
    SerialCom->print("left = ");
    SerialCom->println(PT_Left.get_raw_reading());
    SerialCom->print("right = ");
    SerialCom->println(PT_Right.get_raw_reading());
    SerialCom->print("mid = ");
    SerialCom->println(PT_Mid.get_raw_reading());
    SerialCom->print("top = ");
    SerialCom->println(PT_Top.get_raw_reading());
   
    SerialCom->print("ir_3_fuzzy = ");
    SerialCom->print(IR_3.get_dist());


    //      SerialCom->println(PT_Mid.get_raw_reading() + PT_Left.get_raw_reading()+PT_Right.get_raw_reading());
    


#ifndef NO_BATTERY_V_OK
    //500ms timed if statement to check lipo and output speed settings
    if (is_battery_voltage_OK()) {
      SerialCom->print("Lipo OK waiting of voltage Counter 10 < ");
      SerialCom->println(counter_lipo_voltage_ok);
      counter_lipo_voltage_ok++;
      if (counter_lipo_voltage_ok > 10) { //Making sure lipo voltage is stable
        counter_lipo_voltage_ok = 0;
        enable_motors();
        SerialCom->println("Lipo OK returning to RUN STATE");
        return RUNNING;
      }
    } else
    {
      counter_lipo_voltage_ok = 0;
    }
#endif
  }
  return STOPPED;
}
