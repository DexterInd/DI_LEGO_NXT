///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
///----------------------------------------------------------------------------

#pragma platform(NXT)

void setupHighSpeedLink()
{
	// Initialize port S$ to "high speed" mode.
	nxtEnableHSPort();
	nxtSetHSBaudRate(9600);
	nxtHS_Mode = hsMsgModeMaster;
	return;
}


////////////////////////////////////////////////////////////////////////
//  Reads a Response from the XBee
//  Displays the response from the XBee
//  This reads any response from the XBee
////////////////////////////////////////////////////////////////////////
int ReadResponse(int vector1)
  {
    byte BytesRead;                  // Array we'll be reading into.
  	nxtReadRawHS(BytesRead, 1);      // Read the array.

		string sTemp = " ";
  	StringFormat(sTemp, "%c", BytesRead);
  	nxtDisplayTextLine(2, sTemp);

    vector1 = BytesRead;

	  //int readint = BytesRead[0]+10;
	  //string readintStr = ' ';
	  //StringFormat(readintStr, "%c", readint);
	  //nxtDisplayTextLine(2, readintStr);

	  return vector1;
	}


//The number of milliseconds to delay between checks of the
//receive buffer.
int MS_TO_DELAY = 270; // 100;

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

    int null_catch = 1;
    int power = 0;
    int steer = 0;
    // Change Power and Direction
		for (int i = 0; i<3; i++)
		{
		  ReadResponse(null_catch);
      if(null_catch == 0) {                   // If we catch the null, read the next three things.
        ReadResponse(power);
        ReadResponse(steer);
      }
      else{
        i = 3;
        null_catch = 1;
      }                         // If we don't catch the null, just restart and look for the null.

	  }
	}
}
