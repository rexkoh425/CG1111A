#include <EEPROM.h> // EEPROM used to hold values 
#include <MeMCore.h>

#define D1 A2 
#define D2 A3 

// time delay between RGB LEDs (to allow LDR to stabilise)
#define RGBWait 200 //milliseconds, 0.2 seconds [test lower values]

// time delay between LDR readings
#define LDRWait 10 //0.01 seconds [test lower values]

// how many LDR readings to take average from 
#define NUM_AVG_READS 5

// LDR sensor pin A0 
#define LDR 0

// countdown before taking calibration readings
#define COUNTDOWN 5

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
  { LOW,  LOW  }    // IR sensor
};

//holds RGB values before calculations
int currentColour[3] = {0, 0, 0};

// VALUES FOR BALANCE
// to save in EEPROM for comparison
int balance[3][3] =
{
  {0, 0, 0},  // White
  {0, 0, 0},  // Black
  {0, 0, 0}   // Grey
};

void setup() 
{
  // basic setup + serial monitor
  //Serial.begin(9600); // for serial monitor, remove during actual

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  // gets balance values (comparing to white and black)
  setBalance(); 

  // saves calibration values to mBot internal memory
  writeToEEPROM();

  Serial.println("Colour calibration complete :)");
}

// for checking if calibration is right
void loop() 
{
  for (int c = 0; c < 3; c++) // from RED to BLUE
  {
    // turns on LED
    digitalWrite(D1, ledPins[c][0]);
    digitalWrite(D2, ledPins[c][1]);
    delay(RGBWait);

    // gets LDR reading
    currentColour[c] = getLDRReading();
    // calculates colour value within 0 - 255 range
    currentColour[c] = (float) (currentColour[c] - balance[BLACK][c]) / (float) balance[GREY][c] * 255.0;

    // returns 1 if colour value read is negative
    if (currentColour[c] < 0) currentColour[c] = 1;

    // turns off LED
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

  int red = currentColour[0];
  int green = currentColour[1];
  int blue = currentColour[2];

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
      // turns on LED
      digitalWrite(D1, ledPins[i][0]);
      digitalWrite(D2, ledPins[i][1]);
      delay(RGBWait);

      // saves avg RGB value to balance array
      balance[c][i] = getLDRReading(); 

      // turns off LEDs
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      delay(RGBWait);

      // prints balance values for White and Black for checking
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

// saves calibration values to mBot internal memory
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

// helper function to get LDR average reading
int getLDRReading()
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

// colour matching function for testing
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
