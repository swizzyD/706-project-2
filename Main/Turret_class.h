class Turret
{
    Servo servo;              // the servo
    int pos;              // current servo position
    int increment;        // increment to move for each interval
    int  updateInterval;      // interval between updates
    unsigned long lastUpdate; // last update of position
    int prev_raw = 50;
    int pos_correction = 5;

  public:
    Turret(int interval)
    {
      updateInterval = interval;
      increment = 1;
    }

    void Attach(int pin)
    {
      servo.attach(pin);
    }

    void Detach()
    {
      servo.detach();
    }

    void Write(int val)
    {
      this->pos = val;
      servo.write(val);
    }

    int Track(int raw_mid) {
      int pos_correction = sheep(raw_mid);
      this->pos += pos_correction;
      if(this->pos >180){
        this->pos -= 180;
      }
      else if(this->pos<0){
        this->pos += 180;
      }
      servo.write(this->pos);
      return this->pos;
    }
    int sheep(int raw_mid) {
      
      if (raw_mid > this->prev_raw) {
        this->pos_correction = this->pos_correction*-1;
      }
      this->prev_raw= raw_mid;
      return this->pos_correction;
    }

};
