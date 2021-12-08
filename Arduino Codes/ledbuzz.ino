#include "ledbuzz.h"
#include "pitches.h"

float sleep = 10; //milliseconds
int pot_reading = 0; //potentiometer reading
float buzz_baby_shark_sleep = 0; //delay between tone and no tone
int knight_ride_times = 3; //number of times LED go back and forth
const int duty1 = 255; //analog duty 
const int duty2 = 80; //analog duty 
const int duty3 = 15; //analog duty 
const int led_array[NUM_LED] = {3,5,6,9,10,11}; //i/o pins led array 
const int motion_array[KNIGHT_MOTION] = {2,2,3,5,6,9,10,11,2,2}; //i/o pins with 2 placeholders in front and back (dummy)
const int song_notes[] =
  {
    NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G4, 
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, 
    NOTE_G4, NOTE_D4, NOTE_E4, NOTE_G4,
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, 
    NOTE_G4, NOTE_G4, NOTE_D4, NOTE_E4, 
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,
    NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_FS4
  }; //baby shark notes

const int note_durations[] = {2, 2, 4, 4, 4, 8, 4, 
                        8, 4, 4, 4, 4, 4, 4, 
                        8, 4, 8, 4, 4, 4, 4, 
                        4, 4, 8, 4, 8, 4, 4, 4, 1.5}; //baby shark note durations

/**
   * @brief  Setup o/p and i/p pins Arduino
   */
void setup()
{
  //Set LED's as output pins
  for (int i=0; i<NUM_LED; i++)
  {
  	pinMode(led_array[i], OUTPUT); 
  }
}

/**
   * @brief  Main loop body
   */
void loop()
{
  read_potentiometer(); 
  buzz_baby_shark();
  knight_rider();
}

/**
   * @brief  knight rider LED program runs back anf forth 'knight_rider_times'
   */
void knight_rider()
{ 
  while (knight_ride_times>0)
  {
    // Forward Knight Rider
    for (int i=KNIGHT_LED; i<=KNIGHT_MOTION-KNIGHT_LED; i++)
    {
      read_potentiometer();
      analogWrite(motion_array[i], duty1);
      delay(sleep);
      analogWrite(motion_array[i-1], duty2);
      delay(sleep);
      analogWrite(motion_array[i-2], duty3);
      delay(sleep);
    }

    // Backward Knight Rider
    for (int j=KNIGHT_MOTION-KNIGHT_LED-1; j>=KNIGHT_LED-1; j--)
    {
      read_potentiometer();
      analogWrite(motion_array[j], duty1);   
      delay(sleep);
      analogWrite(motion_array[j+1], duty2);
      delay(sleep);
      analogWrite(motion_array[j+2], duty3);
      delay(sleep);
    }
    
    knight_ride_times--;
  }
  
  knight_ride_times=3;
}

/**
   * @brief  Get potentiometer input and map to LED sleep and baby shark delay times
   */
void read_potentiometer()
{
  pot_reading = analogRead(POT_PIN);
  sleep = map(pot_reading, 0, 1024, 10, 50);
  buzz_baby_shark_sleep = map(pot_reading, 0, 1024, 1.5, 3.0);
}

/**
   * @brief  Play the notes for the specified duration to recreate baby shark tone
   */
void buzz_baby_shark()
{
  for (int note = 0; note < 30; note++) 
  {
    read_potentiometer();
	int note_duration = 1000 / note_durations[note];
    tone(BUZZ_PIN, song_notes[note], note_duration);
    int sleep = note_duration * buzz_baby_shark_sleep;
    delay(sleep);
    noTone(BUZZ_PIN);
  }
}