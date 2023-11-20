
float get_ultrasonic_distance() {

  pinMode(ULTRASONIC, OUTPUT);
  
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);

  pinMode(ULTRASONIC, INPUT);
  
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT); // in microseconds

  float conversion = 100.0 / 1000000.0;  //1 meter = 100 cm ; 1 second = 1000000 microseconds
  float distance_cm = duration * SPEED_OF_SOUND * 0.5 * conversion;
  
  return distance_cm;
}

/* limits the values returned by the PID to between -255 & 255 */
int limit_correction(float error){

  if(error > 255 ){
    return 255;
  }
  if(error < -255){
    return -255;
  }
  return error;
}

/* PID calculation */
int motor_deviation(){
  
  float dist_from_wall = filter_dist(get_ultrasonic_distance()); //smoothens the input from the ultrasonic sensor

  float error = dist_from_wall - MID_POINT ; //distance of robot from the center
  float change_in_error = error - previous_error; 
  float total_error = Kp * error +  Kd * (change_in_error); //calculates the correction to be output to the motor according to kp and kd values
  previous_error = error;

  //if wall is not present , return 0 --> no correction is applied
  if(dist_from_wall <= 0 || dist_from_wall > 15.0){

    previous_error = 0.0;
    return 0;
  }
  return limit_correction(total_error);
}


/* decides which wheel to be adjusted according to the correction needed */
void move_forward(int error) { 

  int left_correction = 0;
  int right_correction = 0;

  //error > 0 means robot is too far left , thus the correction will be applied to the right motor so right motor slows down and robot will turn right.
  if(error > 0){
    right_correction = error;
  }else{
    left_correction = error;
  }
  
  //innate motor deviations are applied on top of pid corrections as it should move straight at midpoint where correction is 0
  left_motor.run(-MOVE_SPEED + LEFT_DEVIATION - left_correction);
  right_motor.run(MOVE_SPEED - RIGHT_DEVIATION - right_correction);
  
}

/* low-pass filter that smoothens the PID inputs */
float filter_dist(float dist){

  float filtered_dist = 0 ;
  float ratio = 0.05; //how much of the current dist will be considered
  
  if(abs(dist - previous_filtered_dist) > 2.0){   //account for the sudden change when there is no wall or large changes. Raw value make it responsive to current changes.  

    filtered_dist = dist;

  }else{ 

    filtered_dist = (1-ratio) * previous_filtered_dist + ratio * dist; // smoothen input
  
  } 

  previous_filtered_dist = filtered_dist;
  return filtered_dist;
}

/* reset values after each turn as PID values left from before the turn should not be used after the turn */
void reset(){

  previous_error = 0.0 ;
  previous_filtered_dist = 0.0;
  
}

/* a function that apply PID for a certain period of time and also acts as a delay */
void delay_pid(int delay_ms){

  int loop_num = delay_ms / loop_time_ms ; // loop_time is the time required for 1 loop

  for(int i = 1 ; i <= loop_num ; i += 1){ //looping 4 times would be eqivalent to a 20ms delay
    move_forward(motor_deviation());
    delay(4);
  }

}


