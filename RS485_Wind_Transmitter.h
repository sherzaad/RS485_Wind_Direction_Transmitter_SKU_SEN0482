#ifndef RS485_Wind_Transmitter_h_
#define RS485_Wind_Transmitter_h_

#include "Arduino.h"

class RS485_Wind_Transmitter
{
  public:
    RS485_Wind_Transmitter() {}
    void addedCRC(unsigned char *buf, int len);
    unsigned int CRC16_2(unsigned char *buf, int len);
    float readWindSpeed(unsigned char Address);
    int readWindDirection(unsigned char Address);
    unsigned char ModifyAddress(unsigned char Address1, unsigned char Address2);
    ~RS485_Wind_Transmitter() {}

    inline void Init(HardwareSerial *s) {
      ser = s;
      ser->begin(9600);
    }

  private:
    HardwareSerial *ser;

};

#endif

