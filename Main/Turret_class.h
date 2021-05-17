//class Turret
//{
//  Servo servo;              // the servo
//  int pos;              // current servo position 
//  int increment;        // increment to move for each interval
//  int  updateInterval;      // interval between updates
//  unsigned long lastUpdate; // last update of position
// 
//public: 
//  Turret(int interval)
//  {
//    updateInterval = interval;
//    increment = 1;
//  }
//  
//  void Attach(int pin)
//  {
//    servo.attach(pin);
//  }
//  
//  void Detach()
//  {
//    servo.detach();
//  }
//
//  void Write(int val)
//  {
//    servo.write(val);
//  }
//  void Update()
//  {
//    if((millis() - lastUpdate) > updateInterval)  // time to update
//    {
//      lastUpdate = millis();
//      pos += increment;
//      servo.write(pos);
//      Serial.println(pos);
//      if ((pos >= 180) || (pos <= 0)) // end of sweep
//      {
//        // reverse direction
//        increment = -increment;
//      }
//    }
//  }
//  
//  void Track(String mid, double right_val, double left_val)
//  {
//    if (mid == "light"){
//      servo.write(pos);
//    }
//    if (right_val > left_val){
//      servo.write(pos+2);
//    }
//
//    else if (right_val < left_val){
//      servo.write(pos-2);
//    }
//
//  }
//  
//};
