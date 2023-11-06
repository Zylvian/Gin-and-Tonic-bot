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

// 
int cl_time = 8500;


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

  /*pourGT();

  delay(10000);*/

  uint8_t pourButtonState = digitalRead(pourButtonPin);
  if(pourButtonState == LOW){
    pourGT();
  }

  uint8_t loadButtonState = digitalRead(loadButtonPin);
  if(loadButtonState == LOW){
    loadDrink();
  }

  Serial.print(loadButtonState);
  
  delay(1);
}

void loadDrink(){
  Serial.print("asdasd:");
  controlPumps(3, true);
  delay(pump_cycle_time);
  controlPumps(3, false);
}

void pourGT(){
  controlPumps(3, true);
  
  delay(tonic_amount*cl_time);
  controlPumps(1, false);
  
  int remaining_time = gin_amount - tonic_amount;
  delay(remaining_time*cl_time);
  
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
    Serial.print("A pump was toggled:");
    Serial.println(on, DEC); 
    digitalWrite(pinIn1, on);
    digitalWrite(pinIn2, LOW);
}
