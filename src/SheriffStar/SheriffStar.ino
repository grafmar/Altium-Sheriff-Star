/*
  ATTINY LED control to show different patterns and programs on 6 LEDs
  One button is read in to select the program
*/

const int buttonPin = 0;

uint32_t buttonTime = 0U;
bool buttonStateLast = false;
bool buttonStateDebounced = false;
bool buttonEvent = false;

void debounceButton();


void setup() {
  // initialize input and output pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  // DDRB = 0x1E;
  // PORTB = 0x01;
}


void loop() {
  // debounce the button which sets the buttonEvent for one cycle if a button is pressed
  debounceButton();
  
  // if the button is pressed, change to the next program
  if (buttonEvent) {
    setNextProgram();
  }

  // evaluate the program, which also sets the LEDs
  evalProgram();
}


void debounceButton() {
  bool buttonState = (digitalRead(buttonPin) == LOW);   // read the state of the button
//  bool buttonState = ((PORTB & 0x01) == 0x00);          // read the state of the button
  uint32_t now = millis();                              // get milliseconds from start as time reference
  buttonEvent = false;                                  // reset the buttonEvent

  if (buttonState != buttonStateDebounced) {            // only check if the button state is different than the debounced state
    if (buttonState != buttonStateLast) {               // if the button state changed since last time reset the timer
      buttonTime = now;
    }

    if ((now-buttonTime)> 30U) {                        // if state is stable for 30ms:
      buttonStateDebounced = buttonState;               //   set debounced state
      if (buttonState) {                                
        buttonEvent = true;                             //   set the buttonEvent (only if the button is pressed)
      }
    }
  }
  buttonStateLast = buttonState;                        // store the buttonState for the next check
}
