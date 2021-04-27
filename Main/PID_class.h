

class PID{
  private:
  String sensor;
  float Kp, Ki, Kd;
  int limMin, limMax;
  int integrator, prevError, differentiator, prevMeasurement;
  int Ts;  //sampling time
  int tau; // derivative low pass filter time constant (1/2*pi*Fcutoff)  in ms
  int out;
  
  public: 
  PID(float Kp, float Ki, float Kd, int limMin, int limMax);
  int PID_update(int target, int measurement);
  
};

PID::PID(float Kp, float Ki, float Kd, int limMin, int limMax){
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
  this->Ts = 20;    //sampling time 20ms
  this->tau = 64;  // 10Hz cutoff
  this->limMin = limMin;
  this->limMax = limMax;
  this->integrator = 0;
  this->prevError = 0;
  this->differentiator = 0;
  this->prevMeasurement = 0;
  this->out = 0;
}

int PID::PID_update(int target, int measurement){
  int error;
  if(abs(target - measurement) > 1){
    error = target - measurement;
  }
  else{
    error = 0;
  }
  float proportional = Kp * error;
  integrator += 0.5f * Ki * Ts * (error + prevError);

  //integrator dynamic clamp
  float limMaxIntegrator, limMinIntegrator;

  if(limMax > proportional){
    limMaxIntegrator = limMax - proportional;
  }
  else{
    limMaxIntegrator = 0.0f;
  }

  if(limMin < proportional){
    limMinIntegrator = limMin - proportional;
  }
  else{
    limMinIntegrator = 0.0f;
  }

  //clamp integrator
  if(integrator > limMaxIntegrator){
    integrator = limMaxIntegrator;
  }
  if(integrator < limMinIntegrator){
    integrator = limMinIntegrator;
  }

  differentiator = (2.0f * Kd * (measurement - prevMeasurement) + (2.0f * tau - Ts) * differentiator)
                 / (2.0f * tau + Ts);


  out = proportional + integrator + differentiator;
  
  //clamp output
  if(out > limMax){
    out = limMax;
  }
  if(out < limMin){
    out = limMin;
  }

  prevError = error;
  prevMeasurement = measurement;


  
  return out;

}
