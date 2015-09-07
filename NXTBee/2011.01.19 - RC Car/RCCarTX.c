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
int MS_TO_DELAY = 100;

//The current byte to write.  This will be incremented everytime
//the method NextByte is called, until 255 is reached.  Then, it
//will roll back to zero.
byte currByte = 0;

///Task: Reset control motor encoders.
void resetMotors(){
  nMotorEncoder[motorA] = 0;
  nMotorEncoder[motorC] = 0;
}

///Task: Read speed input
int speed(){
  int speed_input = nMotorEncoder[motorC];
  // Cleanup data
  if (speed_input < 0){
    nMotorEncoder[motorC] = 0;
    speed_input = 0;
  }
  return speed_input;
}

int direction(){
  int direction_input = nMotorEncoder[motorA]*2;
  // Cleanup data; ensure that you're never over 90 and under -90;
  if (direction_input < -90) direction_input = -90;
  if (direction_input > 90) direction_input = 90;
  direction_input = direction_input + 90;
  return direction_input;
}

bool forward(){
  bool FR = true;
  if(nNxtButtonPressed == 2) FR = false;
  return FR;
}

///Task: Display the input data on the screen
void display_data(){
	  string speed_input_string, direction_input_string;
	  StringFormat(speed_input_string, "%i", speed());
	  StringFormat(direction_input_string, "%i", direction());
	  nxtDisplayTextLine(2, speed_input_string);
	  nxtDisplayTextLine(4, direction_input_string);
}

////////////////////////////////////////////////////////////////////////
//                         Main Task
////////////////////////////////////////////////////////////////////////
task main()
{
	eraseDisplay();
	bNxtLCDStatusDisplay = true;
  setupHighSpeedLink();
  ubyte testBytes[3];                 //This is the buffer that will hold the bytes to transmit.
  int xmitCount = 1;

  resetMotors();

  //The test will continue until an error occurs or
  //the enter button on the brick is pressed.
  while(nNxtButtonPressed != kEnterButton)
	{
	  eraseDisplay();

    //Set the bytes of the buffer to the next byte; send the bytes.
    testBytes[0] = speed();
    testBytes[1] = direction();
    if(forward()){
      testBytes[2] = 0; // Car going forward.
    }
    else{
      testBytes[2] = 1; // Car going backward.
    }
    int retVal = TransmitBytes(testBytes, sizeof(testBytes));

	  //StringFormat(tStr0, "%3d", testBytes[0]);

    display_data();         // Displays pertinent data.
	  //string TempStr = tStr0;
	  //nxtDisplayTextLine(2, TempStr);

    //Delay.  Possibly, this could be used for an ACK.
    wait1Msec(MS_TO_DELAY);
	}
}
