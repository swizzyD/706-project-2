class Sensor {
  public:
    Sensor(int pin) {
      this->analogPin = pin;
    }
    //    void set_last_val(int v) {
    //      this->last_val = v;
    //    }
    int get_raw_reading() {
      return analogRead(this->analogPin);
    }

  protected:
    int last_val;
    //int current_reading;
    int analogPin;
};

class Kalman {
  private:
    double a_priori_est, a_post_est, a_priori_var, a_post_var, kalman_gain;

  protected:
    double process_noise;
    double sensor_noise;

    double last_est;

  public:
    Kalman(double process_noise, double sensor_noise) {
      this->process_noise = process_noise;
      this->sensor_noise = sensor_noise;
    }

    double filter(double rawdata) {
      this->a_priori_est = this->last_est;
      this->a_priori_var = process_noise;

      this->kalman_gain = a_priori_var / (a_priori_var + sensor_noise);
      this->a_post_est = a_priori_est + kalman_gain * (rawdata - a_priori_est);
      this->a_post_var = (1 - kalman_gain) * a_priori_var;
      return a_post_est;
    }

    void set_last_est(double prev_est) {
      this->last_est = prev_est;
    }
};

class Infrared: public Sensor, public Kalman {
  public:
    Infrared(int pin, double A, double beta, double process_noise, double sensor_noise) : Kalman(process_noise, sensor_noise), Sensor(pin) {
      this->process_noise = process_noise;
      this->sensor_noise = sensor_noise;
      this->A = A;
      this->beta = beta;
    }
    double get_dist() {
      // Returns Distance of Infrared in mm from short IR sensor GP2Y0A41SK0F
      // short range IR sensor trendline dist = A * val^beta
      double dist, est;
      dist = this->A * pow(this->get_raw_reading(), this->beta);
      est = this->filter(dist);
      this->set_last_est(est);
      return est;
    }
  private:
    double A;
    double beta;
};

class Phototransistor: public Sensor, public Kalman {
  public:
    Phototransistor(int pin, double A, double B, double process_noise, double sensor_noise): Kalman(process_noise, sensor_noise), Sensor(pin) {
      this-> A = A;
      this-> B = B;
      this-> process_noise = process_noise;
      this-> sensor_noise = sensor_noise;
    }
    
    double get_dist() {
      // Converts Phototransistor readings into distance (mm)
      // Looks like an logarithmic relationship: dist = A*ln(raw) - B
      double dist =  this->A * log(this->get_raw_reading() - this->B);
      double est = this->filter(dist);
      this->set_last_est(est);
      return est;
    }

  private:
    double A, B;

};

class Ultrasonic {
  public:
    Ultrasonic(int e_pin, int t_pin) {
      this->echo_pin = e_pin;
      this->trig_pin = t_pin;
    }
    int get_dist() {
      long duration, mm;

      // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
      // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
      // Clearing the trig pin
      digitalWrite(trig_pin, LOW);
      delayMicroseconds(2);
      // pulsing high for at least 10 microseconds
      digitalWrite(trig_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig_pin, LOW);


      // Read the signal from the sensor: a HIGH pulse whose
      // duration is the time (in microseconds) from the sending
      // of the ping to the reception of its echo off of an object.
      pinMode(echo_pin, INPUT);
      duration = pulseIn(echo_pin, HIGH);

      // Convert the time into a distance
      mm = (duration / 2.0) / 2.91;
      return mm;
    }

  private:
    int echo_pin, trig_pin;

};
