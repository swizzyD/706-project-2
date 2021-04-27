

//Serial Pointer
HardwareSerial *SerialCom;


void setup(void)
{

  // Setup the Serial port and pointer, the pointer allows switching the debug info through the USB port(Serial) or Bluetooth port(Serial1) with ease.
  SerialCom = &Serial;
  SerialCom->begin(115200);
}

void loop() {
  SerialCom->write("FUCKKKK");
  if(SerialCom->available()){
    data = SerialCom->read()
    SerialCom->println(data);
  }

}
