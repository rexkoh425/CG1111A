#include <MeMCore.h>
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
int delay_54 = 28 * beat;
int delay_44 = 16 * beat;

/*
void playMissionImpossible()
 {
  for (int i = 0; i < 76; i++)
   {
    if (i==64)
     {
      buzzer.noTone();
     } 
    else
   {
    if(i<16)
    {
      if(i<8)
      buzzer.tone(verse1[i], durations[k]);
      else
      buzzer.tone(verse1[i-8], durations[k]);
    }
    if(i>=16&&i<32)
    {
      if(i<24)
      buzzer.tone(verse2[i-16], durations[k]);
      else
      buzzer.tone(verse2[i-24], durations[k]);

    }
    if(i>=32&&i<76)
    {
      if(i<54)
      buzzer.tone(verse3[i-32], tdurations[c]);
      else
      buzzer.tone(verse3[i-54], tdurations[c]);
      
    }
  }
  if(i<32)
  k++;
  else
  c++;

  if(k==8)
  {
  k-=8;
  }
  if(c==11)
  {
    c-=11;
  }
   }
}
*/

void playMissionImpossible()
{
  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      buzzer.tone(verse1[i], v1_durations[i]);
      delay(v1_durations[i]);
    }
  }

  for (int j = 0; j < 2; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      buzzer.tone(verse2[i], v1_durations[i]);
      delay(v1_durations[i]);
    }
  }

  for (int i = 0; i < 11; i++)
  {
    buzzer.tone(verse3[i], v3_durations_54[i]);
    delay(v3_durations_54[i]);
  }

  delay(delay_54);

  for (int i = 0; i < 11; i++)
  {
    buzzer.tone(verse4[i], v3_durations_54[i]);
    delay(v3_durations_54[i]);
  }
  delay(delay_54);
}

void setup() {
  // Any setup code here runs only once.
}

void loop() 
{
  // The main code here will run repeatedly (i.e., looping):
  playMissionImpossible();
  delay(1600); // Pause for 2 seconds before repeating the tune.
}