// to be called at setup
void readFromEEPROM()
{
  int eeAddress = 0;

  Serial.println("Retrieving balance values from EEPROM...");
  for (int a = 0; a < 2; a++) // GREY and BLACK
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
    currentColour[c] = get255Colour[c];
    if (currentColour[c] < 0) currentColour = 1;
  }
  led.setColor(currentColour[0], currentColour[1], currentColour[2]);
  led.show();
  delay(1000);

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

int get255Colour(int RGB)
{
  int colour = getRawColour(RGB);
  colour = ((float)colour - (float) balanceArray[1][RGB]) / (float) balanceArray[0][RGB] * 255.0;
  return colour;
}

int getRawColour(int RGB)
{
  digitalWrite(D1, ledPins[RGB][0]);
  digitalWrite(D2, ledPins[RGB][1]);
  delay(RGBWait);

  int colourValue = getLDRReading();

  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);

  return colourValue;
}

int getLDRReading()
{
  int total = 0;
  for (int i = NUM_READS; i > 0; i--)
  {
    total += analogRead(LDR);
    delay(LDRWait);
  }
  return total / NUM_READS;
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