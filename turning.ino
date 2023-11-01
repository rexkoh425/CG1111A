
void turn(char* direction){

  if (direction == "left") {
    for (int turn_speed = 0; turn_speed <= 205; turn_speed += 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(2);
    }
    for (int turn_speed = 205; turn_speed >= 0; turn_speed -= 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(2);
    }
  }
  else if(direction == "right"){
    for (int turn_speed = 0; turn_speed <= 205; turn_speed += 1)
    {
    left_motor.run(-turn_speed);
    right_motor.run(-turn_speed);
    delay(2);
    }
    for (int turn_speed = 205; turn_speed >= 0; turn_speed -= 1)
    {
    left_motor.run(-turn_speed);
    right_motor.run(-turn_speed);
    delay(2);
    }
  }else {//u turn
    for (int turn_speed = 0; turn_speed <= 210; turn_speed += 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(4);
    }
    for (int turn_speed = 210; turn_speed >= 0; turn_speed -= 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(4);
    }
  }
}

void stop()
{
	left_motor.run(0);
  right_motor.run(0);
}

void turn_function()
{
  char *colour = Colour_calc(currentColour[0],currentColour[1],currentColour[2]);
  
  if (colour == "red")
  {

	turn("left");

  } else if (colour == "green") {

	turn("right");

  } else if (colour == "orange") {

	turn(180);

  } else if (colour == "purple") {

  
	turn("left");
	left_motor.run(-MOVE_SPEED);
  right_motor.run(MOVE_SPEED);
	delay(2000);
	turn("left");
   

  } else if (colour == "light blue") {

    
	turn("right");
	left_motor.run(MOVE_SPEED);
  right_motor.run(MOVE_SPEED);
	delay(2000);
	turn("right");
  

  } else {

  stop();
	//play_victory_tune();
	delay(10000);
  
  }

  delay(1000);//stop after turn 
}



