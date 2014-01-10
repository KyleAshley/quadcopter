quadcopter
==========

These files are device drivers for the HC9S12 microprocessor alongside:
  Sharp Long Range Distance Sensor
  GY-80 10 DOF IMU

The goal is to implement a flight control system for use in a Quadcopter. The hardware chosen for the physical flight system is variable, but in general si designed to include:
  4 500-1000kv Brushless DC Motors
  4 PWM controlled ESC's
  
The general layout of the flight control system is as follows.

Flight Control files are responsible for communcation to ESC's, dictating throttle response based on am instance of navigation parameters. 

Navigation files are responsible for updating sensor readings corresponding to pitch, roll, altitude and heading. Navigation serves as a central hub for all sensor communcation. These readings are requested by Flight Control on a regular basis and appropiate action is taken based on the desired function ie: Hover, Forward Flight etc.

Files not associated with Flight Control, Navigation, or Sensors are Microprocessor specific drivers for:
  iic communcation
  timing
  A2D protocol
  lcd driver
  etc.
  


