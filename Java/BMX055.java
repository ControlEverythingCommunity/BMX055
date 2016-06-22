// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// BMX055
// This code is designed to work with the BMX055_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class BMX055
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, BMX055 ACCL I2C address is 0x18(24)
		I2CDevice device_accl = bus.getDevice(0x18);

		// Select PMU_Range register, 0x0F(15)
		// Range = +/- 2g
		device_accl.write(0x0F, (byte)0x03);
		// Select PMU_BW register, 0x10(16)
		// Bandwidth = 7.81 Hz
		device_accl.write(0x10, (byte)0x08);
		// Select PMU_LPW register, 0x11(17)
		// Normal mode, Sleep duration = 0.5ms
		device_accl.write(0x11, (byte)0x00);
		Thread.sleep(300);

		// Read 6 bytes of data
		// xAccl LSB, xAccl MSB, yAccl LSB, yAccl MSB, zAccl LSB, zAccl MSB
		byte[] data = new byte[6];
		data[0] = (byte)device_accl.read(0x02);
		data[1] = (byte)device_accl.read(0x03);
		data[2] = (byte)device_accl.read(0x04);
		data[3] = (byte)device_accl.read(0x05);
		data[4] = (byte)device_accl.read(0x06);
		data[5] = (byte)device_accl.read(0x07);

		// Convert the data
		int xAccl = ((data[1] & 0xFF) * 256 + (data[0] & 0xF0)) / 16;
		if(xAccl > 2047)
		{
			xAccl -= 4096;
		}

		int yAccl = ((data[3] & 0xFF) * 256 + (data[2] & 0xF0)) / 16;
		if(yAccl > 2047)
		{
			yAccl -= 4096;
		}

		int zAccl = ((data[5] & 0xFF) * 256 + (data[4] & 0xF0)) / 16;
		if(zAccl > 2047)
		{
			zAccl -= 4096;
		}

		// Get I2C device, LSM9DSO GYRO I2C address is 0x68(104)
		I2CDevice device_gyro = bus.getDevice(0x68);

		// Select Range register, 0x0F(15)
		// Full scale = +/- 125 degree/s
		device_gyro.write(0x0F, (byte)0x04);
		// Select Bandwidth register, 0x10(16)
		// ODR = 100 Hz
		device_gyro.write(0x10, (byte)0x07);
		// Select LPM1 register, 0x11(17)
		// Normal mode, Sleep duration = 2ms
		device_gyro.write(0x11, (byte)0x00);
		Thread.sleep(300);

		// Read 6 bytes of data
		// xGyro LSB, xGyro MSB, yGyro LSB, yGyro MSB, zGyro LSB, zGyro MSB
		data[0] = (byte)device_gyro.read(0x02);
		data[1] = (byte)device_gyro.read(0x03);
		data[2] = (byte)device_gyro.read(0x04);
		data[3] = (byte)device_gyro.read(0x05);
		data[4] = (byte)device_gyro.read(0x06);
		data[5] = (byte)device_gyro.read(0x07);

		// Convert the data
		int xGyro = ((data[1] & 0xFF) * 256 + (data[0] & 0xFF)) ;
		if(xGyro > 32767)
		{
			xGyro -= 65536;
		}	

		int yGyro = ((data[3] & 0xFF) * 256 + (data[2] & 0xFF)) ;
		if(yGyro > 32767)
		{
			yGyro -= 65536;
		}

		int zGyro = ((data[5] & 0xFF) * 256 + (data[4] & 0xFF)) ;
		if(zGyro > 32767)
		{
			zGyro -= 65536;
		}

		// Get I2C device, LSM9DSO MAG I2C address is 0x10(16)
		I2CDevice device_mag = bus.getDevice(0x10);

		// Select Mag register, 0x4B(75)
		// Soft reset
		device_mag.write(0x4B, (byte)0x83);
		// Select Mag register, 0x4C(76)
		// Normal Mode, ODR = 10 Hz
		device_mag.write(0x4C, (byte)0x00);
		// Select Mag register, 0x4E(78)
		// X, Y, Z-Axis enabled
		device_mag.write(0x4E, (byte)0x84);
		// Select Mag register, 0x51(81)
		// No. of Repetitions for X-Y Axis = 9
		device_mag.write(0x51, (byte)0x04);
		// Select Mag register, 0x52(82)
		// No. of Repetitions for Z-Axis = 15
		device_mag.write(0x52, (byte)0x0F);
		Thread.sleep(300);

		// Read 6 bytes of data
		// xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
		data[0] = (byte)device_mag.read(0x42);
		data[1] = (byte)device_mag.read(0x43);
		data[2] = (byte)device_mag.read(0x44);
		data[3] = (byte)device_mag.read(0x45);
		data[4] = (byte)device_mag.read(0x46);
		data[5] = (byte)device_mag.read(0x47);

		// Convert the data
		int xMag = ((data[1] & 0xFF) * 256 + (data[0] & 0xF8)) / 8;
		if(xMag > 4095)
		{
			xMag -= 8192;
		}	

		int yMag = ((data[3] & 0xFF) * 256 + (data[2] & 0xF8)) / 8;
		if(yMag > 4095)
		{
			yMag -= 8192;
		}

		int zMag = ((data[5] & 0xFF) * 256 + (data[4] & 0xFE)) / 2;
		if(zMag > 16383)
		{
			zMag -= 32768;
		}

		// Output data to screen
		System.out.printf("Acceleration in X-Axis : %d %n", xAccl);
		System.out.printf("Acceleration in Y-Axis : %d %n", yAccl);
		System.out.printf("Acceleration in Z-Axis : %d %n", zAccl);
		System.out.printf("X-axis Of Rotation : %d %n", xGyro);
		System.out.printf("Y-axis Of Rotation : %d %n", yGyro);
		System.out.printf("Z-axis Of Rotation : %d %n", zGyro);
		System.out.printf("Magnetic field in X-Axis : %d %n", xMag);
		System.out.printf("Magnetic field in Y-Axis : %d %n", yMag);
		System.out.printf("Magnetic field in Z-Axis : %d %n", zMag);
	}
}