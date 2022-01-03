/* A mini timer that accepts minutes and seconds as inputs by adjusting
   the potiontiometer. Countdown will be shown on LCD. 
   LED and Buzzer will go off once countdown reaches 00:00. 
   Button on the LEFT side is the main button for selection. 
   Button on the RIGHT side is a pause button to pause the countdown. */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 4, 5, 6, 7); //RS, E, D4, D5, D6, D7

int pot = A0, button = A1, pauseButton = A2; //input pins
int led = 8, buzzer = 9;                    //output pins
int min, sec, pageCount = 0;
int buttonState = 0, pauseButtonState = 0;      //current state of the button
int lastButtonState = 0, pauseLastButtonState = 0; //previous state of button

int pauseButtonCondition = 0; //determines if countdown is paused
int n_delay = 200;  //Countdown to the time change of 1 second
bool sec_change = true; //logic state for the time change of 1 second

/**
   * @brief  Setup o/p and i/p pins, and LCD
   */
void setup() 
{
  pinMode(pot, INPUT);
  pinMode(button, INPUT);
  pinMode(pauseButton, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); //turn off LED
  
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  lcd.print("Arduino Timer"); // Print a starting message to the LCD.
  delay(1000);
  lcd.clear();
}

/**
   * @brief  Main loop body
   */
void loop() 
{
  switch(pageCount) //determines the page the timer is in
  {
    /**
      * @brief Start page, displays starting message
      */
    case 0: 
      lcd.setCursor(0,0);
      lcd.print("Press button to");
      lcd.setCursor(0,1);
      lcd.print("set timer");
    
      buttonCheck(); //check if button is pressed
      break;
    
    /**
      * @brief Ask user to set value of minutes for timer
      */
    case 1:
      lcd.setCursor(0,0);
      lcd.print("Set minutes: ");
      lcd.setCursor(13,0);
      min = map(analogRead(pot),0,1023,0,59); //maps pot value to 0-59 mins and store the minutes set
      
      if(min < 10) //for values less than 10 to display 0 infront, 
      {					//else it will show the last seen digit
        lcd.print("0");
      }
      lcd.print(min, DEC); 
      lcd.setCursor(0,1);
      lcd.print("Press to confirm");
      
      buttonCheck();
      break;
    
    /**
      * @brief Ask user to set value of seconds for timer
      */
    case 2:
      lcd.setCursor(0,0);
      lcd.print("Set seconds: ");
      lcd.setCursor(13,0);
      sec = map(analogRead(pot),0,1023,0,59); //map pot value to 0-59 sec and store the seconds set
      
      if(sec < 10) //for values less than 10 to display 0 infront,
      {					//else it will show the last seen digit
        lcd.print("0");
      }
      lcd.print(sec, DEC); 
      lcd.setCursor(0,1);
      lcd.print("Press to confirm");
    
      buttonCheck();
      break;
    
    /**
      * @brief Display time set by user
      */
    case 3:
      lcd.setCursor(0,0);
      lcd.print("Time set: ");
      if(min < 10)  //for values less than 10 to display 0 infront,
      {        		//else it will show the last seen digit
        lcd.print("0");
      }
      lcd.print(min);
      lcd.print(":");
      if(sec < 10)  //same with sec
      {
        lcd.print("0");
      }
      lcd.print(sec);
      lcd.setCursor(0,1);
      lcd.print("Press to confirm");
    
      buttonCheck();
      break;
    
    /**
      * @brief Display countdown of time
      */
    case 4:
      pauseButtonCheck(); //check if pause button is pressed
      if (pauseButtonCondition) //true means paused
      {
        lcd.setCursor(0,1);
        lcd.print("Paused");
      }
    
      else //unpaused
      {
        if (sec_change) //if 1 second passed
        { //diplays min and sec
          lcd.setCursor(0,1);
          lcd.print("      "); //to clear the "Paused" text when unpaused
          lcd.setCursor(0,0);
          lcd.print("Time left: ");
          if(min < 10)  //for values less than 10 to display 0 infront,
          {        		//else it will show the last seen digit
            lcd.print("0");
          }
          lcd.print(min);
          lcd.print(":");
          if(sec < 10)
          { 
            lcd.print("0"); 
          }
          lcd.print(sec);
          
          sec_change = false;
        }
        
        if(min == 0 && sec == 0) //to exit this case once countdown reaches 00:00
        {
          pageCount++;
          delay(100); //to briefly see the "00:00" displayed before clearing LCD
          lcd.clear();
          break; //jumps out of this case immediately
        }
        if(sec == 0) //decrements 1 minute every 60 sec
        {
          min--;
          sec = 60;
        }
        //splits the countdown of 1 sec into 200 x 5ms, this reduces
        //the delay every loop for the buttons to be more responsive
        if (n_delay == 0) //decrements 1 sec every 200 n_delay
        {
          sec--;
          n_delay = 200; //reset to 200 for next second
          sec_change = true; //indicates 1 sec has passed
        }
      
        n_delay--; //decrements 1 n_delay every
        delay(5); //5ms, this ensures that the loop is fast enuf
        		 // to capture the input from pause button
      } //end if(pauseButtonCondition)-else
      break;
    
    /**
      * @brief Display Ending message, blink LED and sound buzzer
      */
    case 5:
      lcd.setCursor(0,0);
      lcd.print("TIME IS UP!");
      lcd.setCursor(0,1);
      lcd.print("Hold to reset");
      digitalWrite(led, LOW); //turn on LED
      tone(buzzer, 500, 400); //sound buzzer
      delay(200);
      digitalWrite(led, HIGH); //turn off LED
      noTone(buzzer);  //turn off buzzer
      delay(200);
    
      buttonCheck();
      break;
    
    default: 
      lcd.print("ERROR"); //should not happen unless glitch
      break;
  
 } //end switch case
} // end main loop

/**
   * @brief  check if button is pressed using edge detection
   */
void buttonCheck() 
{
  buttonState = digitalRead(button); //reads current state of button

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) // if the state has changed
  {
    if (buttonState == HIGH) // if the current state is HIGH then the button
    {						 //went from LOW to HIGH, meaning it is pressed
      if(pageCount < 5) //for pageCount 0 to 4
      {
        pageCount++; //increment the pageCount for program to move on to next case
      }
      else //reset counter at last page when pageCount is 5
      {
        pageCount = 0;
      }
      lcd.clear();
      delay(200);
    }
    delay(30); // Delay a little bit to avoid bouncing
  }
  //save the current state as the last state, for next time through the loop
  //this ensures that if the button is pressed and held, it will not trigger
  //anything until it is released and pressed again
  lastButtonState = buttonState;    
}

/**
   * @brief  check if pause button is pressed using edge detection
   */
void pauseButtonCheck()
{
  pauseButtonState = digitalRead(pauseButton); //reads current state of pause button
  
  // compare the pauseButtonState to its previous state
  if (pauseButtonState != pauseLastButtonState) // if the state has changed
  {
    if (pauseButtonState == HIGH) // if the current state is HIGH then the button
    {							  // went from LOW to HIGH, meaning it is pressed
      pauseButtonCondition = !(pauseButtonCondition); //invert the logic of condition
    }
    delay(30); // Delay a little bit to avoid bouncing
  }
  //save the current state as the last state, for next time through the loop
  //this ensures that if the button is pressed and held, it will not trigger
  //anything until it is released and pressed again
  pauseLastButtonState = pauseButtonState;
}