#pragma config(Sensor, S1,     DIMU,                sensorI2CCustomFastSkipStates)
/**
 * IMU-driver.h provides an API for the Dexter Industries IMU Sensor.  This program
 * demonstrates how to use that API.
 * License: You may use this code as you wish, provided you give credit where it's due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 2.00 AND HIGHER.
 */

#include "drivers/DIMU-driver.h"


task main(){

  float g_x_val, g_y_val, g_z_val;      // Gyro axis values
  float a_x_val, a_y_val, a_z_val;      // Gyro axis values

  nxtDisplayCenteredTextLine(0, "Dexter Ind.");
  nxtDisplayCenteredBigTextLine(1, "IMU");
  nxtDisplayCenteredTextLine(3, "Datalogger");
  wait1Msec(2000);
  eraseDisplay();

  // Fire up the gyro and initialize it. Only needs to be done once.
  //DIMUconfigGyro(DIMU, DIMU_GYRO_RANGE_500);
  if (!DIMUconfigAccel(DIMU, DIMU_ACC_RANGE_2G))
    PlaySound(soundException);

  if(!DIMUconfigGyro(DIMU, DIMU_GYRO_RANGE_250))
    PlaySound(soundException);

  //////////////////
  // File Initiation
  const string sFileName = "Travel.txt";  // Our destination KML File
  TFileIOResult nIoResult;
  TFileHandle hFileHandle;
  int nFileSize = 90000;
  hFileHandle = 0;
  Delete(sFileName, nIoResult);
  OpenWrite(hFileHandle, nIoResult, sFileName, nFileSize);

  while (true){
		// Read the GYROSCOPE
    // All at once, very convenient if you need all 3
    DIMUreadGyroAxes(DIMU, g_x_val, g_y_val, g_z_val);

		nxtDisplayTextLine(1, "%f", g_x_val);
		nxtDisplayTextLine(2, "%f", g_y_val);
		nxtDisplayTextLine(3, "%f", g_z_val);

		// Read the ACCELEROMETER
		a_x_val = DIMUreadAccelAxis10Bit(DIMU, DIMU_ACC_X_AXIS);
		a_y_val = DIMUreadAccelAxis10Bit(DIMU, DIMU_ACC_Y_AXIS);
		a_z_val = DIMUreadAccelAxis10Bit(DIMU, DIMU_ACC_Z_AXIS);

		nxtDisplayTextLine(5, "%f", a_x_val);
		nxtDisplayTextLine(6, "%f", a_y_val);
		nxtDisplayTextLine(7, "%f", a_z_val);

		// Now write to file
		string line = (string)g_x_val;     // Turn it into a string to write to file.
		strcat(line, ",");
		WriteString(hFileHandle, nIoResult, line);    // Write it all to file.
		line = (string)g_y_val;
		strcat(line, ",");
		WriteString(hFileHandle, nIoResult, line);    // Write it all to file.
		line = (string)g_z_val;
		strcat(line, ",");
		WriteString(hFileHandle, nIoResult, line);    // Write it all to file.
  	line = (string)a_x_val;
		strcat(line, ",");
		WriteString(hFileHandle, nIoResult, line);    // Write it all to file.
		line = (string)a_y_val;
		strcat(line, ",");
		WriteString(hFileHandle, nIoResult, line);    // Write it all to file.
		line = (string)a_z_val;
		strcat(line, "\r\n");
    WriteString(hFileHandle, nIoResult, line);    // Write it all to file.
		wait1Msec(500);
  }
}
