#include <MeEEPROM.h>
#include <MeMCore.h>

//colour sensor
#define WHITE 0
#define BLACK 1
#define GREY 2

#define RGBWait 200
#define LDRWait 10
#define NUM_AVG_READS 5

#define LDR 0

#define D1 A2 
#define D2 A3

#define DEBUG_COLOUR

int balance[3][3] =
{
  {1008, 1007, 1000},  // White
  {996, 954, 944},  // Black
  {12, 53, 56}   // Grey
};

int currentColour[3] = {0, 0, 0};

int ledPins[4][2] =
{
  { HIGH, HIGH },   // Red LED
  { LOW,  HIGH },   // Green LED
  { HIGH, LOW  },   // Blue LED
  { LOW,  LOW  }    // NIL (by right for IR sensor) (used as OFF)
};
//
//colour compare
#define white_limit 200 
#define red_limit 125
#define blue_limit 125
#define green_limit 125

int red = 255;
int green = 0;
int blue = 0;
//
//turning
MeLineFollower lineFinder(PORT_2); // assigning lineFinder to RJ25 port 2
MeDCMotor left_motor(M1);
MeDCMotor right_motor(M2);
//
//PID
#define TIMEOUT 2000   //to adjust timeout     
#define SPEED_OF_SOUND 340  
#define ULTRASONIC 12

#define MOVE_SPEED 255
#define LEFT_DEVIATION 2
#define RIGHT_DEVIATION 0 

const float Kp = 35.0;               
const float Kd = 100.0;
const float MID_POINT = 11.0;
float previous_error = 0.0 ;
//

void setup() {

  Serial.begin(9600);
  readFromEEPROM();
  delay(2000);

}

void loop() {
  
  //move_forward(motor_deviation());
/*
  if(sense_black_strip() == true){
    stop();
    //readColourSensor();
    //turn_function();
    turn("uturn");
  }
*/
  readColourSensor();
  Serial.println(Colour_calc(currentColour[0] ,currentColour[1] ,currentColour[2]));
}
