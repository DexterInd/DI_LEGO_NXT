#pragma config(Sensor, S1,     ATLAS,               sensorLowSpeed)

// Name:  dAccel 2.0.c
// Author:  Dexter Industries, 2011
// Copyright 2011, Dexter Industries
//
//  ** THIS EXAMPLE WORKS WELL for all G
//  ** Max resolution for 2g is 0.015625g / bit (64 bits / g)
//  ** Max resolution for 4g is 0.03125 g / bit (32 bits / g)
//  ** Max resolution for 8g is 0.0625 g / bit  (16 bits / g)
// This program:
//    - Reads the 10-bit accelerometer and
//    - Displays the acceleration on all three axis in g

#define accl_read 0x3A            //  Read address of accelerometer (00011101 + 0)
// #define accl_writ 0x13A           //  Write address of accelerometer (10011101 + 0)

int g_mode = 0x02;                // Here we define the G-Mode we're going to use.
                                  // 0x00 = +/- 2g
                                  // 0x01 = +/- 4g
                                  // 0x02 = +/- 8g

byte x_off, y_off, z_off;         // Offset globals.

//////////////////////////////////////////////////////////////////////////////
// Acell axis_reading gets 8 bits of axis reading data.
//
void start_accl(){
  ubyte I2Csnd[4];
  I2Csnd[0] = 3;          // Sending address, register, value.
  I2Csnd[1] = accl_read;  // I2C Address of Accelerometer.
  ubyte I2Crec[1];        // We are looking for a single byte returned.

  //Set the Mode Control - P.25 of Documentation
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x16;                   // Register address of Mode Control
    I2Csnd[3] = 0x05;                   // 0x05 For 2G
    if(g_mode > 0x00) I2Csnd[3] = 0x09; // 0x09 For 4G
    if(g_mode > 0x01) I2Csnd[3] = 0x01; // 0x01 For 8G

    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)
}

//////////////////////////////////////////////////////////////////////////////
// Acell axis_reading gets 8 bits of axis reading data.
// reg is the register we'll be reading from.
byte accl_axis_reading_8(ubyte reg){
  ubyte I2Csnd[3];        //
  I2Csnd[0] = 2;      // Sending address, register.
  I2Csnd[1] = accl_read;   // I2C Address of accl.
  I2Csnd[2] = reg;    // Register of the data we're requesting.

  byte I2Crec[1];        // We are looking for a single byte returned.
                         // Must be signed.  We're looking for a number
                         // between -128 and 128

  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

  wait1Msec(10);
  return I2Crec[0];
}

//////////////////////////////////////////////////////////////////////////////
// Accel axis reading gets 10 bits of axis reading data.
// reg is the register we'll be reading.  By register I mean the x, y, or z register.
long accl_axis_reading_10(ubyte reg){

  byte LSB = 0x00;       // Least significant bit of reading.
  byte MSB = 0x00;       // Most significant bit of reading.
  signed long axis_reading = 0;  // This will be the assembled reading.

  ubyte I2Csnd[3];        //
  I2Csnd[0] = 2;          // Sending address, register.
  I2Csnd[1] = accl_read;  // I2C Address of accl.
  I2Csnd[2] = reg;        // First Register of the data we're requesting.

  byte I2Crec[2];         // We are looking for a single byte returned each time.
                          // When you call any of the 2byt LSB registers, it will send the second byte for free!

  // Get reading.
  sendI2CMsg(S1, I2Csnd[0], 2);     //  Port 1, Message Array, Reply Size
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 2);   //  Port 1, Reply Array, Bytes to Read

  LSB = I2Crec[0]; // & 0xFF;    // Save the LSB to record.
  MSB = I2Crec[1];

  signed long iMSB = 0;
  if(MSB > 0) MSB = (MSB+1)<<8;     // if it's greater than zero, butshift it and add a 1.  For some reason 1 gets dropped when we shift.
  iMSB = MSB;
  iMSB += LSB;

  if(iMSB> 512){                    // Adjust for sign.  If it's positive (less than 512) then ignore.  If it's less, convert two's complement.
    iMSB = iMSB-1023;
  }

  return iMSB;
}


