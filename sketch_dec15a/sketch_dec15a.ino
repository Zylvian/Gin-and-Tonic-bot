// Pump pins.
int ginPinIn1 = 8;
int ginPinIn2 = 9;

int tonicPinIn1 = 10;
int tonicPinIn2 = 11;

int pourButtonPin = 7;
int loadButtonPin = 5;

// MEASURES
// All measures have been made with 4mm tubing connected:
// Pump to flask tube: 60cm
/// Pump to glass tube: 30cm

// Time it takes for liquid to move from start to end of tube.
long pump_cycle_time = 96000;

// Amount of gin and tonic to be dispensed (in cl).
unsigned long gin_amount = 1;
unsigned long tonic_amount = 1;

// Time it takes to dispense a centiliter
unsigned long cl_time = 10500;

// Time when button was pressed;
unsigned long buttonPressedTime = 0;
bool isPouring = false;

// Pump 1 is usually for gin, pump 2 is for tonic.
unsigned long pump1remaining = 0;
unsigned long pump2remaining = 0;

unsigned long timeSinceButton = 0;
unsigned long timeStopped = 0;

// Setup
void setup() {

  Serial.begin(9600);
  
  pinMode(ginPinIn1, OUTPUT);
  pinMode(ginPinIn2, OUTPUT);
  
  pinMode(tonicPinIn1, OUTPUT);
  pinMode(tonicPinIn2, OUTPUT);
  
  pinMode(pourButtonPin, INPUT_PULLUP);
  pinMode(loadButtonPin, INPUT_PULLUP);
}

void loop() {

  uint8_t pourButtonState = digitalRead(pourButtonPin);

  if(isPouring == false && (millis() - timeStopped > 1000)) {
      // Check if button is pressed.
        if(pourButtonState == LOW){
          buttonPressed();
          startPourGT();
        }
  }
  if(isPouring == true){
      timeSinceButton = millis() - buttonPressedTime;
      // Check if button is pressed, treat as interrupt and disable pumps.
      if(pourButtonState == LOW){
        Serial.println(buttonPressedTime, DEC);
        if(!(timeSinceButton < 1000)){
          Serial.println("Attempted interrupt of pumps");
          stopPour();
        }
       }
        // If not, continue pouring.
      else{
        continuePour();
      }
      
  }


  // Button dysfunctional atm
  uint8_t loadButtonState = digitalRead(loadButtonPin);
  if(loadButtonState == LOW){
    //buttonPressed();
    //loadDrink();
    Serial.println("this button works?");
  }
  
  delay(1);
}

void loadDrink(){
  controlPumps(3, true);
  delay(pump_cycle_time);
  controlPumps(3, false);
}

void startPourGT(){
  
  Serial.println("GT pouring started!");
  isPouring = true;
  controlPumps(3, true);
  pump1remaining = gin_amount*cl_time;
  pump2remaining = tonic_amount*cl_time;


  Serial.println("---");
  Serial.println(tonic_amount);
  Serial.println(cl_time);
  Serial.println(pump2remaining);
  Serial.println("---");

  Serial.println(pump1remaining);
  Serial.println(timeSinceButton);
}

void continuePour(){
  // Pump 1 check
  bool stopPump1 = pump1remaining <= timeSinceButton;
  bool stopPump2 = pump2remaining <= timeSinceButton;

  Serial.print("PUMP 2: -> ");
  Serial.println(pump2remaining);
  Serial.print("TIME SINCE BUTTON: -> ");
  Serial.println(timeSinceButton, DEC);
  if(stopPump1){
    controlPumps(1, false);
  }
  if(stopPump2){
    controlPumps(2, false);
  }

  if(stopPump1 && stopPump2){
    stopPour();
  }
}

void stopPour(){
  timeStopped = millis();
  timeSinceButton = 0;
  isPouring = false;
  controlPumps(3, false);
}

// int which:
// 1 is gin
// 2 is tonic
// 3 is both.
void controlPumps(int which, bool on){
  
    if(which == 1){
      togglePump(ginPinIn1, ginPinIn2, on);
    }
    else if(which == 2){
      togglePump(tonicPinIn1, tonicPinIn2, on);
    }
    else if(which == 3){
      togglePump(ginPinIn1, ginPinIn2, on);
      delay(1);
      togglePump(tonicPinIn1, tonicPinIn2, on);
    }
}


void togglePump(int pinIn1, int pinIn2, bool on){
    //Serial.print("A pump was toggled:");
    //Serial.println(on, DEC); 
    digitalWrite(pinIn1, on);
    digitalWrite(pinIn2, LOW);
}

void buttonPressed(){
    buttonPressedTime = millis();
}
