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
int MS_TO_DELAY = 10; // 100;

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

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{

    wait1Msec(MS_TO_DELAY);

    //Display the polling count
    nxtDisplayTextLine(1, (string)count + " Polling");
    ++count;

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
        string line = "";
        GetStringFromBuffer(retVal, line);
        nxtDisplayTextLine(5, line);
      }
      else
      {
        //Handle non-ASCII bytes sent  by
        // displaying the bytes as a string of numbers.
        string displayString = "";
        string tmpString = "";

        //Add each byte to the string.
        for (int ndx = 0; ndx < retVal; ++ndx)
        {
          StringFormat(tmpString, "%3d", receiveBuffer[ndx]);
          displayString += tmpString;
        }

        //Display the string.
        nxtDisplayTextLine(6,displayString);
        //nMotorEncoder[motorB] = 0;                // Reset the Motor Encoder of Motor B.
        int rx_posn = nMotorEncoder[motorB];   // This is where our motor posn is right now.
        int tx_posn = receiveBuffer[0];      // Set the  target for Motor Encoder of Motor B to 360.

        if(tx_posn > rx_posn){
            motor[motorB] = (tx_posn-rx_posn)/2;
            nMotorEncoderTarget[motorB] = rx_posn - tx_posn;
        }
        if(tx_posn < rx_posn){
            motor[motorB] = (tx_posn-rx_posn)/2;
            nMotorEncoderTarget[motorB] = tx_posn - rx_posn;
        }

        StringFormat(displayString, "%3d", (rx_posn);
        nxtDisplayTextLine(4,displayString);
        //motor[motorB] = 75;                               // Motor B is run at a power level of 75.




      }
    }

	}
}
