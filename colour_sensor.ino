
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

bool sense_black_strip() {

  int sensor_state = lineFinder.readSensors();
  return sensor_state == S1_IN_S2_IN;
}