// Writes a register value on the accelerometer.
// drift_reg is the register to write to
// drift is the drift offset of the reading.
ubyte accl_axis_write(ubyte drift_reg, int drift){

  byte I2Csnd[4];           //
  I2Csnd[0] = 3;            // Sending address, register, value.
  I2Csnd[1] = accl_read;    // I2C Address of accl.
  I2Csnd[2] = drift_reg;    // Register of the data we're requesting.
  I2Csnd[3] = drift;        // The drift value to write for calibration.

  /*
  writeDebugStream("Drift Val:");
  writeDebugStreamLine("%i", drift);
  */

  ubyte I2Crec[1];          // We are looking for a single byte returned.

  // Write least significant bit of the offset.

  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);   // (Port 1, Reply Array, Bytes to Read)

  wait1Msec(10);

  I2Csnd[2] = drift_reg++;
  // Handling the sign.  If the value of drift is greater than 127, it's a negative number.
  if(drift > 127) I2Csnd[3] = 0x07;   //  Drift is a ngtv number, we have to mask MSBits of Offset.
  if(drift <= 127) I2Csnd[3] = 0x00;  //  Drift is a pstv number, we don't have to do anything.

  // Write most significant bit of the offset.
  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)
  wait1Msec(10);
  return I2Crec[0];

  //if(drift > 127) I2Csnd[3] = 0x03;    // If drift is ngtv number, we have to set MSB's to 1's.
  //I2Csnd[2] = drift_reg+1;    // Register of the data we're requesting.

  //sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  //wait1Msec(10);
  //readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)
  //wait1Msec(10);
  //return I2Crec[0];
}

// This assumes that the z axis is placed flat.  When sensor is flat on the ground
// the x and y axis should be zero.
// There may be a better way to calibrate this.  But it's the most user-friendly.
void calibrate_accel(){
  eraseDisplay();
  nxtDisplayTextLine(1, "Enter Cal Mode");
  nxtDisplayTextLine(4, "Place flat.");
  wait10Msec(100);
  eraseDisplay();

  // Clear all of the drift registers.
  accl_axis_write(0x10, 0x00);                   // Reset all LSB calibration to 0
  //accl_axis_write(0x11, 0x00);
  accl_axis_write(0x12, 0x00);                   // Reset all LSB calibration to 0
  //accl_axis_write(0x13, 0x00);
  accl_axis_write(0x14, 0x00);                   // Reset all LSB calibration to 0
  //accl_axis_write(0x15, 0x00);

  byte x_drift = accl_axis_reading_8(0x06);     // Read the x-axis reading.
  byte y_drift = accl_axis_reading_8(0x07);     // Read the y-axis reading.
  byte z_drift = accl_axis_reading_8(0x08);     // Read the z-axis reading.

  // This number changes depending on the g-force.  This is automatically calculated by the value of g_mode.
  ubyte z_factor = 0x0F;

  /*if(g_mode == 0x00) z_factor = 0x3F;                      // Placed right-side-up and perpendicular to the ground, axis should read 0
  if(g_mode == 0x01) z_factor = 0x1F;                      // Placed right-side-up and perpendicular to the ground, axis should read 0x3F
  if(g_mode == 0x02) z_factor = 0x0F;
  */

  writeDebugStream("Z_Factor: ");
  writeDebugStreamLine("%x", z_factor);
  writeDebugStream("Z_dift: ");
  writeDebugStreamLine("%x", z_drift);

  if(z_drift > z_factor){
    z_drift += z_factor;
    //z_drift -= z_factor;
  }
  else{
    z_drift -= z_factor;
    //z_drift += z_factor;
  }

  writeDebugStream("Z_dift Mod: ");
  writeDebugStreamLine("%x", z_drift);

  // Does the multiplier here change between g readings?
  int fctr = -2;

  /*
  if(g_mode == 0x00) fctr = -2;
  if(g_mode == 0x01) fctr = -2;
  if(g_mode == 0x02) fctr = -2;
  */

  // Show calculated offset values in Debug Stream
  // Verify in Debug Stream.
  /*
  writeDebugStream("Write X Off:");
  writeDebugStreamLine("%x", (fctr*x_drift));
  writeDebugStream("Write Y Off:");
  writeDebugStreamLine("%x", (fctr*y_drift));
  writeDebugStream("Write Z Off:");
  writeDebugStreamLine("%x", (fctr*z_drift));

  */

  // Read Back the Register Values of Offset to Double Check
  // Verify in Debug Stream.
  byte x_returned = accl_axis_write(0x10, (fctr*x_drift));  // Write all the drifts to register.
  if (x_drift > 0) accl_axis_write(0x11, 0x03);
  byte y_returned = accl_axis_write(0x12, (fctr*y_drift));
  if (y_drift > 0) accl_axis_write(0x13, 0x03);
  byte z_returned = accl_axis_write(0x14, (fctr*z_drift));
  //if (z_drift > 0) accl_axis_write(0x15, 0x03);

  wait10Msec(100);

  // Read Back the Register Values of Offset to Double Check
  // Verify in Debug Stream.
  /*
  writeDebugStream("Read X Off:");
  writeDebugStreamLine("%x", accl_axis_reading_8(0x10));
  writeDebugStream("Read Y Off:");
  writeDebugStreamLine("%x", accl_axis_reading_8(0x12));
  writeDebugStream("Read Z Off:");
  writeDebugStreamLine("%x", accl_axis_reading_8(0x14));
  */
}

