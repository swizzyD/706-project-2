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

class Infrared: public Sensor{
  public:
    Infrared(int pin, double A, double beta) : Sensor(pin) {
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

class Phototransistor: public Sensor {
  public:
    Phototransistor(int pin, double A, double B): Sensor(pin) {
      this-> A = A;
      this-> B = B;
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
