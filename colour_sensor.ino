
// to be called at setup
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

void readColourSensor()
{
  for (int c = 0; c < 3; c++) // from RED to BLUE
  {
    digitalWrite(D1, ledPins[c][0]);
    digitalWrite(D2, ledPins[c][1]);
    delay(RGBWait);

    currentColour[c] = getAvg();
    currentColour[c] = ((float) (currentColour[c] - balance[BLACK][c])) / (float) balance[GREY][c] * 255.0;
    
    // prevent negative values
    if (currentColour[c] < 0) currentColour[c] = 1;

    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    delay(RGBWait);
  }
  led.setColor(currentColour[0], currentColour[1], currentColour[2]);
  led.show();
  
  // runs the following code if DEBUG_COLOUR is #defined 
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
  Serial.print("GREY range: ");
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

char *Colour_calc(int red, int green, int blue) {
 if (red > 200 && green > 200 && blue > 200)
 {
  return "white";
 }

 if (red < 80 && green < 80 && blue < 80)
 {
  return "black";
 }
 if(red > green){

    if(green > 180){  // was 180

      if(green > blue){

        return "orange";
      }
    }
    if (blue > green) {

      return "purple";
    }

    return "red";

  }else if(blue > green){

      return "blue";

  }else{

    return "green";
  }
}

bool sense_black_strip() {

  int sensor_state = lineFinder.readSensors();
  return sensor_state == S1_IN_S2_IN;
}
