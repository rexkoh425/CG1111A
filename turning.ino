/* Following are the functions to control the movement of the 
// mBot once it reaches a waypoint
*/
void turn_left(){
 
 right_motor.run(turning_speed + RIGHT_DEVIATION);
 left_motor.run(turning_speed - LEFT_DEVIATION);
 delay(LEFT_ANGLE_MS);
}

void turn_right(){
 
 left_motor.run(-turning_speed + LEFT_DEVIATION);
 right_motor.run(-turning_speed - RIGHT_DEVIATION);
 delay(RIGHT_ANGLE_MS);
}

void u_turn(){
 
 left_motor.run(turning_speed - LEFT_DEVIATION);
 right_motor.run(turning_speed + RIGHT_DEVIATION);
 delay(U_TURN_MS);
}


void double_left_turn(){
 
  // First turn
  right_motor.run(turning_speed + RIGHT_DEVIATION);
  left_motor.run(turning_speed - LEFT_DEVIATION);
  delay(LEFT_ANGLE_MS);

  // Move forward
  delay_pid(DOUBLE_TURN_STRAIGHT_LEFT_MS);
 
  // Second turn --> decelerate the left motor as it negotiates the turn 
  for (int i = turning_speed; i >= -turning_speed/1.5; i -= 10)
  {
  right_motor.run(turning_speed + RIGHT_DEVIATION);
  left_motor.run(-i - LEFT_DEVIATION);
  delay(17);
  }
}

void double_right_turn(){

  //First turn
  left_motor.run(-turning_speed + LEFT_DEVIATION);
  right_motor.run(-turning_speed - RIGHT_DEVIATION);
  delay(RIGHT_ANGLE_MS);

  // Move forward
  delay_pid(DOUBLE_TURN_STRAIGHT_RIGHT_MS);
 
  // Second turn --> decelerate the right motor as it negotiates the turn
  for (int i = turning_speed; i >= -turning_speed/1.5; i -= 10)
  {
  left_motor.run(-turning_speed + LEFT_DEVIATION);
  right_motor.run(i - RIGHT_DEVIATION);
  delay(15);
  }
}

void stop()
{
  left_motor.run(0);
  right_motor.run(0);
}

/* Function to execute corresponding movement action corresponding the colour detected by the colour sensor */
void new_turn_function()
{
  //currentColour is a global array which hold the R,G,B value of the sensed colour while Colour_calc outputs the colour base on the R,G,B values
  char *colour = Colour_calc(currentColour[0],currentColour[1],currentColour[2]);
  
  if (colour == "red"){
    
   turn_left();

  }else if (colour == "green") {

   turn_right();

  } else if (colour == "orange") {

   u_turn();

  } else if (colour == "purple") {
  
   double_left_turn();

  } else if (colour == "blue") {  

   double_right_turn();

  } else if (colour == "white") {

   stop();
   playMissionImpossible();
   delay(10000);
   
  }
}
