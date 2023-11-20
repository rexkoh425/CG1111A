#include <MeEEPROM.h>
#include <MeMCore.h>

//colour sensor values / initialisation
#define WHITE 0
#define BLACK 1
#define GREY 2

#define RGBWait 100 //wait time before the RGB LED to stabilise before reading from the LDR
#define LDRWait 10 // wait time between each time reading the LDR
#define NUM_AVG_READS 5 // number of reading taken for each R ,G and B
#define COUNTDOWN 5 // use in calibration

#define LDR 0  // LDR Pin

#define D1 A2 // 2-to-4 decoder 1A select input
#define D2 A3 // 2-to-4 decoder 1B select input

//#define DEBUG_COLOUR

int balance[3][3] =
{
  {0, 0, 0},  // White
  {0, 0, 0},  // Black
  {0, 0, 0}   // Grey
};

int currentColour[3] = {0, 0, 0};

int ledPins[4][2] =
{
  { HIGH, HIGH },   // Red LED
  { LOW,  HIGH },   // Green LED
  { HIGH, LOW  },   // Blue LED
  { LOW,  LOW  }    // NIL (by right for IR sensor) (used as OFF)
};

MeRGBLed led(0,30);
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
//IR Sensor 
#define IRPin 1
#define IRWait 10
//
//turning
MeLineFollower lineFinder(PORT_2); // assigning lineFinder to RJ25 port 2
MeDCMotor left_motor(M1);  // Left motor port
MeDCMotor right_motor(M2); // Right motor port

/* hard-coded values for turning */
#define turning_speed 255 //turning speed
const long unsigned int LEFT_ANGLE_MS = 350; //Duration to turn left 90 deg 
const long unsigned int RIGHT_ANGLE_MS = 300; //Duration to turn right 90 deg
const long unsigned int U_TURN_MS = 585; //Duration to turn 180 deg
const long unsigned int DOUBLE_TURN_STRAIGHT_LEFT_MS = 425; //Duration to move straight for double left turn
const long unsigned int DOUBLE_TURN_STRAIGHT_RIGHT_MS = 355; //Duration to move straight for double right turn
//
//PID
#define TIMEOUT 2000         // Ultrasonic sensor will return 0 if nothing is detected after this amount of time    
#define SPEED_OF_SOUND 340   // Speed of sound used in calculating distance using time
#define ULTRASONIC 12        // Ultrasonic sensor port

#define MOVE_SPEED 255       // Speed at which the motors will be running when moving forward
#define LEFT_DEVIATION 2     // Left motor is slowed down by 2 as robot moves towards the right when set to the same speed in code
#define RIGHT_DEVIATION 0    // Right motor remains the same

const float Kp = 20.0;       // Proportionality Constant for P term              
const float Kd = 700.0;      // Proportionality Constant for D term 
const float MID_POINT = 7.0; // Distance from the ultrasonic sensor to the wall when the robot in the center
float previous_error = 0.0;
float previous_filtered_dist = 0.0;

const int loop_time_ms = 5;  // Time taken in ms for 1 loop which includes PID + 4ms delay
//

//music
MeBuzzer buzzer;


int verse1[] = {785, 785, 932, 1047, 785, 785, 698, 740};
int verse2[] = {523, 523, 622, 698, 523, 523, 466, 494};
int verse3[] = {932, 785, 587, 932, 785, 554, 587, 785, 523, 466, 523};
int verse4[] = {622, 523, 785, 622, 523, 740, 622, 523, 698, 622, 698};

int beat = 80;

int v1_durations[] = {3 * beat, 3 * beat, 2 * beat, 2 * beat, 3 * beat, 3 * beat, 2 * beat, 2 * beat};
int v3_durations_54[] = {beat, beat, 8 * beat, beat, beat, 8 * beat, beat, beat, 8 * beat, beat, beat};
int v3_durations_44[] = {beat, beat, 14 * beat, beat, beat, 14 * beat, beat, beat, 14 * beat, beat, beat};
int delay_54 = 20 * beat;//was 20
int delay_44 = 16 * beat;
//

void setup() {

  //Serial.begin(9600);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  
  led.setpin(13);
  readFromEEPROM();
  delay(4000);

}

void loop() {

  move_forward(motor_deviation()); //PID algorithm --> move straight by applying the necessary corrections
    
  if(sense_black_strip()){ // Returns true if line sensor senses the black strip
    delay(5);              // Delay 5ms after sensing the black strip to move forward a little more thus the robot will align with the PID better after turning
    stop();                // Sets motor speed to 0
    readColourSensor();    // Reads paper colour and inputs the colour read into a global array of R,G,B
    new_turn_function();   // Reads the R,G,B array and turns according to the colour
    reset();               // Resets previous_error and previous_filtered_dist
  }
  delay(4); 
}
