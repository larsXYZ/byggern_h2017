//Control function, changes volume, song etc
void listenForData()
{
  //Filter out repeating interrupt
  noInterrupts();
  delay(200);
  interrupts();
  
  //Starts game if not started yet
  if (!start)
  {
    start = 1;
    return;
  }
  

  //DEBUG
  Serial.println("INTERRUPT");

  
  bool CTRL_MODE = digitalRead(INPUT_PIN_0);
  bool DIRECTION = digitalRead(INPUT_PIN_1);

  if (CTRL_MODE) //Startup and end game
  {
    if (DIRECTION)
    {
      play_song(3); //Play startup sound
    }
    else
    {
      play_song(4); //Play end game sound
    }
  }
  else
  {
    if (DIRECTION) //Increase
    {
      if (current_song < SONG_COUNT-1) current_song++;
      else current_song = 0;
    }
    else  //Decrease
    {
      if (current_song > 0) current_song--;
      else current_song = SONG_COUNT;
    }
  }
}
