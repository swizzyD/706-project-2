int get_ultrasonic_distance()
{
  long duration, mm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  // Clearing the trig pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // pulsing high for at least 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);


  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);

  // Convert the time into a distance
  mm = (duration / 2.0) / 2.91;
  
  return mm;
}

double get_ir_1()
{
  // short range IR sensor trendline dist = 15350 * val^-0.947
  double A = 25325,   beta = -1.048;
  double dist;
  dist = A * pow(IR_1_READING, beta);
  return dist;
}

double get_ir_2()
{
  // short range IR sensor trendline dist = 18126 * val^-0.963
  double A = 25610,   beta = -1.032;
  double dist;
  dist = A * pow(IR_2_READING, beta);
  return dist;
}

// long range
double get_ir_3()
{
  // short range IR sensor trendline dist = 18126 * val^-0.963
  double A = 25610,   beta = -1.032;
  double dist;
  dist = A * pow(IR_3_READING, beta);
  return dist;
}

void ir_reading()
{
  SerialCom->print("IR_1_reading:");
  SerialCom->println(IR_1_READING);
  SerialCom->print("IR_2_reading:");
  SerialCom->println(IR_2_READING);
  SerialCom->print("IR_3_reading:");
  SerialCom->println(IR_3_READING);
}

void ultrasonic_reading()
{
  SerialCom->print("ultrasonic reading:");
  SerialCom->println(get_ultrasonic_distance());
}

void gyro_reading()
{
  SerialCom->print("gyro reading:");
  SerialCom->println(GYRO_READING);
}

void gyro_setup() {
  int i;
  float sum = 0;
  SerialCom->println("gyro setup");
  for (i = 0; i < 100; i++)     //  read 100 values of voltage when gyro is at still, to calculate the zero-drift
  {
    sum += GYRO_READING;
  }
  delay(5);

  gyroZeroVoltage = sum / 100;  // average the sum as the zero drifting
  SerialCom->print("Zero Voltage: ");
  SerialCom->println(gyroZeroVoltage);
}


void update_angle()
{

  // convert the 0-1023 signal to 0-5v
  gyroRate = (GYRO_READING * gyroSupplyVoltage) / 1023;
  // find the voltage offset the value of voltage when gyro is zero (still)
  gyroRate -= (gyroZeroVoltage / 1023 * 5);

  // read out voltage divided the gyro sensitivity to calculate the angular velocity
  float angularVelocity = gyroRate / gyroSensitivity;
  // if the angular velocity is less than the threshold, ignore it
  if (abs(angularVelocity) >= rotationThreshold) { // we are running a loop in T. one second will run (1000/T).

    float angleChange = angularVelocity / (1000 / SAMPLING_TIME);
    currentAngle -= angleChange;
  }

  // keep the angle between 0-360
  if (currentAngle < 0)    {
    currentAngle += 360;
  }  else if (currentAngle > 359) {
    currentAngle -= 360;
  }

  SerialCom->print("Angular Velocity: ");
  SerialCom->print(angularVelocity);
  SerialCom->print(" Current Angle: ");
  SerialCom->println(currentAngle);

}
