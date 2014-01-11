quadcopter
==========

These files are device drivers for the HC9S12 microprocessor alongside:
  Sharp Long Range Distance Sensor
  GY-80 10 DOF IMU

The goal is to implement a flight control system for use in a Quadcopter. However, framework has been created to allow for any number of motors. The hardware chosen for the physical flight system is variable, but in general is designed to include:
  4 500-1000kv Brushless DC Motors
  4 PWM controlled ESC's
  
The general layout of the flight control software is as follows.

Navigation files are responsible for updating sensor readings corresponding to pitch, roll, altitude and heading. Navigation serves as a central hub for all sensor communcation, maintaining an instance of the most recent readings. These readings are requested by Flight Control on a regular basis and appropiate action is taken based on the desired function ie: Hover, Forward Flight etc.

Flight Control files are responsible for communcation to ESC's, dictating throttle response based on am instance of navigation parameters.

Flight Control has been implemented to incorporate a certain degree of machine learning. If the desired response from a course correction is not observed in succesive sensor readings, the 'gain' of future corrections in the same navigational instance are augmented to reflect a more appropriate response. Throttle response is also limited to maximum tolerances dictated by the chosen ESC and may be editted with the MAXDTY and MINDTY constants.

Course corrections are also effected by the desired threshold of response as seen by the current navigational instance. 
Ex: If you choose to only correct pitch within a degree of variance you may specify this by changing the PITCHTHRESH constant to 1.

Files not associated with Flight Control, Navigation, or Sensors are Microprocessor specific drivers for:
  iic communcation,
  timing,
  A2D,
  lcd driver,
  etc.
  


