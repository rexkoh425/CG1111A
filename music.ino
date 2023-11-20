
/* plays our ending music --> Mission Impossible Theme Song */
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
