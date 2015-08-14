/* LEGO MINDSTORMS NXT Segway program Example

 - Example #1: Segway-Wall.c (Includes description of commands to control Segway movement)

Author: Laurens Valk

License: Use the examples as you like, but give credit to the original author.

For more information about compatibility and use, please look in the driver file.
*/


//Adjust these lines to match your Segway Setup

//Choose 1 for Dexter Industries Gyro (dIMU) or 0 for HiTechnic Gyro
const int your_gyro_sensor = 1;

//Choose gyro sensor port (S1, S2, S3, S4)
const tSensors Gyro = S2;

//Choose the radius of your wheel. That's half the diameter, measured in meters!
const float your_wheel_radius = 0.021;
///////

//If you're not using the dIMU, you may remove this #include line
#include "C:\Program Files (x86)\Robotics Academy\3rd Party Drivers\rdpartyrobotcdr-v2.1\drivers\DIMU-driver.h"


#include "segway-driver-lv.h"

task main()
{
  StartTask(balancing);
  //It's that simple - now go program something cool below!


  /*Here's how to control the Segway.

  MOVING AROUND
  - Adjust <steering> to steer.
  	   steering = 0  ; //Go Straight
  	   steering = 10 ; //Turn Right
       steering = 17;  //Turn Right quicklier
  	   steering = -10; //Turn Left
       etc...

   - Adjust <speed> for moving forward and backwards.
       Its value is the motor power that the robot will attempt to maintain

  	   speed = 0;     //Stand still
  	   speed = -20 ; //Go backward at 20% of motor power
       speed = 50;   //Go forward at half of maximum motor power (recommended)
       etc...

    - Adjust <acceleration> to change how quickly your robot will reach a certain speed.

       acceleration = 60; means that if your current speed is -20 and you change it
       to 40, it will take 1 seconds to get to that speed. You don't need to change this
       in your program, just set it once. Setting <acceleration = 60;> is recommended.

  TIPS & TRICKS
  - Don't mess with motor B and C!
  - You can read the motor encoders like you would normally do. Just don't reset them.
  - Speed sets the motor speed - This means you go faster if you use bigger wheels.
   */



   //////////////////////////////////////////////
   //
   // EXAMPLE #1: Wall avoidance


   acceleration = 60;
   const tSensors Sonar = S4;
   SensorType[Sonar] = sensorSONAR;

   steering = 0;
   speed = 50;

   while(true)
   {
     /* if(SensorValue[Sonar] < 60)
     {
       speed = -50;
       wait1Msec(2000);
       speed = 0;
       wait1Msec(500);
       steering = 10;
       wait1Msec(1000);
       steering = 0;
       speed = 50;
     }
     wait1Msec(100);
     */
   }
}
