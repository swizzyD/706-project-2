

//Serial Pointer
HardwareSerial *SerialCom;
int val;

void setup(void)
{

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial;
  SerialCom->begin(115200);
}

void loop() {
  SerialCom->println("FUCK");
  if (SerialCom->available()){
      int val = SerialCom->read();
      SerialCom->println("recieved");
  }
  }

  
