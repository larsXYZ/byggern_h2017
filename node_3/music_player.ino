void play_song(int* song) //Plays current song
{
  int startsong = current_song; //Used to check if song is changed mid play
  
  
  for (int note = 0; note < SONG_LENGTH; note += 2)
  {
       
    if (song[note] == R)//If note is pause, we pause
    {
       analogWrite(SPEAKER_PIN, 0);
       delayMicroseconds(song[note+1]);
    }
    else //Else we play the note
    {
      for (int duration = 0; duration < song[note+1]; duration += song[note]/1000)
      {
        if (startsong != current_song) return; //If song is changed quit playing
        
          //Plays note
         analogWrite(SPEAKER_PIN, volume);
         delayMicroseconds(song[note]/2);
    
         analogWrite(SPEAKER_PIN, 0);
         delayMicroseconds(song[note]/2);
    
      }
    }
  }
}

