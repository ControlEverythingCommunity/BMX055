// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMX055
// This code is designed to work with the BMX055_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, BMX055 I2C address for Accl is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x18);

	// Select PMU_Range register(0x0F)
	// Range = +/- 2g(0x03)
	char config[2] = {0};
	config[0] = 0x0F;
	config[1] = 0x03;
	write(file, config, 2);
	// Select PMU_BW register(0x10)
	// Bandwidth = 7.81 Hz(0x08)
	config[0] = 0x10;
	config[1] = 0x08;
	write(file, config, 2);
	// Select PMU_LPW register(0x11)
	// Normal mode, Sleep duration = 0.5ms(0x00)
	config[0] = 0x11;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from register(0x02)
	// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
	char reg[6] = {0};
	reg[0] = 0x02;
	write(file, reg, 1);
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Error : Input/output Error \n");
		exit(1);
	}

	// Convert the data
	int xAccl = (data[1] * 256 + (data[0] & 0xF0)) / 16;
	if(xAccl > 2047)
	{
		xAccl -= 4096;
	}

	int yAccl = (data[3] * 256 + (data[2] & 0xF0)) / 16;
	if(yAccl > 2047)
	{
		yAccl -= 4096;
	}

	int zAccl = (data[5] * 256 + (data[4] & 0xF0)) / 16;
	if(zAccl > 2047)
	{
		zAccl -= 4096;
	}

	// Get I2C device, BMX055 I2C address for Gyro is 0x68(104)
	ioctl(file, I2C_SLAVE, 0x68);

	// Select Range register(0x0F)
	// Full scale = +/- 125 degree/s(0x04)
	config[0] = 0x0F;
	config[1] = 0x04;
	write(file, config, 2);
	// Select Bandwidth register(0x10)
	// ODR = 100 Hz(0x07)
	config[0] = 0x10;
	config[1] = 0x07;
	write(file, config, 2);
	// Select LPM1 register(0x11)
	// Normal mode, Sleep duration = 2ms(0x00)
	config[0] = 0x11;
	config[1] = 0x00;
	write(file, config, 2);
	sleep(1);
	
	// Read 6 bytes of data from register(0x02)
	// xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
	reg[0] = 0x02;
	write(file, reg, 1);
	read(file, data, 6);

	// Convert the data
	int xGyro = (data[1] * 256 + data[0]);
	if (xGyro > 32767)
	{
	xGyro -= 65536;
	}

	int yGyro = (data[3] * 256 + data[2]);
	if (yGyro > 32767)
	{
	yGyro -= 65536;
	}

	int zGyro = (data[5] * 256 + data[4]);
	if (zGyro > 32767)
	{
		zGyro -= 65536;
	}

	// Get I2C device, BMX055 I2C address for Mag is 0x10(16)
	ioctl(file, I2C_SLAVE, 0x10);

	// Select Mag register(0x4B)
	// Soft reset(0x83)
	config[0] = 0x4B;
	config[1] = 0x83;
	write(file, config, 2);
	// Select Mag register(0x4C)
	// Normal Mode, ODR = 10 Hz(0x00)
	config[0] = 0x4C;
	config[1] = 0x00;
	write(file, config, 2);
	// Select Mag register(0x4E)
	// X, Y, Z-Axis enabled(0x84)
	config[0] = 0x4E;
	config[1] = 0x84;
	write(file, config, 2);
	// Select Mag register(0x51)
	// No. of Repetitions for X-Y Axis = 9(0x04)
	config[0] = 0x51;
	config[1] = 0x04;
	write(file, config, 2);
	// Select Mag register(0x52)
	// No. of Repetitions for Z-Axis = 15(0x0F)
	config[0] = 0x52;
	config[1] = 0x0F;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from register(0x42)
	// xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
	reg[0] = 0x42;
	write(file, reg, 1);
	read(file, data, 6);

	int xMag = (data[1] * 256 + (data[0] & 0xF8)) / 8;
	if(xMag > 4095)
	{
		xMag -= 8192;
	}
	
	int yMag = (data[3] * 256 + (data[2] & 0xF8)) / 8;
	if(yMag > 4095)
	{
		yMag -= 8192;
	}
	int zMag = (data[5] * 256 + (data[4] & 0xFE)) / 2;
	if(zMag > 16383)
	{
		zMag -= 32768;
	}

	// Output data to screen
	printf("Acceleration in X-axis : %d \n", xAccl);
	printf("Acceleration in Y-axis : %d \n", yAccl);
	printf("Acceleration in Z-axis : %d \n", zAccl);
	printf("X-Axis of Rotation : %d \n", xGyro);
	printf("Y-Axis of Rotation : %d \n", yGyro);
	printf("Z-Axis of Rotation : %d \n", zGyro);
	printf("Magnetic field in X-Axis : %d \n", xMag);
	printf("Magnetic field in Y-Axis : %d \n", yMag);
	printf("Magnetic field in Z-Axis : %d \n", zMag);
}