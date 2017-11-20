void play_song(int* song) //Plays current song
{
  
  for (int note = 0; note < SONG_LENGTH; note += 2)
  {
       
    if (song[note] == R)//If note is pause, we pause
    {
       digitalWrite(SPEAKER_PIN, LOW);
       delayMicroseconds(song[note+1]);
    }
    else //Else we play the note
    {
      for (int duration = 0; duration < song[note+1]; duration += song[note]/1000)
      {
        if (last_message != NO_MESSAGE) return;
        
          //Plays note
         digitalWrite(SPEAKER_PIN, HIGH);
         delayMicroseconds(song[note]/2);
    
         digitalWrite(SPEAKER_PIN, LOW);
         delayMicroseconds(song[note]/2);
    
      }
    }
  }
}

