#include <MeMCore.h>
#include <EEPROM.h>

MeRGBLed led(0,30);

#define D1 A2
#define D2 A3
#define LDR 0

#define LDRWait 10
#define RGBWait 200

//int RGBWaitArray[6] = {300, 200, 150, 100, 75, 50};

#define COUNTDOWN 5
#define NUM_READS 5

int ledPins[4][2] =
{
  { HIGH, HIGH },   // Red LED
  { LOW,  HIGH },   // Green LED
  { HIGH, LOW  },   // Blue LED
  { LOW,  LOW  }    // NIL (by right for IR sensor) (used as OFF)
};

int currentColour[3] = {0, 0, 0};

int balanceArray[2][3] =
{
  {0, 0, 0}, // white, then replace with grey
  {0, 0, 0}  // black
};

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  led.setpin(13);

  setBalanceValues();
  writeToEEPROM();

  Serial.println("Colour calibration complete :)");
  Serial.println("");
}

void loop() {
  String RGBArray[3] = {"R: ", "G: ", "B: "};
  for (int RGB = 0; RGB < 3; RGB++)
  {
    currentColour[RGB] = get255Colour(RGB);
    Serial.println(currentColour[RGB]);
  }
  Serial.println(Colour_calc(currentColour[0], currentColour[1], currentColour[2]));
  //Serial.println(Colour_calc(255, 255, 255));
}

void setBalanceValues()
{
  for (int i = 0; i < 2; i++)
  {
    String colour = "WHITE";
    if (i == 1)
    {
      colour = "BLACK";
    }
    
    Serial.println("Put " + colour + " sample for calibration in: ");
    for (int count = COUNTDOWN; count > 0; count--)
    {
      Serial.println(count);
      delay(1000);
    }

    Serial.print(colour + " values: ");
    for (int RGB = 0; RGB < 3; RGB++)
    {
      balanceArray[i][RGB] = getRawColour(RGB);
      Serial.print(String(balanceArray[i][RGB]) + " ");
    }
    Serial.println("\n");
  }

  Serial.print("GREY scale: ");
  for (int RGB = 0; RGB < 3; RGB++)
  {
    balanceArray[0][RGB] -= balanceArray[1][RGB];
    Serial.print(String(balanceArray[0][RGB]) + " ");
  }
  Serial.println("\n");
}

int get255Colour(int RGB)
{
  int colour = getRawColour(RGB);
  colour = ((float)colour - (float) balanceArray[1][RGB]) / (float) balanceArray[0][RGB] * 255.0;
  if (colour < 0)
  {
    return 1;
  }
  if (colour > 255)
  {
    return 255;
  }
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

void writeToEEPROM()
{
  int EEAddress = 0;

  for (int GB = 0; GB < 2; GB++) // GREY and BLACK
  {
    for (int RGB = 0; RGB < 3; RGB++)
    {
      EEPROM.put(EEAddress, balanceArray[GB][RGB]);
      EEAddress += sizeof(int);
    }
  }

  // for checking when reading memory
  EEPROM.put(EEAddress, 'J');
  EEPROM.put(EEAddress + 1, 'B');
  EEPROM.put(EEAddress + 2, 'M');
}

char *Colour_calc(int red, int green, int blue) 
{
 if (red > 230 && green > 230 && blue > 230)
 {
  led.setColor(255, 255, 255);
  led.show();
  return "white";
 }

 if (red < 80 && green < 80 && blue < 80)
 {
  return "black";
 }

 if(red > green){

    if(green > blue)
    {
      if(green > 180)
      {
        led.setColor(255, 100, 0);
        led.show();
        return "orange";
      }
      led.setColor(255, 0, 0);
      led.show();
      return "red";
    }
 }

  if(blue > green){

    float ratio = (float) red / (float) blue;
    Serial.println(ratio);
    if (ratio > float(0.9))
    {
      led.setColor(255, 0, 255);
      led.show();
      return "purple";
    }
    else
    {
      led.setColor(0, 0, 255);
      led.show();
      return "blue";
    }
  }
  else
  {
    led.setColor(0, 255, 0);
    led.show();
    return "green";
  }
}
