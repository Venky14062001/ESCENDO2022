#include "led.h"
#include "tone.h"

float sleep; //milliseconds
int pot_reading = 0; //potentiometer reading
float buzz_baby_shark_sleep = 0; //delay between tone and no tone
int knight_ride_times = 3; //number of times LED go back and forth
const float sleep_min = 100; //min sleep for LED
const float sleep_max =  1500; //max sleep for LED
const float buzz_sleep_min = 1.0; //min sleep for baby shark
const float buzz_sleep_max =  2.0; //max sleep for baby shark
const float no_tone_sleep = 75; //no tone delay between notes
const int duty1 = 255; //maximum brightness
const int duty2 = 120; //mid-brightness
const int duty3 = 50; //dim/minimum brightness
const int led_array[NUM_LED] = {7,6,5,4,3,2}; //i/o pins led array 
const int motion_array[KNIGHT_MOTION] = {1,1,7,6,5,4,3,2,1,1}; //i/o pins with 2 placeholders in front and back (dummy)
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
   * @brief  knight rider LED program runs back and forth 'knight_rider_times'
   */
void knight_rider()
{ 
  while (knight_ride_times>0)
  {
    // Forward Knight Rider
    for (int i=KNIGHT_LED-1; i<=KNIGHT_MOTION-KNIGHT_LED; i++)
    {
      read_potentiometer();
      knight_rider_led_clear();
      analogWrite(motion_array[i], duty1);
      analogWrite(motion_array[i-1], duty2);
      analogWrite(motion_array[i-2], duty3);
      delay(sleep);
    }

    knight_rider_led_clear();

    // Backward Knight Rider
    for (int j=KNIGHT_MOTION-KNIGHT_LED; j>=KNIGHT_LED-1; j--)
    {
      read_potentiometer();
      analogWrite(motion_array[j], duty1);   
      analogWrite(motion_array[j+1], duty2);
      analogWrite(motion_array[j+2], duty3);
      delay(sleep);
      knight_rider_led_clear();
    }
    
    knight_ride_times--;
  }

  knight_rider_led_clear();
  
  knight_ride_times=3;
}

/**
   * @brief  set all LED's to zero
   */
void knight_rider_led_clear()
{
  //Set all LED's to 0 brightness
  for (int k=0; k<NUM_LED; k++)
  {
    analogWrite(led_array[k], 0); 
  }
}

/**
   * @brief  Get potentiometer input and map to LED sleep and baby shark delay times
   */
void read_potentiometer()
{
  pot_reading = analogRead(POT_PIN);
  sleep = sleep_min + (pot_reading*((sleep_max-sleep_min)/1024)); //sleep_min - sleep_max
  buzz_baby_shark_sleep = buzz_sleep_min + (pot_reading*((buzz_sleep_max-buzz_sleep_min)/1024));  
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
    int sleep_tone = note_duration * buzz_baby_shark_sleep;
    delay(sleep_tone);
    noTone(BUZZ_PIN);
    delay(no_tone_sleep);
  }
} 