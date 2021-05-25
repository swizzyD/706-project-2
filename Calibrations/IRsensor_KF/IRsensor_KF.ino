    double last_est1 = 0;
    double last_est2 = 0;
    double process_noise = 1;
    double sensor_noise = 1;    // Change the value of sensor noise to get different KF performance
    
void setup() {
    Serial.begin(115200);   //Set up serial port at 115200 baud rate


}

void loop() {
  int val1, val2;                // Integer variable for analogue input
  double temp1, temp2, est1, est2;       // Variables for raw and fitered distance measurement
  val1=analogRead(A4);      //Read the analog port 0 and store the value in val
  val2=analogRead(A6);
  temp1 = DistanceSide1(val1);  //Convert to mm distance based on sensor calibration
  temp2 = DistanceSide2(val2);


  
//  Serial.print(val);
//  Serial.print(" , ");
//  Serial.println(temp);    //Print the value to the serial port
 // Serial.print(" , ");
  est1 = Kalman(temp1, last_est1);  //Filtered estimated measurement
  est2 = Kalman(temp2, last_est2);  //Filtered estimated measurement
  //Serial.println(est);
  Serial.print(est1);
  Serial.print(" , ");
  Serial.println(est2);
  last_est1 = est1;         // store estimate as previous measurement for next cycle
  last_est2 = est2;
  delay(100);            //Wait one second 

}

double DistanceSide1(int RawADC)   {    // Convert to distance in mm based on calibration
  // --------------------------- SIDE 1 ------------------------------------------------
  // short range IR sensor trendline dist = 15350 * val^-0.947
  double A = 25325,   beta = -1.048;
  double dist;
  dist = A * pow(RawADC, beta);
  return dist;
}

double DistanceSide2(int RawADC)   {    // Convert to distance in mm based on calibration
  // --------------------------- SIDE 2 -------------------------------------------------
  // short range IR sensor trendline dist = 18126 * val^-0.963
  double A = 25610,   beta = -1.032;
  double dist;
  dist = A * pow(RawADC, beta);
  return dist;
}

double Kalman(double rawdata, double prev_est){   // Kalman Filter
  double a_priori_est, a_post_est, a_priori_var, a_post_var, kalman_gain;

  a_priori_est = prev_est;  
  a_priori_var = process_noise; 

  kalman_gain = a_priori_var/(a_priori_var+sensor_noise);
  a_post_est = a_priori_est + kalman_gain*(rawdata-a_priori_est);
  a_post_var = (1- kalman_gain)*a_priori_var;
  return a_post_est;
}
