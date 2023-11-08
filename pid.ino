
float get_ultrasonic_distance() {

  
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);

  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT); // Microseconds

  // m -> cm ( * 100) and microsecond -> second ( / 1000000)
  float conversion = 100.0 / 1000000.0;
  float distance_cm = duration * SPEED_OF_SOUND * 0.5 * conversion;
  
 
  return distance_cm;
}

int limit_correction(float error){

  if(error > 255 ){
    return 255;
  }
  if(error < -255){
    return -255;
  }
  return error;
}

float mod(float change_in_error){

  if(change_in_error < 0){
    return 0 - change_in_error;
  }
  return change_in_error;
}

int motor_deviation(){
  
  float dist_from_wall = filter_dist(get_ultrasonic_distance());
   
  float error = dist_from_wall - MID_POINT ;
  float change_in_error = error - previous_error;
  float total_error = (Kp*error +  Kd * (change_in_error))/factor;  // actual
  previous_error = error;

  if(dist_from_wall <= 0 || dist_from_wall > 14.0){

    previous_error = 0.0;
    return 0;
  }
  return limit_correction(total_error);//added this
}



void move_forward(int error) { 

  int left_correction = 0;
  int right_correction = 0;

  if(error > 0){
    right_correction = error;
  }else{
    left_correction = error;
  }
  
  left_motor.run(-MOVE_SPEED + LEFT_DEVIATION - left_correction);
  right_motor.run(MOVE_SPEED - RIGHT_DEVIATION - right_correction);
  
}




