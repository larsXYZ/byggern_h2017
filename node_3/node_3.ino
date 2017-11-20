//Node 3 will be written in Arduino IDE

//Notes
#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816    // 261 Hz 
#define  d4    3401    // 294 Hz 
#define  e4    3030    // 329 Hz 
#define  f4    2865    // 349 Hz 
#define  g4    2551    // 392 Hz 
#define  a4    2272    // 440 Hz 
#define  a4s   2146
#define  b4    2028    // 493 Hz 
#define  c5    1912    // 523 Hz
#define  d5    1706
#define  d5s   1608
#define  e5    1517    // 659 Hz
#define  f5    1433    // 698 Hz
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955
#define  R     9999        //REST

//List of songs
#define SONG_COUNT 3 //2 songs, 1 mute, and 2 game sounds
#define SONG_LENGTH 40
unsigned int songList[SONG_COUNT+2][SONG_LENGTH]=
{
  /*STARWARS IMPERIAL MARCH */  {a4,500, R,50 ,a4,500, R,50 ,a4,500 ,f4,400 ,c5,200 ,a4,600 ,f4,400 ,c5,200 ,a4,600 ,e5,500 ,e5,500 ,e5,500 ,f5,400 ,c5,200 ,g5,600 ,f5,400 ,c5,200 ,a4,600}
  /*STARWARS THEME */         ,{f4,210, f4,210, f4,210,  a4s,1280,   f5,1280,  d5s,210,  d5,210,  c5,210, a5s,1280, f5,640, d5s,210,  d5,210,  c5,210, a5s,1280, f5,640, d5s,210, d5,210, d5s,210,c5,1280, R,0}
  /*MUTE*/                    ,{R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0}
  /* STARTUP */               ,{c4,100 ,a4,100 ,c5,400 ,d5s,500 ,e4,150 ,R,10000 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0}
  /* END GAME */              ,{e4,800 ,e3,1200 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0 ,R,0}
};

//Music variables
int current_song;
int volume;
int start;

//Control pins
#define INTERRUPT_PIN 3
#define INPUT_PIN_0 4
#define INPUT_PIN_1 5
#define SPEAKER_PIN 9

void setup()
{
  //Setup variables
  volume = 255;
  current_song = 0;
  start = 0;

  //Setup interrupt pin
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), listenForData, RISING);

  //Setup other pins
  pinMode(INPUT_PIN_0,INPUT);
  pinMode(INPUT_PIN_1,INPUT);

  //Setup output pin
  pinMode(SPEAKER_PIN, OUTPUT);

  //Activate Serial
  Serial.begin(9600);

  

  //Waits for node 1 to enable music
  while (!start){Serial.println("...");}
  
  //Plays startup sound
  play_song(songList[3]);
  Serial.println("SETUP COMPLETED");
  delay(3000);
  current_song = 0;
}

void loop()
{
  Serial.println(current_song);
  //Main operating loop, always listens for commands with interrupt pin
  play_song(songList[current_song]);
}
