///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
///----------------------------------------------------------------------------

#pragma platform(NXT)
#include "C:\Users\John\Desktop\Desktop Packaged\485\2011.01.19 - RC Car\di_crosstalk.h"

void setupHighSpeedLink(const bool bMaster)
{
	// Initialize port S$ to "high speed" mode.
	nxtEnableHSPort();
	nxtSetHSBaudRate(9600);
	nxtHS_Mode = hsMsgModeMaster;
	return;
}

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

  bFloatDuringInactiveMotorPWM = true;     // The motors WILL coast when power is not appli
  int motorpower = 0;                      // Variable holds the power.

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{

    wait1Msec(MS_TO_DELAY);


    // Change Power and Direction

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
