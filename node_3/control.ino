//Control function, changes volume, song etc
void ISR0()
{
  bool CTRL_MODE = digitalRead(INPUT_PIN_0);
  bool DIRECTION = digitalRead(INPUT_PIN_1);

  //Update last message
  if (CTRL_MODE)
  {
    if (DIRECTION) last_message = PLAY_STARTUP;
    else last_message = PLAY_END;
  }
  else
  {
    if (DIRECTION) last_message = NEXT_SONG;
    else last_message = PREV_SONG;
  }
  
}

void listenForData()
{
  Serial.println("DATA RECEIVED");
  //Starts game if not started yet
  if (!start)
  {
    start = 1;
    return;
  }

  switch (last_message)
  {
    case (NO_MESSAGE):
    {
      Serial.println("NO_MESSAGE");
      return;
    }
    case (PLAY_STARTUP):
    {
      last_message = NO_MESSAGE;
      Serial.println("PLAY_STARTUP");
      play_song(songList[3]);
      return;
    }
    case (PLAY_END):
    {
      last_message = NO_MESSAGE;
      Serial.println("PLAY_END");
      play_song(songList[4]);
      return;
    }
    case (NEXT_SONG):
    {
      last_message = NO_MESSAGE;
      Serial.println("NEXT_SONG");
      if (current_song < SONG_COUNT - 1) current_song++;
      return;
    }
    case (PREV_SONG):
    {
      last_message = NO_MESSAGE;
      Serial.println("PREV_SONG");
      if (current_song > 0) current_song--;
      return;
    }
  }
  
  
}