task main(){
  g_mode = 0x02;                // Here we define the G-Mode we're going to use.
                                // 0x00 = +/- 2g
                                // 0x01 = +/- 4g
                                // 0x02 = +/- 8g
  start_accl();               // Fire up the accelerometer.  Initialize it.    Only needs to be done once.
  calibrate_accel();            // Calibrated z-axis.


  // Select the divisor to bring us from bytes to g's.
  float div = 64.0000;                // Assume 2g
  if(g_mode == 0x00) div = 64.0000;   // 2g is 64 LSB/g
  if(g_mode == 0x01) div = 32.0000;   // 4g is 32 LSB/g
  if(g_mode == 0x02) div = 16.0000;   // 8g is 16 LSB/g

  while (true){

      // ACCELEROMETER
      // In this example, we'll just get the 8-bit data.

      byte x_acc = accl_axis_reading_8(0x06);         // Get x axis data.
      byte y_acc = accl_axis_reading_8(0x07);         // Get y axis data.
      byte z_acc = accl_axis_reading_8(0x08);         // Get z axis data.
      long x_10Bit = accl_axis_reading_10(0x00);      // Get x axis data.
      long y_10Bit = accl_axis_reading_10(0x02);      // Get y axis data.
      long z_10Bit = accl_axis_reading_10(0x04);      // Get z axis data.

      eraseDisplay();

      float i_x_acc = ((int)(x_acc))/div;
      float i_y_acc = ((int)(y_acc))/div;
      float i_z_acc = ((int)(z_acc))/div;

      float f_x_10Bit = x_10Bit/64.000000;
      float f_y_10Bit = y_10Bit/64.000000;
      float f_z_10Bit = z_10Bit/64.000000;

      //nxtDisplayTextLine(1, "8 Bit x,y,z");

      nxtDisplayTextLine(1, "%f", i_x_acc);
      nxtDisplayTextLine(2, "%f", i_y_acc);
      nxtDisplayTextLine(3, "%f", i_z_acc);

      //nxtDisplayTextLine(5, "10 Bit x,y,z");

      nxtDisplayTextLine(5, "%f", f_x_10Bit);
      nxtDisplayTextLine(6, "%f", f_y_10Bit);
      nxtDisplayTextLine(7, "%f", f_z_10Bit);
  }
}
