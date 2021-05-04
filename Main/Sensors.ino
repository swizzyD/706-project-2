int get_ultrasonic_range()
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
  // Returns distances in cm from short IR sensor GP2Y0A41SK0F
  // 5V
  // Adapted from https://www.smart-prototyping.com/blog/Sharp-Distance-Measuring-Sensor-GP2Y0A41SK0F-Tutorial
  double distance;
  double volts = IR_1_READING * 0.0048828125; // value from sensor * (5/1024)

  distance = 13 * pow(volts, -1); //side 1 distance in cm in d[0]

  return distance;
}

double get_ir_2()
{
  // Returns distances in cm from short IR sensor GP2Y0A41SK0F
  // 5V
  // Adapted from https://www.smart-prototyping.com/blog/Sharp-Distance-Measuring-Sensor-GP2Y0A41SK0F-Tutorial
  double distance;
  double volts = IR_2_READING * 0.0048828125; // value from sensor * (5/1024)

  distance = 13 * pow(volts, -1); //side 1 distance in cm in d[0]

  return distance;
}

void side_reading()
{
  SerialCom->print("IR_1_reading:");
  SerialCom->println(IR_1_READING);
  SerialCom->print("IR_2_reading:");
  SerialCom->println(IR_2_READING);
}

void ultrasonic_reading()
{
  SerialCom->print("ultrasonic reading:");
  SerialCom->println(get_ultrasonic_range());
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
