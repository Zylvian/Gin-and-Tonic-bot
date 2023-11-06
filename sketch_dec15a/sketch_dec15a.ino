// Pump pins.
int ginPinIn1 = 8;
int ginPinIn2 = 9;

int tonicPinIn1 = 10;
int tonicPinIn2 = 11;

int pourButtonPin = 7;
int loadButtonPin = 5;

// Time it takes for liquid to move from start to end of tube.
int pump_cycle_time = 20000;


// Amount of gin and tonic to be dispensed (in cl).
int gin_amount = 20;
int tonic_amount = 80;

// Time it takes to dispense a centiliter
int cl_time = 8500;

// Time when button was pressed;
unsigned long buttonPressedTime = 0;
bool isPouring = false;

// Pump 1 is usually for gin, pump 2 is for tonic.
unsigned long pump1remaining = 0;
unsigned long pump2remaining = 0;

uint8_t pourButtonState;
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

  pourButtonState = digitalRead(pourButtonPin);

  if(isPouring == false) {
      // Check if button is pressed.
        if(pourButtonState == LOW){
          buttonPressed();
          startPourGT();
        }
  }
  if(isPouring == true){
      // Check if button is pressed, treat as interrupt and disable pumps.
      if(pourButtonState == LOW){
          controlPumps(3, false);
        }
      // If not, continue pouring.
    continuePour();
  }


  // Button dysfunctional atm
  /*uint8_t loadButtonState = digitalRead(loadButtonPin);
  if(loadButtonState == LOW){
    buttonPressed();
    loadDrink();
  }*/
  
  delay(1);
}

void loadDrink(){
  controlPumps(3, true);
  delay(pump_cycle_time);
  controlPumps(3, false);
}

void startPourGT(){
  controlPumps(3, true);
  pump1remaining = gin_amount*cl_time;
  pump2remaining = tonic_amount*cl_time;
}

void continuePour(){
  unsigned long timeSinceButton = millis() - buttonPressedTime;
  pump1remaining = pump1remaining - timeSinceButton;
  pump2remaining = pump2remaining - timeSinceButton;
  // Pump 1 check
  if(pump1remaining <= 0){
    controlPumps(2, false);
  }
  if(pump2remaining <= 0){
    controlPumps(1, false);
  }

  if(pump1remaining <= 0 && pump2remaining <= 0){
    isPouring = false;
  }
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
    Serial.print("A pump was toggled:");
    Serial.println(on, DEC); 
    digitalWrite(pinIn1, on);
    digitalWrite(pinIn2, LOW);
}

void buttonPressed(){
    buttonPressedTime = millis();
}
