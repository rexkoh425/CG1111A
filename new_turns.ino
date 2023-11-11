
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
  /*
  //left_motor.run(0);
  long current_time = millis();

  while(millis() < DOUBLE_TURN_STRAIGHT_MS + current_time){
    move_forward(motor_deviation());
    delay(4);
  }

  //left_motor.run(0);
  */
  delay_pid(DOUBLE_TURN_STRAIGHT_MS);
  right_motor.run(turning_speed + RIGHT_DEVIATION);
  left_motor.run(turning_speed - LEFT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
}

void double_right_turn(){
  
  left_motor.run(-turning_speed + LEFT_DEVIATION);
  right_motor.run(-turning_speed - RIGHT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
  /*
  //right_motor.run(0);
  long current_time = millis();

  while(millis() < DOUBLE_TURN_STRAIGHT_MS + current_time){
    move_forward(motor_deviation());
    delay(4);
  }

  //left_motor.run(0);
  */
  delay_pid(DOUBLE_TURN_STRAIGHT_MS);
  left_motor.run(-turning_speed + LEFT_DEVIATION);
  right_motor.run(-turning_speed - RIGHT_DEVIATION);
  delay(RIGHT_ANGLE_MS);
  //right_motor.run(0);
}

void new_turn_function()
{
  char *colour = Colour_calc(currentColour[0],currentColour[1],currentColour[2]);
  
  if (colour == "red"){
    
   turn_left();

  }else if (colour == "green") {

	 turn_right();

  } else if (colour == "orange") {

	 u_turn();

  } else if (colour == "purple") {
	
   double_left_turn();

  } else if (colour == "blue") {  // Test 230 for blue 

	 double_right_turn();

  } else if (colour == "white") {

   stop();
	 playMissionImpossible();
	 delay(10000);
   
  }
}