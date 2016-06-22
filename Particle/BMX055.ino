// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMX055
// This code is designed to work with the BMX055_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <application.h>
#include <spark_wiring_i2c.h>

// BMX055 Accl I2C address is 0x18(24)
#define Addr_Accl 0x18
// BMX055 Gyro I2C address is 0x68(104)
#define Addr_Gyro 0x68
// BMX055 Mag I2C address is 0x10(16)
#define Addr_Mag 0x10

int xAccl = 0, yAccl =  0, zAccl = 0, xGyro = 0, yGyro = 0, zGyro = 0, xMag = 0, yMag = 0, zMag = 0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "BMX055");
  Particle.variable("xAccl", xAccl);
  Particle.variable("yAccl", yAccl);
  Particle.variable("zAccl", zAccl);
  Particle.variable("xGyro", xGyro);
  Particle.variable("yGyro", yGyro);
  Particle.variable("zGyro", zGyro);
  Particle.variable("xMag", xMag);
  Particle.variable("yMag", yMag);
  Particle.variable("zMag", zMag);

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Accl);
  // Select PMU_Range register
  Wire.write(0x0F);
  // Range = +/- 2g
  Wire.write(0x03);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Accl);
  // Select PMU_BW register
  Wire.write(0x10);
  // Bandwidth = 7.81 Hz
  Wire.write(0x08);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Accl);
  // Select PMU_LPW register
  Wire.write(0x11);
  // Normal mode, Sleep duration = 0.5ms
  Wire.write(0x00);
  // Stop I2C Transmission on the device
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Gyro);
  // Select Range register
  Wire.write(0x0F);
  // Full scale = +/- 125 degree/s
  Wire.write(0x04);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Gyro);
  // Select Bandwidth register
  Wire.write(0x10);
  // ODR = 100 Hz
  Wire.write(0x07);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Gyro);
  // Select LPM1 register
  Wire.write(0x11);
  // Normal mode, Sleep duration = 2ms
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select Mag register
  Wire.write(0x4B);
  // Soft reset
  Wire.write(0x83);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select Mag register
  Wire.write(0x4C);
  // Normal Mode, ODR = 10 Hz
  Wire.write(0x00);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select Mag register
  Wire.write(0x4E);
  // X, Y, Z-Axis enabled
  Wire.write(0x84);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select Mag register
  Wire.write(0x51);
  // No. of Repetitions for X-Y Axis = 9
  Wire.write(0x04);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr_Mag);
  // Select Mag register
  Wire.write(0x52);
  // No. of Repetitions for Z-Axis = 15
  Wire.write(0x0F);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[6];

  for (int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr_Accl);
    // Select data register
    Wire.write((2 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr_Accl, 1);

    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }

  // Convert the data to 12-bits
  int xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)
  {
    xAccl -= 4096;
  }
  int yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)
  {
    yAccl -= 4096;
  }
  int zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16;
  if (zAccl > 2047)
  {
    zAccl -= 4096;
  }

  for (int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr_Gyro);
    // Select data register
    Wire.write((2 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr_Gyro, 1);

    // Read 6 bytes of data
    // xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }

  // Convert the data
  int xGyro = (data[1] * 256) + data[0];
  if (xGyro > 32767)
  {
    xGyro -= 65536;
  }
  int yGyro = (data[3] * 256) + data[2];
  if (yGyro > 32767)
  {
    yGyro -= 65536;
  }
  int zGyro = (data[5] * 256) + data[4];
  if (zGyro > 32767)
  {
    zGyro -= 65536;
  }

  for (int i = 0; i < 6; i++)
  {
    // Start I2C Transmission
    Wire.beginTransmission(Addr_Mag);
    // Select data register
    Wire.write((66 + i));
    // Stop I2C Transmission
    Wire.endTransmission();

    // Request 1 byte of data
    Wire.requestFrom(Addr_Mag, 1);

    // Read 6 bytes of data
    // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
    if (Wire.available() == 1)
    {
      data[i] = Wire.read();
    }
  }

  // Convert the data
  int xMag = ((data[1] * 256) + (data[0] & 0xF8)) / 8;
  if (xMag > 4095)
  {
    xMag -= 8192;
  }
  int yMag = ((data[3] * 256) + (data[2] & 0xF8)) / 8;
  if (yMag > 4095)
  {
    yMag -= 8192;
  }
  int zMag = ((data[5] * 256) + (data[4] & 0xFE)) / 2;
  if (zMag > 16383)
  {
    zMag -= 32768;
  }

  // Output data to dashboard
  Particle.publish("Acceleration in X-Axis : ", String(xAccl));
  Particle.publish("Acceleration in Y-Axis : ", String(yAccl));
  Particle.publish("Acceleration in Z-Axis : ", String(zAccl));
  delay(1000);
  Particle.publish("X-Axis of rotation : ", String(xGyro));
  Particle.publish("Y-Axis of rotation : ", String(yGyro));
  Particle.publish("Z-Axis of rotation : ", String(zGyro));
  delay(1000);
  Particle.publish("Magnetic field in X-Axis : ", String(xMag));
  Particle.publish("Magnetic field in Y-Axis : ", String(yMag));
  Particle.publish("Magnetic filed in Z-Axis : ", String(zMag));
  delay(1000);
}
