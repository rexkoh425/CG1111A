void turn_left(){
 
 right_motor.run(turning_speed + RIGHT_DEVIATION);
 left_motor.run(turning_speed - LEFT_DEVIATION);
 delay(RIGHT_ANGLE_MS);
 //left_motor.run(0);

}

void turn_right(){
 
 
 left_motor.run(-turning_speed + LEFT_DEVIATION);
 right_motor.run(-turning_speed - RIGHT_DEVIATION);
 delay(RIGHT_ANGLE_MS);
 //right_motor.run(0);

}

void u_turn(){
 
 
 left_motor.run(-turning_speed + LEFT_DEVIATION);
 right_motor.run(-turning_speed - RIGHT_DEVIATION);
 delay(U_TURN_MS);
 //right_motor.run(0);

}

void double_left_turn(){
  
  right_motor.run(turning_speed + RIGHT_DEVIATION);
  left_motor.run(turning_speed - LEFT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
  //left_motor.run(0);
  long current_time = millis();
  long updated_time = 0;

  while(millis() < DOUBLE_TURN_STRAIGHT_MS + current_time){
    move_forward(motor_deviation());
    delay(4);
  }

  //left_motor.run(0);
  right_motor.run(turning_speed + RIGHT_DEVIATION);
  left_motor.run(turning_speed - LEFT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
}

void double_right_turn(){
  
  left_motor.run(-turning_speed + LEFT_DEVIATION);
  right_motor.run(-turning_speed - RIGHT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
  //right_motor.run(0);
  long current_time = millis();
  long updated_time = 0;

  while(millis() < DOUBLE_TURN_STRAIGHT_MS + current_time){
    move_forward(motor_deviation());
    delay(4);
  }

  //left_motor.run(0);
  left_motor.run(-turning_speed + LEFT_DEVIATION);
  right_motor.run(-turning_speed - RIGHT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
  //right_motor.run(0);
}