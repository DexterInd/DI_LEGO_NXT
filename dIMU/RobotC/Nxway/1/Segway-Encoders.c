/*******INFO*******

LEGO MINDSTORMS NXT Segway program Example

Example #3: Segway-Encoders.c Rotation sensor program for a LEGO NXT Segway

Author: Laurens Valk (laurensvalk.com)

License: Use the examples as you like, but give credit to the original author.
         See driver file for more details.

Requirements - Hardware:
- Assembled Segway with LEGO MINDSTORMS NXT parts: http://www.laurensvalk.com
- LEGO MINDSTORMS NXT compatible Gyroscope (see segway-driver-lv.h for details)

Requirements - Software:
- RobotC: http://robotc.net/
- RobotC Driver Suite version 2.3 or later: http://botbench.com/blog/robotc-driver-suite/
- segway-driver-lv.h: Included in this package, or find it at http://www.laurensvalk.com

Version:
- 1.0

***************/


//Adjust these lines to match your Segway Setup

//Choose 1 for Dexter Industries Gyro (dIMU) or 0 for HiTechnic Gyro
const int your_gyro_sensor = 1;

//Choose gyro sensor port (S1, S2, S3, S4)
const tSensors Gyro = S2;

//Choose the radius of your wheel. That's half the diameter, measured in meters!
//Choose 0.021 for NXT2.0 wheels; 0.027 for NXT1.0 wheels;
const float your_wheel_radius = 0.021;
///////

//If you're not using the dIMU, you may remove this #include line
//Change the location of the driver file below if you installed your drivers elsewhere!
#include "rdpartyrobotcdr-v2.1/drivers/DIMU-driver.h"
#include "segway-driver-lv.h"

task main()
{
  StartTask(balancing);
  while(starting_balancing_task){}

  while(true)
  {
	  speed = 30;
	  while(nMotorEncoder[motorB] > -1000){wait1Msec(100);}
	  //#1: Polling 10 times/sec is good enough -- Save processor power for the balancing task
	  //#2: Going forwards means that the motors run backwards, because of their orientation.
	  //    So to go forward for 1000 degrees, the encoder must reach -1000 degrees.
	  speed = -60;
	  while(nMotorEncoder[motorB] < 0){wait1Msec(100);}
  }

}
