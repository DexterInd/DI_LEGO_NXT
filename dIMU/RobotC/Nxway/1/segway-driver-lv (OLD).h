/* LEGO MINDSTORMS NXT Segway program driver

Author: Laurens Valk

License: Use and modify the driver and examples as you like, but give credit to the original author.
         If, for any reason, you want to use this code commercially, email me
         at laurensvalk _at_ gmail _dot_com.

Compatible Gyroscopes:
- HiTechnic Gyro (NGY1044)
- Dexter Industries 6DOF IMU (dIMU)
- ...

The task in this driver should run parallel to the main program task.
It will handle all balancing control and leaves the main task free for
custom user programs, allowing the Segway to move around and, for example
avoiding walls. The following examples make use of this driver:

 - Example #1: Segway-Wall.c (Includes description of commands to control Segway movement)
 - Example #2: Segway-Line.c

This Segway driver uses Sensor drivers (v2.1 or later) from the RobotC driver suite:
http://botbench.com/blog/robotc-driver-suite/


*/

#define dIMU_Gyro 1
#define HiTechnic_Gyro 0

int steering = 0;
int acceleration = 0;
int speed = 0;

task balancing()
{
  //MOTOR SETUP
  nMotorPIDSpeedCtrl[motorB] = mtrNoReg;
  nMotorPIDSpeedCtrl[motorC] = mtrNoReg;
  const float r = your_wheel_radius/0.042;

  //VARIABLE AND SENSOR SETUP, DIFFERENT FOR EACH GYRO TYPE
  float gw_dth_dt,gw_th,gw_y,gw_dy_dt,kp,ki,kd;

  if(your_gyro_sensor == 1){
    //GAINS AND PID CONSTANTS FOR DIMU
    gw_dth_dt = 0.4;
    gw_th = 50.00;
    gw_y = 0.40*r;
    gw_dy_dt = 1*r;
    kp = 0.4/r;  // originaly 0.4
    ki = 3.8/r;
    kd = 0.005/r;

    //SET UP DIMU
    SensorType[Gyro] = sensorI2CCustomFastSkipStates;
    wait1Msec(200);
    if(!DIMUconfigGyro(Gyro, DIMU_GYRO_RANGE_250)){     // ,false)){
      PlaySound(soundException);nxtDisplayTextLine(1,"dIMU Connected?");
      wait1Msec(2000);StopAllTasks();}
    wait1Msec(500);
  }
  else{
    //GAINS AND PID CONSTANTS FOR DIMU
    gw_dth_dt = 0.2;
    gw_th = 30.00;
    gw_y = 0.4*r;
    gw_dy_dt = 1.0*r;
    kp = 0.4/r;
    ki = 3.8/r;
    kd = 0.005/r;
    //SET UP HiTechnic Gyro
    SensorType[Gyro] = sensorRawValue;
  }

  //SETUP VARIABLES FOR CALCULATIONS
  float u = 0,                    // Sensor Measurement (raw)
        th = 0,//Theta            // Angle of robot (degree)
        dth_dt = 0;//dTheta/dt    // Angular velocity of robot (degree/sec)
  float e = 0,//Error             // Sum of four states to be kept zero: th, dth_dt, y, dy_dt.
        de_dt = 0,//dError/dt     // Change of above error
        _edt = 0,//Integral Error // Accumulated error in time
        e_prev = 0;//Previous Error/ Error found in previous loop cycle
  float pid = 0,                  // SUM OF PID CALCULATION
        dt = 0.005,//dt           // Length of each loop
        mean_reading = 0;         // Long time average of sensormeasurement
  float y,//y                     // Measured Motor position (degrees)
        dy_dt,//dy/dt             // Measured motor velocity (degrees/sec)
	      v = 0,//velocity          // Desired motor velocity (degrees/sec)
	      y_ref = 0;//reference pos // Desired motor position (degrees)
  int motorpower = 0,             // Power ultimately applied to motors
      last_steering = 0,          // Steering value in previous cycle
      straight = 0,               // Average motor position for synchronizing
      d_pwr = 0;                  // Change in power required for synchronizing
  const int poll_time_gyro = 2,   // Time required for one measurement (mSec)
            n_max = 7;            // Number of measurement used for floating motor speed average
  int n = 0,n_comp = 0,           // Intermediate variables needed to compute measured motor speed
  encoder[n_max];                 // Array containing last n_max motor positions

  while(true)
  {
    //COMPUTE GYRO ANGULAR VELOCITY AND ESTIMATE ANGLE
  	if(your_gyro_sensor == dIMU_Gyro){
	  	u =   -DIMUreadGyroAxis(Gyro, DIMU_GYRO_Z_AXIS);
	  	u = u -DIMUreadGyroAxis(Gyro, DIMU_GYRO_Z_AXIS);}
    else{
      u =   SensorRaw[Gyro];wait1Msec(poll_time_gyro);
      u = + SensorRaw[Gyro];wait1Msec(poll_time_gyro);}
  	dth_dt = u/2 - mean_reading;
  	mean_reading = mean_reading*0.999 + (0.001*dth_dt);
  	th = th + dth_dt*dt;

  	//COMPUTE MOTOR ENCODER POSITION AND SPEED
  	n++;if(n == n_max){n = 0;}
  	encoder[n] = nMotorEncoder[motorB] + nMotorEncoder[motorC] + y_ref;
  	n_comp = n+1;if(n_comp == n_max){n_comp = 0;}
  	int delta_enc = encoder[n] - encoder[n_comp];
  	y = encoder[n];
  	dy_dt = delta_enc;

  	//COMPUTE COMBINED ERROR AND PID VALUES
  	e = gw_th * th + gw_dth_dt * dth_dt + gw_y * y + gw_dy_dt * dy_dt;
  	de_dt = (e - e_prev)/dt;
  	_edt = _edt + e*dt;
  	e_prev = e;
  	pid = (kp*e + ki*_edt + kd*de_dt);

    //ADJUST REFERENCE POSITION ON SPEED AND ACCELERATION
    if(v < speed*10){
    v = v + acceleration*10*dt;}
    else if(v > speed*10){
    v = v - acceleration*10*dt;}
    y_ref = y_ref + v*dt;

  	//ADJUST MOTOR SPEED TO STEERING AND SYNCHING
    if(steering == 0){
        if(last_steering != 0){
	        straight = nMotorEncoder[motorC] - nMotorEncoder[motorB];}
		    d_pwr = (nMotorEncoder[motorC] - nMotorEncoder[motorB] - straight)/r;}
    else{d_pwr = steering/r;}
    last_steering = steering;

    //ERROR CHECKING OR SHUTDOWN
    if(abs(th)>60 || abs(motorpower) > 2000){StopAllTasks();}

  	//CONTROL MOTOR POWER AND STEERING
  	motorpower = 	pid;
    motor[motorB] = motorpower + d_pwr;
    motor[motorC] = motorpower - d_pwr;

    //WAIT THEN REPEAT
  	wait1Msec(dt*1000-poll_time_gyro*2);
  }
}
