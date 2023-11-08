#include <MeEEPROM.h>
#include <MeMCore.h>

//colour sensor
#define WHITE 0
#define BLACK 1
#define GREY 2

#define RGBWait 200
#define LDRWait 10
#define NUM_AVG_READS 5
#define COUNTDOWN 5

#define LDR 1

#define D1 A2 
#define D2 A3

#define DEBUG_COLOUR

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

#define MOVE_SPEED 250
#define LEFT_DEVIATION 2
#define RIGHT_DEVIATION 0 

const float Kp = 35.0;               
const float Kd = 100.0;//was 100
const float MID_POINT = 8.0;
float previous_error = 0.0 ;
float factor = 1;
//
//music
MeBuzzer buzzer;

// verse 1= {G5, G5, A#5,C6, G5, G5, F5, F#5} total 16
int verse1[] = {785, 785, 932, 1047, 785, 785, 698, 740};
//verse2= {C5, C5, D#5, F5, C5, C5, A#4, B4 } total 16
int verse2[] = {523, 523, 622, 698, 523, 523, 466, 494};
//verse3={A#5, G5, D5, A#5, G5, C#5, A#5, G5, C5, A#4, C5, 0, D#5, C5, G5, D#5, C5, F#5, D#5, C5, F5, D#5, F5 } total 44, 22 per iteration
int verse3[] = {932, 785, 587, 932, 785, 554, 587, 785, 523, 466, 523};
int verse4[] = {622, 523, 785, 622, 523, 740, 622, 523, 698, 622, 698};

int beat = 80;

int v1_durations[] = {3 * beat, 3 * beat, 2 * beat, 2 * beat, 3 * beat, 3 * beat, 2 * beat, 2 * beat};
int v3_durations_54[] = {beat, beat, 8 * beat, beat, beat, 8 * beat, beat, beat, 8 * beat, beat, beat};
int v3_durations_44[] = {beat, beat, 14 * beat, beat, beat, 14 * beat, beat, beat, 14 * beat, beat, beat};
int delay_54 = 20 * beat;//was 20
int delay_44 = 16 * beat;

//
bool colour_sense = true;

void setup() {

  Serial.begin(9600);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  //setBalance();
  readFromEEPROM();
  delay(4000);

}

void loop() {


if(!colour_sense){

readColourSensor();
  Serial.println(currentColour[0]);
   Serial.println(currentColour[1]);
    Serial.println(currentColour[2]);
    Serial.println(Colour_calc(currentColour[0],currentColour[1],currentColour[2]));
  //playMissionImpossible();
  //delay(10000);

}else{

  move_forward(motor_deviation());
  //motor_deviation();
  if(sense_black_strip() == true){
    stop();
    readColourSensor();
    turn_function();
  }

}

}
