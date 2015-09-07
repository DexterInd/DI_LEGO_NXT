///----------------------------------------------------------------------------
/// flagwave_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
///----------------------------------------------------------------------------

#pragma platform(NXT)
#include "di_crosstalk.h"

//The number of milliseconds to delay between checks of the
//receive buffer.
int MS_TO_DELAY = 10;

//The current byte to write.  This will be incremented everytime
//the method NextByte is called, until 255 is reached.  Then, it
//will roll back to zero.
byte currByte = 0;

////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
task main()
{
	eraseDisplay();
	bNxtLCDStatusDisplay = true;
  setupHighSpeedLink();

  //This is the buffer that will hold the bytes to transmit.
  ubyte testBytes[1];
  int xmitCount = 1;

  nMotorEncoder[motorB] = 0;          // Reset the Motor Encoder of Motor B.

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{
	  eraseDisplay();

    int motorposn = nMotorEncoder[motorB];

    //This will display the the motor position.
    nxtDisplayTextLine(1, (string)motorposn + " XMIT");

    //Set the bytes of the buffer to the next byte.
    testBytes[0] = motorposn;

    //Transmit the bytes.
    int retVal = TransmitBytes(testBytes, sizeof(testBytes));

    //Format the byte string and display it.
    string tStr0;
    StringFormat(tStr0, "%3d", testBytes[0]);
    string TempStr = tStr0;
    nxtDisplayTextLine(2, TempStr);

    //Delay.  Possibly, this could be used for an ACK.
    wait1Msec(MS_TO_DELAY);
	}
}
