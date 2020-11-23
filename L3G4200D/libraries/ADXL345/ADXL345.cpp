#include "Arduino.h"
#include "ADXL345.h"

#include <math.h>

ADXL345::ADXL345()
{  
  xg =0;
  yg=0;
  zg=0;
}

void ADXL345::init(char x_offset, char y_offset, char z_offset)
{
  writeTo(ADXL345_POWER_CTL, 8); 
  
  writeTo(ADXL345_DATA_FORMAT, 0x0B);
  
  writeTo(ADXL345_OFSX, x_offset);
  writeTo(ADXL345_OFSY, y_offset);
  writeTo(ADXL345_OFSZ, z_offset);  
}

void ADXL345::setSoftwareOffset(double x, double y, double z)
{
  _xoffset = x;
  _yoffset = y;
  _zoffset = z; 
}

AccelRotation ADXL345::readPitchRoll()
{
  //http://developer.nokia.com/community/wiki/How_to_get_pitch_and_roll_from_accelerometer_data_on_Windows_Phone
  //http://www.hobbytronics.co.uk/accelerometer-info
  
  AccelG accel;
  accel = readAccelG();
  
  AccelRotation rot;
  
  rot.pitch = (atan2(accel.x,sqrt(accel.y*accel.y+accel.z*accel.z)) * 180.0) / PI;
  rot.roll = (atan2(accel.y,(sqrt(accel.x*accel.x+accel.z*accel.z))) * 180.0) / PI;
  
  return rot;
}


void ADXL345::printCalibrationValues(int samples)
{
  double x,y,z;
  double xt,yt,zt;
  xt = 0;
  yt = 0;
  zt = 0;
  
  Serial.print("Calibration in: 3");
  delay(1000);
  Serial.print(" 2");
  delay(1000);  
  Serial.println(" 1");  
  delay(1000);  
  
  for(int i=0; i<samples; i++)
  {
    AccelG accel = readAccelG();
    xt += accel.x;
    yt += accel.y;
    zt += accel.z;
    delay(100);
  }
  
  Serial.println("Accel Offset (mg): ");
  Serial.print("X: ");
  Serial.print(xt/float(samples)*1000,5);
  Serial.print(" Y: ");
  Serial.print(yt/float(samples)*1000,5);
  Serial.print(" Z: ");
  Serial.println( zt/float(samples)*1000,5);
  
  delay(2000);
}


// Writes val to address register on device
void ADXL345::writeTo(byte address, byte val) 
{
  Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
  Wire.write(address); // send register address
  Wire.write(val); // send value to write
  Wire.endTransmission(); // end transmission
}

AccelG ADXL345::readAccelG()
{
  AccelRaw raw;
  raw = readAccel();
  
  //Scale = (16*2)/2^13

  double fXg, fYg, fZg;
  fXg =raw.x * 0.00390625 + _xoffset;
  fYg =raw.y * 0.00390625 + _yoffset;
  fZg =raw.z * 0.00390625 + _zoffset;  
  
  AccelG res;
  
  res.x = fXg * ALPHA + (xg * (1.0-ALPHA));
  xg = res.x;
  
  res.y = fYg * ALPHA + (yg * (1.0-ALPHA));
  yg = res.y;
  
  res.z = fZg * ALPHA + (zg * (1.0-ALPHA));
  zg = res.z;
  
  return res;
  
  
}

AccelRaw ADXL345::readAccel() 
{
  readFrom(ADXL345_DATAX0, ADXL345_TO_READ, _buff); //read the acceleration data from the ADXL345

  // each axis reading comes in 16 bit resolution, ie 2 bytes. Least Significat Byte first!!
  // thus we are converting both bytes in to one int
  AccelRaw raw;
  raw.x = (((int)_buff[1]) << 8) | _buff[0];
  raw.y = (((int)_buff[3]) << 8) | _buff[2];
  raw.z = (((int)_buff[5]) << 8) | _buff[4];
  
  return raw;
}

// Reads num bytes starting from address register on device in to _buff array
void ADXL345::readFrom(byte address, int num, byte _buff[]) 
{
  Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
  Wire.write(address); // sends address to read from
  Wire.endTransmission(); // end transmission

  Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
  Wire.requestFrom(ADXL345_DEVICE, num); // request 6 bytes from device Registers: DATAX0, DATAX1, DATAY0, DATAY1, DATAZ0, DATAZ1
  
  int i = 0;
  while(Wire.available()) // device may send less than requested (abnormal)
  {
    _buff[i] = Wire.read(); // receive a byte
    i++;
  }

  Wire.endTransmission(); // end transmission
}

void ADXL345::printAllRegister() 
{
	byte _b;
	Serial.print("0x00: ");
	readFrom(0x00, 1, &_b);
	print_byte(_b);
	Serial.println("");
	int i;
	for (i=29;i<=57;i++)
        {
		Serial.print("0x");
		Serial.print(i, HEX);
		Serial.print(": ");
		readFrom(i, 1, &_b);
		print_byte(_b);
		Serial.println("");    
	}
}

void ADXL345::print_byte(byte val)
{
	int i;
	Serial.print("B");
	for(i=7; i>=0; i--){
		Serial.print(val >> i & 1, BIN);
	}
}
