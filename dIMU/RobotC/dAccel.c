#pragma config(Sensor, S1,     ATLAS,               sensorLowSpeed)

#define accl_read 0x3A        //  Read address of accelerometer (00011101 + 0)
#define accl_writ 0x13A       //  Write address of accelerometer (10011101 + 0)

void start_accl(){
  ubyte I2Csnd[4];
  I2Csnd[0] = 3;    // Sending address, register, value.
  I2Csnd[1] = accl_read; // I2C Address of Accelerometer.
  ubyte I2Crec[1];        // We are looking for a single byte returned.

  //Set the Mode Control
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x16;       // Register address of Mode Control
    I2Csnd[3] = 0x05;       // 0x05 For 2G
                            // 0x09 For 4G
                            // 0x01 For 8G
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)
}

// Acell axis_reading gets a byte of axis reading data
ubyte accl_axis_reading(ubyte reg){
  ubyte I2Csnd[3];        //
  I2Csnd[0] = 2;      // Sending address, register.
  I2Csnd[1] = accl_read;   // I2C Address of accl.
  I2Csnd[2] = reg;    // Register of the data we're requesting.

  ubyte I2Crec[1];        // We are looking for a single byte returned.

  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

  wait1Msec(10);
  return I2Crec[0];
}

// Writes a register value on the accelerometer.
// drift_reg is the register to write to
// drift is the drift offset of the reading.
ubyte accl_axis_write(ubyte drift_reg, int drift){

  byte I2Csnd[4];           //
  I2Csnd[0] = 3;            // Sending address, register.
  I2Csnd[1] = accl_writ;    // I2C Address of accl.
  I2Csnd[2] = drift_reg;    // Register of the data we're requesting.
  I2Csnd[3] = drift;        // The drift value to write for calibration.

  ubyte I2Crec[1];          // We are looking for a single byte returned.

  // Write least significant bit of the offset.

  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

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
}

// This assumes that the z axis is placed flat.  There may be a better way to calibrate this.
void calibrate_accel(){
  eraseDisplay();
  nxtDisplayTextLine(1, "Enter Cal Mode");
  nxtDisplayTextLine(4, "Place flat.");
  wait10Msec(1);
  eraseDisplay();

  // Clear all of the drift registers.
  //for(byte i = 0x10; i < 0x16; i++)                    // Loop from registers 0x10 to 0x16.
  //{
    accl_axis_write(0x10, 0x00);              // Reset all LSB calibration to 0
    // accl_axis_write((drift_reg+1), 0x00);          // Reset all MSB calibration to 0
  //}

  ubyte x_drift = accl_axis_reading(0x06);     // Read the x-axis reading.
  ubyte y_drift = accl_axis_reading(0x07);     // Read the y-axis reading.
  ubyte z_drift = accl_axis_reading(0x08);     // Read the z-axis reading.

  z_drift = (z_drift-0x3F);                           // Placed right-side-up and perpendicular to the ground, axis should read 0x3F

  byte x_returned = accl_axis_write(0x10, (-2*x_drift)); // Write all the drifts.
  byte y_returned = accl_axis_write(0x12, (-2*y_drift));
  byte z_returned = accl_axis_write(0x14, z_drift);

  wait10Msec(1);
}

task main(){
  start_accl();               // Fire up the accelerometer.  Initialize it.    Only needs to be done once.
  calibrate_accel();          // Calibrated z-axis.

  while (true){

      // ACCELEROMETER
      // In this example, we'll just get the 8-bit data.  This can be extended to
      // 10 bit by going into the data sheet.

      byte x_acc = accl_axis_reading(0x06);         // Get x axis data.
      byte y_acc = accl_axis_reading(0x07);         // Get y axis data.
      byte z_acc = accl_axis_reading(0x08);         // Get z axis data.

      eraseDisplay();

      // 2g is 64 LSB/g
      // 4g is 32 LSB/g  //
      // 8g is 16 LSB/g  // Also note if 8g, must use LSB/MSB

      float i_x_acc = ((int)x_acc)/64.000;
      float i_y_acc = ((int)y_acc)/64.000;
      float i_z_acc = ((int)z_acc)/64.000;

      nxtDisplayTextLine(1, "%f", i_x_acc);
      nxtDisplayTextLine(2, "%f", i_y_acc);
      nxtDisplayTextLine(3, "%f", i_z_acc);

    wait1Msec(50);
  }
}
