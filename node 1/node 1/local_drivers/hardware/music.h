/*
	DRIVER FOR COMMUNICATING WITH NODE 3.
	-Changes songs
	-Plays 2 soundeffects, startup and end game
	-Uses PB0, PD4, and PD5
*/

#ifndef _MUSIC_
#define _MUSIC_

//Functions
void music_init(); //Sets up pins for output
void music_start_transmission(); //Starts transmission to node 3, music_end_transmission() must be called before next transmission can be sent
void music_end_transmission(); //Ends transmission
void music_start_up_sound(); //Plays startup sound
void music_ending_sound(); //Plays ending sound
void music_next_song(); //Goes to next song, if possible
void music_prev_song(); //Goes to next song, if possible

#endif