#include "RS485_Wind_Transmitter.h"

void RS485_Wind_Transmitter::addedCRC(unsigned char *buf, int len)
{
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (unsigned int)buf[pos];
    for (int i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  buf[len] = crc % 0x100;
  buf[len + 1] = crc / 0x100;
}

unsigned int RS485_Wind_Transmitter::CRC16_2(unsigned char *buf, int len)
{
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (unsigned int)buf[pos];
    for (int i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }

  crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
  return crc;
}

float RS485_Wind_Transmitter::readWindSpeed(unsigned char Address)
{
  unsigned char Data[7] = {0};
  unsigned char COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
  char ret = 0;
  float WindSpeed = 0;
  long curr = millis();
  long curr1 = curr;
  char ch = 0;

  COM[0] = Address;

  addedCRC(COM , 6);

  ser->write(COM, 8);

  while (!ret) {

    if (millis() - curr > 1000) {
      WindSpeed = -1;
      break;
    }

    if (millis() - curr1 > 100) {
      ser->write(COM, 11);
      curr1 = millis();
    }

    if (ser->available()) {
      delay(10);
      ch = ser->read();
      if (ch == Address) {
        Data[0] = ch;
        if (ser->available()) {
          ch = ser->read();
          if (ch == 0x03) {
            Data[1] = ch;
            if (ser->available()) {
              ch = ser->read();
              if (ch == 0x02) {
                Data[2] = ch;
                if (ser->available() >= 4) {
                  Data[3] = ser->read();
                  Data[4] = ser->read();
                  Data[5] = ser->read();
                  Data[6] = ser->read();
                  if (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {
                    ret = 1;
                    WindSpeed = (Data[3] * 256 + Data[4]) / 10.00;
                  }
                }
              }
            }
          }
        }
      }
    }
    return WindSpeed;
  }
}

int RS485_Wind_Transmitter::readWindDirection(unsigned char Address)
{
  unsigned char Data[7] = {0};
  unsigned char COM[8] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
  char ret = 0;
  int WindDirection = 0;
  long curr = millis();
  long curr1 = curr;
  char ch = 0;
  COM[0] = Address;
  addedCRC(COM , 6);
  ser->write(COM, 8);
  while (!ret) {

    if (millis() - curr > 1000) {
      WindDirection = -1;
      break;
    }

    if (millis() - curr1 > 100) {
      ser->write(COM, 11);
      curr1 = millis();
    }

    if (ser->available()) {
      delay(10);
      ch = ser->read();
      if (ch == Address) {
        Data[0] = ch;
        if (ser->available()) {
          ch = ser->read();
          if (ch == 0x03) {
            Data[1] = ch;
            if (ser->available()) {
              ch = ser->read();
              if (ch == 0x02) {
                Data[2] = ch;
                if (ser->available() >= 4) {
                  Data[3] = ser->read();
                  Data[4] = ser->read();
                  Data[5] = ser->read();
                  Data[6] = ser->read();
                  if (CRC16_2(Data, 5) == (Data[5] * 256 + Data[6])) {
                    ret = 1;
                    WindDirection = Data[3] * 256 + Data[4];
                  }
                }
              }
            }
          }
        }
      }
    }

    return WindDirection;
  }
}

unsigned char RS485_Wind_Transmitter::ModifyAddress(unsigned char Address1, unsigned char Address2)
{
  unsigned char ModifyAddressCOM[11] = {0x00, 0x10, 0x10, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00};
  char ret = 0;
  long curr = millis();
  long curr1 = curr;
  char ch = 0;

  ModifyAddressCOM[0] = Address1;
  ModifyAddressCOM[8] = Address2;

  addedCRC(ModifyAddressCOM , 9);

  ser->write(ModifyAddressCOM, 11);
  while (!ret) {
    if (millis() - curr > 1000) {
      break;
    }

    if (millis() - curr1 > 100) {
      ser->write(ModifyAddressCOM, 11);
      curr1 = millis();
    }

    if (ser->available()) {
      delay(7);
      ch = ser->read();
      if (ser->available()) {
        ch = ser->read();
        if (ch == Address1) {
          if (ser->available()) {
            ch = ser->read();
            if (ch == 0x10 ) {
              if (ser->available()) {
                ch = ser->read();
                if (ch == 0x10) {
                  if (ser->available()) {
                    ch = ser->read();
                    if (ch == 0x00) {
                      if (ser->available()) {
                        ch = ser->read();
                        if (ch == 0x00) {
                          if (ser->available()) {
                            ch = ser->read();
                            if (ch == 0x01) {
                              ret = 1 ;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return ret;
}
