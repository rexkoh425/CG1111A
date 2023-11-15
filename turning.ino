void turn(char* direction){

  if (direction == "left") {
    for (int turn_speed = 0; turn_speed <= 210; turn_speed += 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(2);
    }
    for (int turn_speed = 210; turn_speed >= 0; turn_speed -= 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(2);
    }
  }
  else if(direction == "right"){ // Test 205
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
    for (int turn_speed = 0; turn_speed <= 205; turn_speed += 1)
    {
    left_motor.run(turn_speed);
    right_motor.run(turn_speed);
    delay(4);
    }
    for (int turn_speed = 205; turn_speed >= 0; turn_speed -= 1)
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
	for (int turn_speed = 0; turn_speed <= 240; turn_speed += 3)
	{
	    left_motor.run(-turn_speed);
	    right_motor.run(turn_speed);
	    delay(4);
	}
	left_motor.run(-240);
	right_motor.run(240);
	delay(500);
	for (int turn_speed = 240; turn_speed >= 0; turn_speed -= 3)
	{
	    left_motor.run(-turn_speed);
	    right_motor.run(turn_speed);
	    delay(4);
	}
	turn("left");

  } else if (colour == "blue") {  // Test 230 for blue 

	turn("right");
	for (int turn_speed = 0; turn_speed <= 230; turn_speed += 3)
	{
	    left_motor.run(-turn_speed);
	    right_motor.run(turn_speed);
	    delay(4);
	}
	left_motor.run(-230);
	right_motor.run(230);
	delay(500);
	for (int turn_speed = 230; turn_speed >= 0; turn_speed -= 3)
	{
	    left_motor.run(-turn_speed);
	    right_motor.run(turn_speed);
	    delay(4);
	}
	turn("right");

  } else if (colour == "white") {
  stop();
	playMissionImpossible();
	delay(10000);
  }
}


