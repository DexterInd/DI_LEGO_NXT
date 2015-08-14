#pragma config(Sensor, S1,     ATLAS,               sensorLowSpeed)

// These bytes set the full scale range of the gyroscope.
// it is important to define full_scale_range.  Values are below.
//0x00 - 250 dps.  Full scale range.
//0x10 - 500 dps.  Full scale range.
//0x30 - 2000 dps.  Full scale range.
#define full_scale_range 0x30
int divisor = 128;      // This will be the divisor we divide the raw value of the gryscope by
                        // to get a scaled value on output.  Default will be for 250 dps,
                        // but we'll define it again in start_gyro.

void start_gyro(ubyte range){
  ubyte I2Csnd[4];
  I2Csnd[0] = 3;    // Sending address, register, value.
  I2Csnd[1] = 0xD2; // I2C Address of gyro.
  ubyte I2Crec[1];        // We are looking for a single byte returned.

  // full_scale_range =  + 0x80;

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
    I2Csnd[3] = 0xB0;//range+0x80;     // Full scale range.
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

// Gyro: gets a full axis reading, scaled to the full scale reading.  Returns
// long in degrees per second.

float gryo_axis_reading_linked(ubyte axis){
 // ubyte axis definitions
 // 0x00 - x-axis
 // 0x01 - y-axis
 // 0x02 - z-axis

 ubyte I2Csnd[3];        //
 I2Csnd[0] = 2;      // Sending address, register.
 I2Csnd[1] = 0xD2;   // I2C Address of gyro.
 ubyte I2Crec[2];    // We are looking for a single byte returned.

 ubyte hb = 0;
 ubyte lb = 0;

 // We will set the value of divisor.  This is important to get a scaled output in dps.
 int divisor = 128;
 if(full_scale_range == 0x10) divisor = 64;
 if(full_scale_range == 0x30) divisor = 16;

 if(axis == 0x00) {               // x-axis
  I2Csnd[2] = 0x28;    // Register of the data we're requesting.
  sendI2CMsg(S1, I2Csnd[0], 2);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 2);   // (Port 1, Reply Array, Bytes to Read)

  // hb = gyro_axis_reading_byte(0x29);
  // lb = gyro_axis_reading_byte(0x28);
  lb = I2Crec[0];
  hb = I2Crec[1];
 }
 if(axis == 0x01) {               // y-axis
  I2Csnd[2] = 0x2B;    // Register of the data we're requesting.
  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 2);   // (Port 1, Reply Array, Bytes to Read)

  // ubyte result = I2Crec[0];

  // hb = gyro_axis_reading_byte(0x2B);
  // lb = gyro_axis_reading_byte(0x2A);
  hb = I2Crec[0];
  lb = I2Crec[1];
 }
 if(axis == 0x02) {               // z-axis
  I2Csnd[2] = 0x2D;    // Register of the data we're requesting.
  sendI2CMsg(S1, I2Csnd[0], 1);     // (Port 1, Message Array, Reply Size)
  wait1Msec(10);
  readI2CReply(S1, I2Crec[0], 2);   // (Port 1, Reply Array, Bytes to Read)


  // hb = gyro_axis_reading_byte(0x2D);
  // lb = gyro_axis_reading_byte(0x2C);
  hb = I2Crec[0];
  lb = I2Crec[1];
 }
 float val = (lb+((long)(hb<<8)))/divisor;  // Assemble the final number by assembling the two bytes,
                                           // and dividing it by the divisor (defined in the gyro startup,
                                           // to get a properly scaled long.
 return val;
}


task main(){


  start_gyro(full_scale_range);  // Fire up the gyro.  Initialize it.             Only needs to be done once.
  long x_val, y_val, z_val;      // Our assembled values.

  const string sFileName = "GYRO2000.txt";  // Our destination KML File
  TFileIOResult nIoResult;
  TFileHandle hFileHandle;
  int nFileSize = 5500;
  hFileHandle = 0;
  Delete(sFileName, nIoResult);
  OpenWrite(hFileHandle, nIoResult, sFileName, nFileSize);

  while (true){

        // Read the GYROSCOPE
        //x_val = gryo_axis_reading(0x00);    // Get x-axis in dps.
        y_val = gryo_axis_reading_linked(0x00);
        //y_val = gryo_axis_reading(0x01);    // Get x-axis in dps.
        //z_val = gryo_axis_reading(0x02);    // Get x-axis in dps.

        string str_x_val = (string)x_val;     // Turn it into a string to write to file.

        nxtDisplayTextLine(5, "%i", x_val);
        nxtDisplayTextLine(6, "%i", y_val);
        nxtDisplayTextLine(7, "%i", z_val);

        string write_coords = " ";
        strcat(write_coords, str_x_val);
        strcat(write_coords, "\r\n");         // Every line is a return and new line.
        WriteString(hFileHandle, nIoResult, write_coords);    // Write it all to file.
        wait1Msec(50);
  }
}
