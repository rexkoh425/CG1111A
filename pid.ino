
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
  float total_error = Kp * error +  Kd * (change_in_error);
  previous_error = error;

  if(dist_from_wall <= 0 || dist_from_wall > 15.0){

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

float filter_dist(float dist){

  float filtered_dist = 0 ;
  float ratio = 0.05;//current weightage
  
  if(abs(dist - previous_filtered_dist) > 2.0){//was2.5

    filtered_dist = dist;

  }else{

    filtered_dist = (1-ratio) * previous_filtered_dist + ratio * dist;
  
  } 

  previous_filtered_dist = filtered_dist;
  return filtered_dist;
  
}

void reset(){

  previous_error = 0.0 ;
  previous_filtered_dist = 0.0;
  
}

void delay_pid(int delay_ms){

  int loop_num = delay_ms / loop_time_ms ;

  for(int i = 1 ; i <= loop_num ; i += 1){
    move_forward(motor_deviation());
    delay(4);
  }

}


