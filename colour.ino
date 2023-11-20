#define ORANGE_GREEN_THRESHOLD 180 // to differentiate between orange and red
#define RED_BLUE_RATIO 0.85 // to differentiate between purple and blue

// to be called at setup
// retrieves White, Black and Grey values from internal memory of mBot for calibration
void readFromEEPROM()
{
  int eeAddress = 0;

  Serial.println("Retrieving balance values from EEPROM...");
  for (int a = 0; a < 3; a++) // from WHITE to GREY
  {
    for (int b = 0; b < 3; b++) // from RED to BLUE
    {
      EEPROM.get(eeAddress, balance[a][b]);
      eeAddress += sizeof(int);

      Serial.print(balance[a][b]);
      
      if (b < 2) // aesthetics
      {
        Serial.print(", ");
      }
    }
    Serial.println("\n");
  }

  // checking if reading/writing was successful
  char check1 = EEPROM.read(eeAddress);
  char check2 = EEPROM.read(eeAddress + 1);
  char check3 = EEPROM.read(eeAddress + 2);

  if (check1 != 'J' || check2 != 'B' || check3 != 'M')
  {
    Serial.println("Unable to retrieve value from EEPROM :( Run calibration again!");
  }
}

// turns on LED, gets LDR readings, converts to 0-255 range, and turns off LED
// repeats 2 more times to retrieve colour values for R, G and B
void readColourSensor()
{
  for (int c = 0; c < 3; c++) // from RED to BLUE
  {
    // turn on LED
    digitalWrite(D1, ledPins[c][0]);
    digitalWrite(D2, ledPins[c][1]);
    delay(RGBWait);

    currentColour[c] = getLDRReadings(); // gets reading from LDR
    currentColour[c] = ((float) (currentColour[c] - balance[BLACK][c])) / (float) balance[GREY][c] * 255.0; // converts reading to value in 0 - 255 range
    
    // prevent negative values
    if (currentColour[c] < 0) currentColour[c] = 1;

    // turns LED off
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
  }
  
  // runs the following code if DEBUG_COLOUR is #defined 
  // prints RGB values 
  #ifndef DEBUG_COLOUR
    Serial.print("Current colour ");
    for (int i = 0; i < 3; i++)
    {
      Serial.print(currentColour[i]);
      Serial.print(" ");
    }
    Serial.println("");
  #endif
}

// reads values from LDR
// called in readColourSensor()
int getLDRReadings()
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

// takes RGB values as input and returns colour matched as String
// flashes colour sensed on mBot in-built LEDs
char *Colour_calc(int red, int green, int blue){
 
  if (red > 210 && green > 210 && blue > 210){
  
   led.setColor(255, 255, 255);
   led.show();
   return "white";
 }

 if (red < 80 && green < 80 && blue < 80){
  
   return "black";
 }

 if(red > green && red > 220){

  if(green > ORANGE_GREEN_THRESHOLD)
  {
   // red > 220, green > ORANGE_GREEN_THRESHOLD
    led.setColor(255, 165, 0);
    led.show();
    led.setColor(0, 0, 0);
    led.show();
    return "orange";
  }

  // red > 220, green < ORANGE_GREEN_THRESHOLD
  led.setColor(255, 0, 0);
  led.show();
  led.setColor(0, 0, 0);
  led.show();
  return "red";
 }

  if(blue > green){
  
    float ratio = (float) red / (float) blue;
    if (ratio > RED_BLUE_RATIO)
    {
      led.setColor(255, 0, 255);
      led.show();
      led.setColor(0, 0, 0);
      led.show();
      return "purple";
    }
    led.setColor(0, 0, 255);
    led.show();
    led.setColor(0, 0, 0);
    led.show();
    return "blue";
  }

  // red < green && blue < green
  led.setColor(0, 255, 0);
  led.show();
  led.setColor(0, 0, 0);
  led.show();
  return "green";
}

// reads average readings from IR sensor
int getIRdistance()
{
  int total = 0;
  for (int i = 0; i < NUM_AVG_READS; i++)
  {
    total += analogRead(IRPin);
    delay(IRWait);
  }
  return total / NUM_AVG_READS;
}

// senses if mBot has reached black strip
bool sense_black_strip() {

  int sensor_state = lineFinder.readSensors();
  return sensor_state == S1_IN_S2_IN;
}
