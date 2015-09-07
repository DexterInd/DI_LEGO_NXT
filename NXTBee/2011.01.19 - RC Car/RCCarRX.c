///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
///----------------------------------------------------------------------------

#pragma platform(NXT)
#include "di_crosstalk.h"

//The number of milliseconds to delay between checks of the
//receive buffer.
int MS_TO_DELAY = 100; // 100;

////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
task main()
{

	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

	//Setup the link to be a high-speed link.
  setupHighSpeedLink();
  bool isASCII = false;
  int count = 1;

  bFloatDuringInactiveMotorPWM = true;     // The motors WILL coast when power is not appli

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{

    wait1Msec(MS_TO_DELAY);

    //Set the receive buffer to all zeros.  THIS IS VERY
    //IMPORTANT.
    memset(receiveBuffer[0],0,MAX_MSG_SIZE);

    //Check for a received bytges.
    int retVal = Receive(isASCII);

    //If we have read any bytes, process them.
    if (retVal > 0)
    {
      //Handle ASCII bytes sent by
      // 1. Converting the bytes to an ASCII string.
      // 2. Displaying the string.
      if (isASCII)
      {
          // In this program it won't be.
      }
      else
      {
        // Handle non-ASCII bytes sent  by
        // displaying the bytes as a string of numbers.
        string displayString, tmpString;

        //Add each byte to the string.
        for (int ndx = 0; ndx < retVal; ++ndx)
        {
          StringFormat(tmpString, "%3d", receiveBuffer[ndx]);
          displayString += tmpString;
        }

        //Display the string.
        nxtDisplayTextLine(6,displayString);

      }
    }

    // Change Power and Direction
    int motorpower = 0;
    //nSyncedMotors = synchAC;    // Synch motors B and C so that B is the master and C is the slave.
    if(receiveBuffer[0] > 10){  // Allow some play in the joystick
      if(receiveBuffer[2] > 0){
        motorpower = receiveBuffer[0];
      }
      else{
        motorpower = receiveBuffer[0]*-1;
      }
      motor[motorA] = motorpower;
      motor[motorC] = motorpower;
    }
    else{
      motor[motorA] = 0;
      motor[motorC] = 0;
    }
    // Change Course
       int steer_posn = receiveBuffer[1] - 90;  // Steer Posn is going to be between -90 and 90
       int rx_posn = nMotorEncoder[motorB];     // This is where our motor posn is right now.
       int tx_posn = steer_posn;                // Set the  target for Motor Encoder of Motor B to 360.

        if(tx_posn > rx_posn){
            motor[motorB] = (tx_posn-rx_posn)/2;
            nMotorEncoderTarget[motorB] = rx_posn - tx_posn;
        }
        if(tx_posn < rx_posn){
            motor[motorB] = (tx_posn-rx_posn)/2;
            nMotorEncoderTarget[motorB] = tx_posn - rx_posn;
        }

        string steer_posn_str;
        StringFormat(steer_posn_str, "%3d", steer_posn);

	}
}
