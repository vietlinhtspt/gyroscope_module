#ifndef ADXL345_h
#define ADXL345_h

#include <Wire.h>
#include "Arduino.h"



#define ADXL345_DEVICE 0x53
#define ADXL345_TO_READ 6

#define ADXL345_POWER_CTL 0x2d
#define ADXL345_DATAX0 0x32
#define ADXL345_DATA_FORMAT 0x31

#define ADXL345_OFSX 0x1E
#define ADXL345_OFSY 0x1F
#define ADXL345_OFSZ 0x20

#define ALPHA 0.5


struct AccelRaw
{
  int x;
  int y;
  int z;
};

struct AccelG
{
  double x;
  double y;
  double z;
};

struct AccelRotation
{
  double pitch;
  double roll;
};

class ADXL345
{
  public:
    ADXL345();
    void init(char x_offset=0, char y_offset=0, char z_offset=0);
    void writeTo(byte address, byte val);
    AccelRaw readAccel();
    AccelG readAccelG();
    void readFrom(byte address, int num, byte _buff[]);
    void printAllRegister();
    void print_byte(byte val);
    void printCalibrationValues(int samples);
    AccelRotation readPitchRoll();
    void setSoftwareOffset(double x, double y, double z);
    
    
  private:
     byte _buff[6];
     
     double xg;
     double yg;
     double zg;
     
     double _xoffset;
     double _yoffset;
     double _zoffset;
     
    
};

#endif
