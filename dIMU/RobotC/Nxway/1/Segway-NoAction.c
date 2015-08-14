/*******INFO*******

Example #2: Segway-NoAction.c Sample Program for a LEGO NXT Segway
Author: Laurens Valk (laurensvalk.com)
License: Use the examples as you like, but give credit to the original author.
         See driver file for more details.

***************/

//Choose 1 for Dexter Industries Gyro (dIMU) or 0 for HiTechnic Gyro
const int your_gyro_sensor = 1;
const tSensors Gyro = S2;
const float your_wheel_radius = 0.021;

#include "rdpartyrobotcdr-v2.1/drivers/DIMU-driver.h"
#include "segway-driver-lv.h"

task main()
{
  StartTask(balancing);
  while(starting_balancing_task){}

  //If you take no action here, the robot will just stay in one position.

  //Modify the following variables to make the Segway move.
  //speed = 50; steering = -7;
  //See other examples, particularly "Segway-Explanation(Wall-Avoidance).c", for more information.
}
