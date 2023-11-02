#define white_limit 200 
#define red_limit 125
#define blue_limit 125
#define green_limit 125

int red;
int green;
int blue;

#include <EEPROM.h> //EEPROM used to hold values 
#include <MeMCore.h>

#define D1 A2 
#define D2 A3 

//time delay between RGB LEDs (to allow LDR to stabilise)
#define RGBWait 200 //milliseconds, 0.2 seconds [test lower values]

//time delay between LDR readings
#define LDRWait 10 //0.01 seconds [test lower values]

#define COUNTDOWN 5

#define LDR 0 //LDR sensor pin A0 [check with mBot]
#define LED 13 //NOT SURE

// defining balance
#define WHITE 0
#define BLACK 1
#define GREY 2

//digitalWrite for individual LEDs
int ledPins[4][2] =
{
  { HIGH, HIGH },   // Red LED
  { LOW,  HIGH },   // Green LED
  { HIGH, LOW  },   // Blue LED
  { LOW,  LOW  }    // NIL (by right for IR sensor) (used as OFF)
};

//holds RGB values before calculations
int currentColour[3] = {0, 0, 0};

int finalColours[5][3] =
{
  { 0, 0, 0 }, // RED
  { 0, 0, 0 }, // GREEN
  { 0, 0, 0 }, // ORANGE
  { 0, 0, 0 }, // PURPLE
  { 0, 0, 0 }  // BLUE
};

// VALUES FOR BALANCE
// to save in EEPROM for comparison
// note: need to call READ function during overall setup
int balance[3][3] =
{
  {0, 0, 0},  // White
  {0, 0, 0},  // Black
  {0, 0, 0}   // Grey
};

#define COUNTDOWN 5// for placing white and black samples
#define NUM_AVG_READS 5


void setup() 
{
  // basic setup + serial monitor
  //Serial.begin(9600); // for serial monitor, remove during actual

  //pinMode(LDR, INPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  // gets balance values (comparing to white and black)
  setBalance(); // function definition below

  writeToEEPROM(); // function definition below, saves balance values

  Serial.println("Colour calibration complete :)");
}

// for checking if calibration is right
void loop() 
{
  for (int c = 0; c < 3; c++) // from RED to BLUE
  {
    digitalWrite(D1, ledPins[c][0]);
    digitalWrite(D2, ledPins[c][1]);
    delay(RGBWait);

    currentColour[c] = getAvg();
    currentColour[c] = (float) (currentColour[c] - balance[BLACK][c]) / (float) balance[GREY][c] * 255.0;

    if (currentColour[c] < 0) currentColour[c] = 1;

    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    delay(RGBWait);
  }

  // print current colour RGB values
  Serial.print("Current color: ");
  String RGB[3] = { "R = ", "G = ", "B = " };
  for (int c = 0; c < 3; c++) // from RED to BLUE
  {
    Serial.print(RGB[c]);
    Serial.print(currentColour[c]);
    
    if (c < 2)
    {
      Serial.print(", ");
    }
  }
  Serial.println("\n");

  red = currentColour[0];
  green = currentColour[1];
  blue = currentColour[2];

  Serial.println(Colour_calc(red, green, blue));
}

void setBalance()
{
  String title[2] = { "White", "Black" };

  for (int c = WHITE; c <= BLACK; c++) // from white to black
  {
    // instructions for calibration, prints countdown by lines
    Serial.println("Put " + title[c] + " sample for calibration in: ");
    for (int i = COUNTDOWN; i > 0; i--)
    {
      Serial.println(i);
      delay(1000); 
    }
    Serial.println("");

    // getting average RGB values
    Serial.print(title[c] + ": ");
    for (int i = 0; i < 3; i++) // from RED to BLUE
    {
      digitalWrite(D1, ledPins[i][0]);
      digitalWrite(D2, ledPins[i][1]);
      delay(RGBWait);

      balance[c][i] = getAvg(); // saves avg RGB value to balance array

      // LEDs off
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      delay(RGBWait);

      Serial.print(balance[c][i]);

      if (i < 2) // aesthetics
      {
        Serial.print(", ");
      }
    }
    Serial.println("\n"); 
  }

  // finding GREY range (WHITE - BLACK)
  Serial.print("Grey range: ");
  for (int j = 0; j < 3; j++) // from RED to BLUE
  {
    balance[GREY][j] = balance[WHITE][j] - balance[BLACK][j];
    Serial.print(balance[GREY][j]);

    if (j < 2) // aesthetics
    {
      Serial.print(", ");
    }
  }
  Serial.println("\n");
}

// i think this is for getting ref values to base colour differentiation on 
void getColourRanges()
{
  String colour[5] = { "Red", "Green", "Orange", "Purple", "Blue" };
  for (int c = 0; c < 5; c++) // cycle through above colours
  {
    Serial.println("Place " + colour[c] + " sample for calibration in: ");
    for (int i = COUNTDOWN; i > 0; i--)
    {
      Serial.println(i);
      delay(1000);
    }
    
    String RGB[3] = { "R = ", "G = ", "B = " };
    Serial.print(colour[c] + ": ");
    for (int i = 0; i < 3; i++) // RGB values
    {
      digitalWrite(D1, ledPins[i][0]);
      digitalWrite(D2, ledPins[i][1]);
      delay(RGBWait);

      finalColours[c][i] = getAvg();
      finalColours[c][i] = (float) (finalColours[c][i] - balance[BLACK][i]) / (float) balance[GREY][i] * 255.0;

      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      delay(RGBWait);

      Serial.print(RGB[i]);
      Serial.print(finalColours[c][i]);
      
      if (i < 2)
      {
        Serial.print(", ");
      }
    }
    Serial.println("\n");
  }
}

void writeToEEPROM()
{
  int eeAddress = 0;

  Serial.println("Writing balance values to EEPROM...");
  for (int a = WHITE; a <= GREY; a++)
  {
    for (int b = 0; b < 3; b++) // from RED to BLUE
    {
      EEPROM.put(eeAddress, balance[a][b]);
      eeAddress += sizeof(int);
    }
  }

  // checking if reading/writing was successful (see readFromEEPROM in colour_sensor)
  EEPROM.put(eeAddress, 'J');
  EEPROM.put(eeAddress + 1, 'B');
  EEPROM.put(eeAddress + 2, 'M');
}

int getAvg()
{
  int total = 0;

  for (int i = 0; i < NUM_AVG_READS; i++)
  {
    int reading = analogRead(LDR);
    total += reading;
    delay(LDRWait);
  }

  return total / NUM_AVG_READS;
}

char *Colour_calc(int red, int green, int blue) {
 if (red > 230 && green > 230 && blue > 230)
 {
  return "white";
 }

 if (red < 50 && green < 50 && blue < 50)
 {
  return "black";
 }
 if(red > green){

    if(green > blue){

      if(green > 110 && blue > 110){

        return "orange";
      }
      return "red";

    }else{

      return "purple";
    }

  }else if(blue > green){

      return "blue";

  }else{

    return "green";
  }
}