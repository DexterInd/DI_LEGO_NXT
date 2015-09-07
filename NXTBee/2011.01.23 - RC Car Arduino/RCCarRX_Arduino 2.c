///----------------------------------------------------------------------------
/// Crosstalk_receive.c -  Crosstalk receive test.
///
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
///----------------------------------------------------------------------------

#pragma platform(NXT)
int power = 0;    // Global variable for power.
int steer = 0;    // Global variable for steering.
int direc = 0;    // Global variable for direciton.

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
void ReadResponse()
{
    byte BytesRead[5];                  // Array we'll be reading into.
  	nxtReadRawHS(BytesRead[0], 5);             // Read the array.

    if(BytesRead[0] == '/') {                   // If we catch the null, read the next three things.
      power = BytesRead[1];                     //
      steer = BytesRead[2];                     // -90 is left, 90 is right
      direc = BytesRead[3];                     // "0" is forward, "1" is reverse.
    }
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
  wait1Msec(MS_TO_DELAY);

	//Setup the link to be a high-speed link.
  setupHighSpeedLink();

  bFloatDuringInactiveMotorPWM = true;     // The motors WILL coast when power is not appli
  int motorpower = 0;                      // Variable holds the power.

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{

    wait1Msec(MS_TO_DELAY);   // Put in place to not overload the system.
    ReadResponse();           // Check the steering, power, and direction.

    // Direction and power.
    if(direc == 0){
        motorpower = power;
    }
    else{
       motorpower = power*-1;
    }
    motor[motorA] = motorpower;
    motor[motorC] = motorpower;

    // Change Course
    int steer_posn = steer;  // Steer Posn is going to be between -90 and 90
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
