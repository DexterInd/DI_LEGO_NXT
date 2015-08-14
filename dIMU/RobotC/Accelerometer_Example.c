#pragma config(Sensor, S1,     ATLAS,               sensorLowSpeed)

// Name:  Accelerometer Example
// Author:  Dexter Industries, 2011
// Copyright 2011, Dexter Industries
// http://www.dexterindustries.com

//  ** Max resolution for 2g is 0.015625g / bit (64 bits / g)
//  ** Max resolution for 4g is 0.03125 g / bit (32 bits / g)
//  ** Max resolution for 8g is 0.0625 g / bit  (16 bits / g)
// This program:
//    - Reads the 10-bit accelerometer and
//    - Displays the acceleration on all three axis in g
// Sensor should be placed on Port 1 of the Lego Mindstorms NXT.

#define accl_read 0x3A            //  Read address of accelerometer (00011101 + 0)
// #define accl_writ 0x13A        //  Write address of accelerometer (10011101 + 0)

int g_mode = 0x02;                // Here we define the G-Mode we're going to use.
                                  // The following options are available for the G-Mode.  Defining them here selects sensitivity.
                                  // 0x00 = +/- 2g
                                  // 0x01 = +/- 4g
                                  // 0x02 = +/- 8g

// byte x_off, y_off, z_off;         // Offset globals.

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

    // Now send the data via I2C.
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);   // (Port 1, Reply Array, Bytes to Read)
}

//////////////////////////////////////////////////////////////////////////////
// Acell axis_reading gets 8 bits of axis reading data.
// reg is the register we'll be reading from.
byte accl_axis_reading_8(ubyte reg){
  ubyte I2Csnd[3];          //
  I2Csnd[0] = 2;            // Sending address, register.
  I2Csnd[1] = accl_read;    // I2C Address of accl.
  I2Csnd[2] = reg;          // Register of the data we're requesting.

  byte I2Crec[1];        // We are looking for a single byte returned.
                         // Must be signed.  We're looking for a number
                         // between -128 and 128
  // Send the I2C Data.
  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);   // (Port 1, Reply Array, Bytes to Read)

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

  LSB = I2Crec[0];                  // Save the Least significant bit to record.
  MSB = I2Crec[1];                  // Save the Most significant bit to record.

  signed long iMSB = 0;
  if(MSB > 0) MSB = (MSB+1)<<8;     // if it's greater than zero, butshift it and add a 1.  For some reason 1 gets dropped when we shift.
  iMSB = MSB;
  iMSB += LSB;

  if(iMSB> 512){                    // Adjust for sign.  If it's positive (less than 512) then ignore.  If it's less, convert two's complement.
    iMSB = iMSB-1023;
  }

  return iMSB;                      // Return the combined value.
}


// Writes a register value on the accelerometer.
// drift_reg is the register to write to
// drift is the drift offset of the reading.
ubyte accl_axis_write(ubyte drift_reg, int drift_LSB, int drift_MSB){

  byte I2Csnd[4];           //
  I2Csnd[0] = 3;            // Sending address, register, value.
  I2Csnd[1] = accl_read;    // I2C Address of accl.
  I2Csnd[2] = drift_reg;    // Register of the data we're requesting.
  I2Csnd[3] = drift_LSB;        // The drift value to write for calibration.

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
  I2Csnd[3] = drift_MSB;

  // Write most significant bit of the offset.
  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)
  wait1Msec(10);
  return I2Crec[0];
}

