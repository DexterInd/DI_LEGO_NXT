///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
///----------------------------------------------------------------------------

#pragma platform(NXT)
#include "C:\Users\John\Desktop\485\2011.01.06 Crosstalk\crosstalk.h"

//The number of milliseconds to delay between checks of the
//receive buffer.
int MS_TO_DELAY = 5000;

//The current byte to write.  This will be incremented everytime
//the method NextByte is called, until 255 is reached.  Then, it
//will roll back to zero.
byte currByte = 0;

////////////////////////////////////////////////////////////////////////
//  NextByte
//  Purpose:  Get the next byte value and increment the value by one.
//            When the value reaches 255, reset the byte to zero.
//  Returns: The next byte as a ubyte.
////////////////////////////////////////////////////////////////////////
ubyte NextByte()
{
  ubyte retVal = 0;

  if (currByte < 255)
  {
    retVal = currByte++;
  }
  else
  {
    currByte = 0;
  }
  return retVal;
}


////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
task main()
{
	eraseDisplay();
	bNxtLCDStatusDisplay = true;
  setupHighSpeedLink();

  //This is the buffer that will hold the bytes to transmit.
  ubyte testBytes[3];
  int xmitCount = 1;

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{
	  eraseDisplay();
	  //This will display the the transmission count.
    nxtDisplayTextLine(1, (string)xmitCount + " XMIT");

    //Set the bytes of the buffer to the next byte.
    testBytes[0] = NextByte();
    testBytes[1] = NextByte();
    testBytes[2] = NextByte();

    //Transmit the bytes.
    int retVal = TransmitBytes(testBytes, sizeof(testBytes));

    //Format the byte string and display it.
    string tStr0;
    string tStr1;
    string tStr2;

    StringFormat(tStr0, "%3d", testBytes[0]);
    StringFormat(tStr1, "%3d", testBytes[1]);
    StringFormat(tStr2, "%3d", testBytes[2]);

    string TempStr = tStr0 + "," + tStr1 + "," + tStr2;
    nxtDisplayTextLine(2, TempStr);

    //Delay.  Possibly, this could be used for an ACK.
    wait1Msec(MS_TO_DELAY);
	}
}
