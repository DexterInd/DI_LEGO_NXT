#pragma config(Sensor, S1,     ATLAS,               sensorLowSpeed)

// These bytes set the full scale range of the gyroscope.
// it is important to define full_scale_range.  Values are below.
//0x00 - 250 dps.  Full scale range.
//0x10 - 500 dps.  Full scale range.
//0x30 - 2000 dps.  Full scale range.
// #define full_scale_range 0x30
#define full_scale_range 0x00
int divisor = 128;      // This will be the divisor we divide the raw value of the gryscope by
                        // to get a scaled value on output.  Default will be for 250 dps,
                        // but we'll define it again in start_gyro.

void start_gyro(ubyte range){
  ubyte I2Csnd[4];
  I2Csnd[0] = 3;    // Sending address, register, value.
  I2Csnd[1] = 0xD2; // I2C Address of gyro.
  ubyte I2Crec[1];        // We are looking for a single byte returned.

  //Write CTRL_REG1
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x20;      // Register address of CTRL_REG1
    I2Csnd[3] = 0x0F;      // Enable all axes. Disable power down.
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

  //Write CTRL_REG2
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x21;                  // Register address of CTRL_REG2
    I2Csnd[3] = 0x00;                  // No High Pass Filter
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);   // (Port 1, Reply Array, Bytes to Read)

  //Write CTRL_REG3
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x22;      // Register address of CTRL_REG3
    I2Csnd[3] = 0x08;      // No interrupts.  Date ready.
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

  //Write CTRL_REG4
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x23;      // Register address of CTRL_REG4
    I2Csnd[3] = range;     // Full scale range.
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

  //Write CTRL_REG5
  ////////////////////////////////////////////////////////////////////////////
    I2Csnd[2] = 0x24;      // Register address of CTRL_REG5
    I2Csnd[3] = 0x00;      // Enable all axes. Disable power down.
    sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
    wait1Msec(10);
    readI2CReply(S1, I2Crec[0], 1);  // (Port 1, Reply Array, Bytes to Read)

   // Set divisor so that the output of our gyro axis readings can be turned
   // into scaled values.
   ///////////////////////////////////////////////////////////////////////////
    if(full_scale_range == 0) divisor = 128;      // Full scale range is 250 dps.
    if(full_scale_range == 0x10) divisor = 64;    // Full scale range is 500 dps.
    if(full_scale_range == 0x30) divisor = 16;    // Full scale range is 2000 dps.

}

// Gyro: axis_reading gets a byte of axis reading data
//
ubyte gyro_axis_reading_byte(ubyte reg){

  ubyte I2Csnd[3];        //
  I2Csnd[0] = 2;      // Sending address, register.
  I2Csnd[1] = 0xD2;   // I2C Address of gyro.
  I2Csnd[2] = reg;    // Register of the data we're requesting.

  ubyte I2Crec[1];    // We are looking for a single byte returned.

  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 1);   // (Port 1, Reply Array, Bytes to Read)

  ubyte result = I2Crec[0];
  return result;
}

// Gyro: gets a full axis reading, scaled to the full scale reading.  Returns
// long in degrees per second.

float gryo_axis_reading(ubyte axis){
 // ubyte axis definitions
 // 0x00 - x-axis
 // 0x01 - y-axis
 // 0x02 - z-axis

 ubyte hb = 0;
 ubyte lb = 0;

 // We will set the value of divisor.  This is important to get a scaled output in dps.
 int divisor = 128;                           // 250 dps
 if(full_scale_range == 0x10) divisor = 64;   // 500 dps
 if(full_scale_range == 0x30) divisor = 16;   // 2000 dps

 if(axis == 0x00) {               // x-axis
  hb = gyro_axis_reading_byte(0x29);
  lb = gyro_axis_reading_byte(0x28);
 }
 if(axis == 0x01) {               // y-axis
  hb = gyro_axis_reading_byte(0x2B);
  lb = gyro_axis_reading_byte(0x2A);
 }
 if(axis == 0x02) {               // z-axis
  hb = gyro_axis_reading_byte(0x2D);
  lb = gyro_axis_reading_byte(0x2C);
 }
 float val = (lb+((long)(hb<<8)))/divisor;  // Assemble the final number by assembling the two bytes,
                                           // and dividing it by the divisor (defined in the gyro startup,
                                           // to get a properly scaled long.
 return val;
}


task main(){


  start_gyro(full_scale_range);  // Fire up the gyro.  Initialize it.             Only needs to be done once.
  long x_val, y_val, z_val;      // Our assembled values.

  while (true){

        // Read the GYROSCOPE
        x_val = gryo_axis_reading(0x00);    // Get x-axis in dps.
        y_val = gryo_axis_reading(0x01);    // Get y-axis in dps.
        z_val = gryo_axis_reading(0x02);    // Get z-axis in dps.

        nxtDisplayTextLine(5, "%i", x_val);
        nxtDisplayTextLine(6, "%i", y_val);
        nxtDisplayTextLine(7, "%i", z_val);

        wait10Msec(100);
  }
}