// This assumes that the z axis is placed flat.  When sensor is flat on the ground
// the x and y axis should be zero.
// There may be a better way to calibrate this.  But it's the most user-friendly.
// Gravity direction is 1 (x), 2 (y), 3 (z) and 0 (off).  This is the field direction that gravity is headed.
void calibrate_accel(int gravity_direction){
  // Start program by calibrating all three axes.  The sensor should be placed flat to the ground with the z-axis
  // pointed directly into the ground.
  eraseDisplay();
  nxtDisplayTextLine(1, "Enter Cal Mode");
  nxtDisplayTextLine(4, "Place flat.");
  wait10Msec(100);
  eraseDisplay();

  // Clear all of the drift registers.
  accl_axis_write(0x10, 0x00, 0x00);             // Reset all LSB calibration to 0
  accl_axis_write(0x12, 0x00, 0x00);             // Reset all LSB calibration to 0
  accl_axis_write(0x14, 0x00, 0x00);             // Reset all LSB calibration to 0

  long x_drift = accl_axis_reading_10(0x00);     // Read the x-axis reading.
  long y_drift = accl_axis_reading_10(0x02);     // Read the y-axis reading.
  long z_drift = accl_axis_reading_10(0x04);     // Read the z-axis reading.

  writeDebugStream("X reading: ");
  writeDebugStreamLine("%dl", x_drift);
  writeDebugStream("Y reading: ");
  writeDebugStreamLine("%dl", y_drift);
  writeDebugStream("Z reading: ");
  writeDebugStreamLine("%dl", z_drift);

  // This number changes depending on the g-force.  This is automatically calculated by the value of g_mode.
  // We will calibrate depending on the axis that's inline with the gravity field.
  ubyte one_g = 0x40;     //  10 bits is 1024, over 16 g's that's 64 bits / G.  Positive 1G is 64 decimal, or 0x40 Hex.
  int fctr = -2;          // We multiply all values of correction by 2, and then their opposite.

  switch(gravity_direction){

   case 1:
      y_drift *= fctr;    // Treat it normally now and zero it out.
      z_drift *= fctr;    // Treated normally here and just zero it out.
      x_drift -= one_g;   // Subtract out the 1-g that the z-axis is experiencing.
      x_drift *= fctr;    // Treated normally here and just zero it out.
    writeDebugStreamLine("Case 1, cal to X");
    break;

   // Calibrate Y-axis to gravity field.
   case 2:
      x_drift *= fctr;    // Treated normally here and just zero it out.
      z_drift *= fctr;    // Treated normally here and just zero it out.
      y_drift -= one_g;   // Subtract out the 1-g that the z-axis is experiencing.
      y_drift *= fctr;    // Treat it normally now and zero it out.
    writeDebugStreamLine("Case 2, cal to Y");
    break;

   default:
      x_drift *= fctr;    // Treated normally here and just zero it out.
      y_drift *= fctr;    // Treated normally here and just zero it out.
      z_drift -= one_g;   // Subtract out the 1-g that the z-axis is experiencing.
      z_drift *= fctr;    // Treat it normally now and zero it out.
    writeDebugStreamLine("Case 0 Selected");
    break;
  }

  writeDebugStreamLine(" - - - - - - - ");
  writeDebugStream("X factored: ");
  writeDebugStreamLine("%dl", x_drift);
  writeDebugStream("Y factored: ");
  writeDebugStreamLine("%dl", y_drift);
  writeDebugStream("Z factored: ");
  writeDebugStreamLine("%dl", z_drift);
  writeDebugStreamLine(" - - - - - - - ");

  byte x_drift_LSB = (byte)(x_drift);
  byte x_drift_MSB = x_drift>> 8;
  byte y_drift_LSB = (byte)(y_drift);
  byte y_drift_MSB = y_drift>> 8;
  byte z_drift_LSB = (byte)(z_drift);
  byte z_drift_MSB = z_drift>> 8;

  // Read Back the Register Values of Offset to Double Check
  // Verify in Debug Stream.
  byte x_returned = accl_axis_write(0x10, (x_drift_LSB), x_drift_MSB);  // Write all the drifts to register.
  byte y_returned = accl_axis_write(0x12, (y_drift_LSB), y_drift_MSB);
  byte z_returned = accl_axis_write(0x14, (z_drift_LSB), z_drift_MSB);


  wait10Msec(100);
}

task main(){
  g_mode = 0x02;                // Here we define the G-Mode we're going to use.
                                // 0x00 = +/- 2g
                                // 0x01 = +/- 4g
                                // 0x02 = +/- 8g
  start_accl();               // Fire up the accelerometer.  Initialize it.    Only needs to be done once.
  calibrate_accel(3);         // Calibrated z-axis.  Call 1 x-axis, 2 y-axis, 3 z-axis.  Default is z-axis.


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

      float f_x_10Bit = x_10Bit/64.000000;    //  Normalize the 10-bit data to g's.
      float f_y_10Bit = y_10Bit/64.000000;    //
      float f_z_10Bit = z_10Bit/64.000000;    //

      nxtDisplayTextLine(1, "%f", i_x_acc);   //  Print the 8-bit data to screen.
      nxtDisplayTextLine(2, "%f", i_y_acc);
      nxtDisplayTextLine(3, "%f", i_z_acc);

      nxtDisplayTextLine(5, "%f", f_x_10Bit); //  Print the 10-bit data to screen.
      nxtDisplayTextLine(6, "%f", f_y_10Bit);
      nxtDisplayTextLine(7, "%f", f_z_10Bit);
  }
}
