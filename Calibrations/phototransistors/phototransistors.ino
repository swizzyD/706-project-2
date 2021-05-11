  float last_est = 0;
  float process_noise = 1;
  float sensor_noise = 10;    // Change the value of sensor noise to get different KF performance
  #include <math.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val;  
  float temp, est;              
  val=analogRead(A15);      //Read the analog port 0 and store the value in val
  temp = PTDist(val);
  Serial.print(val);
  est = Kalman(temp, last_est);
  Serial.print(" , ");
  Serial.println(est);
  last_est = est;
  
}

float Kalman(float rawdata, float prev_est){   // Kalman Filter
  float a_priori_est, a_post_est, a_priori_var, a_post_var, kalman_gain;

  a_priori_est = prev_est;  
  a_priori_var = process_noise; 

  kalman_gain = a_priori_var/(a_priori_var+sensor_noise);
  a_post_est = a_priori_est + kalman_gain*(rawdata-a_priori_est);
  a_post_var = (1- kalman_gain)*a_priori_var;
  return a_post_est;
}

double PTDist(int raw){
  // Converts Phototransistor readings into distance (mm)
  // Looks like an logarithmic relationship: dist = A*ln(raw) - B
  double A = 79.992;
  double B = 156.79;
  return A*log(raw) - B;
}
