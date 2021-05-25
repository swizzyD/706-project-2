class Turret
{
    Servo servo;              // the servo
    int pos;              // current servo position
    int increment;        // increment to move for each interval
    int  updateInterval;      // interval between updates
    unsigned long lastUpdate; // last update of position

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
    void Update()
    {
      if ((millis() - lastUpdate) > updateInterval) // time to update
      {
        lastUpdate = millis();
        pos += increment;
        servo.write(pos);
        Serial.println(pos);
        if ((pos >= 180) || (pos <= 0)) // end of sweep
        {
          // reverse direction
          increment = -increment;
        }
      }
    }

    void Track(String mid, String right_val, String left_val)
    {
      if (mid == "light") {
        servo.write(this->pos);
      }
      else if (right_val == "light" && left_val == "clear") {
        servo.write(this->pos - 2);
        this->pos = this->pos - 2;
      }
      else if (left_val == "light" && right_val == "clear") {
        servo.write(this->pos + 2);
        this->pos = this->pos + 2;
      }

      //
      //    else if (right_val < left_val){
      //      servo.write(this->pos-2);
      //      this->pos = this->pos-2;
      //      Serial.print(right_val);
      //      Serial.print(", ");
      //      Serial.println(left_val);
      //    }

    }

};
