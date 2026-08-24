//Name - Saathvik Sastri  Id - F2026B3PS0499H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//to represent value of the state function
const int openSea = 0;
const int storm = 1;
const int charybdis = 2;
const int wrecked = 3;
const int anchorDown = 4;

//time in ms after which ship will get wrecked if in danger
const int wreckTime = 5000;

//initialize state at open sea
int state = openSea;

//set the pins used for buzzer, button input, led output and distance measurement
int buzzerPin = 11;
int buttonPin = 2;
int ledPin = 9;
int tPin = 7;
int ePin = 8;


unsigned long dangerStart = 0; //measures time from when ship is in danger
/*spaces are kept at the end of every string printed to clear out the previous state
without causing flickering by using lcd.clear() every loop*/
void handleState(bool isStorm, bool isCharybdis){
  lcd.setCursor(0, 0);
  lcd.print("STATE:          ");

  lcd.setCursor(0, 1);
  switch(state){
    case openSea:
        lcd.print("OPEN SEA       ");
    	if(isStorm){
          state = storm;
          dangerStart = millis();
    	}
    	else if(isCharybdis){
          state = charybdis;
          dangerStart = millis();
        }
    	break;
    
    case storm:
    	lcd.print("STORM           ");

    	if(millis() - dangerStart >= wreckTime){
        	state = wrecked;
    	}
    	else if(!isStorm && isCharybdis){
        	state = charybdis; //in case ship transitions directly from storm to charybdis, makes sure danger time is not reset
            digitalWrite(ledPin, LOW);
    	}
    	else if(!isStorm){
        	state = openSea;
        	digitalWrite(ledPin, LOW);
    	}
    	break;
    
    case charybdis:
    	lcd.print("CHARYBDIS       ");
    	tone(buzzerPin, 1000);

    	if(millis() - dangerStart >= wreckTime){
        	state = wrecked;
        	noTone(buzzerPin);
    	}
    	else if(!isCharybdis && isStorm){
        	state = storm;//in case ship transitions directly from charybdis to storm, makes sure danger time is not reset
          	noTone(buzzerPin);
    	}
    	else if(!isCharybdis){
        	state = openSea;
        	noTone(buzzerPin);
    	}
    	break;
    
    case wrecked:
    	lcd.print("WRECKED           ");
    	noTone(buzzerPin);
    	return;
    case anchorDown:
    	lcd.print("ANCHOR DOWN       ");
    	noTone(buzzerPin);
    	digitalWrite(ledPin, LOW);
    	break;
  }
  handleLed(ledPin); //blink the led if in storm
}

/*
When the button is pressed, the anchor changes state.
It ignores button release.
*/
int currentButton = 0; //current button state
int oldButton = 0; //old button state


void handleAnchor(int bNo){
  currentButton = digitalRead(bNo);
  if(oldButton == 0 && currentButton == 1 && state != wrecked){
    if(state == anchorDown){
      state = openSea;
    }
    else {
      state = anchorDown;
      dangerStart = millis();
    }
  }
  oldButton = currentButton;
}

/*
The light sensor does not return 0 and 1023 at min and max slider value
Readings at lowest and highest possible light levels have been found and mapped in a 0-100 range
If the mapped light is < 50 (half total), the ship is in storm conditions
*/
const int minLight = 54;
const int maxLight = 974;
bool checkStorm(int iNo){
  int light = map(analogRead(iNo), minLight, maxLight, 0, 100);
  return light<50;
}

/*
The distance sensor has a range in which it gives accurate readings - around 2.8 cm to 300 cm.
If the distance is either lower or higher, the sensor returns a reading of around 330 cm
To counteract such cases, an invalid distance is defined
If a reading is higher than invalidDist,i.e it is either out of the sensor's range or too far away to be able to trigger charybdis,
previously measured values of distance are used to determine whether the distance is too low or too high and check whether the ship is in danger.
*/
const int invalidDist = 300;
int oldDist = invalidDist;
int newDist;
bool checkCharybdis(int triggerPin, int echoPin) {
  
  newDist = readDist(triggerPin, echoPin);
  
  if(newDist < 0 || newDist >= invalidDist){    
    return oldDist < 100;
  }
  else {
    oldDist = newDist;
    return newDist < 100;
  }
}

//reads distance sensor by sending sound pulses and measuring time of travel
int readDist(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return round(0.01723 * duration);
}

//Blinks LED every 500 ms. Turns it off if the state is not storm.
void handleLed(int lPin){
  if((millis()/500)%2 == 0 && state == storm)digitalWrite(lPin, HIGH);
  else digitalWrite(lPin, LOW);
}


void setup () {
  pinMode(buzzerPin, OUTPUT); //buzzer output
  pinMode(buttonPin, INPUT); //pushButton
  pinMode(tPin, OUTPUT);//distance sensor trigger
  pinMode(ePin, INPUT);//distance sensor echo
  pinMode(ledPin, OUTPUT);//Storm LED
  
  //initialize lcd screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  Serial.begin(9600);
}

void loop () {
  handleAnchor(buttonPin);
  handleState(checkStorm(A0), checkCharybdis(tPin,ePin));
  delay(100); //for performance
}
