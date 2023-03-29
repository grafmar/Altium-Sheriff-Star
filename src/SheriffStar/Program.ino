
uint8_t leds;
uint32_t lastProgStep = 0U;
uint8_t program_set = 0U;
bool showProgramNumber = false;

uint32_t progSetTime = 0U;

static const uint8_t NUM_OF_PROGRAMS = 7U;
static const uint32_t SHOW_PROGRAM_NUMBER_TIME = 1000U;


void setNextProgram() {
  progSetTime = millis();
  lastProgStep = progSetTime;
  showProgramNumber = true;
  program_set = (program_set + 1)% NUM_OF_PROGRAMS;
}

void evalProgram() {
  if (showProgramNumber) {
    if ((millis() - progSetTime) > SHOW_PROGRAM_NUMBER_TIME) {
      showProgramNumber = false;
    }
    if (program_set >= NUM_OF_LEDS) {
      setLED(0x20 | (0x01U<<(program_set-NUM_OF_LEDS)));
    } else {
      setLED(0x01U<<program_set);
    }
  }
  else {
    if (program_set == 0U) {
      // automatic change every 5s
      applyProgram(1U + (millis()/5000)%(NUM_OF_PROGRAMS-1));
    } else {
      applyProgram(program_set);
    }
  }
}

void applyProgram(uint8_t prog) {
  // evaluate program
  uint32_t now = millis();
  uint32_t nowUs = micros();
  static const uint32_t pwmFactor = 20000; // with micros this are 50Hz
  static const uint32_t pwmMinValue = 2000; // Low PWM values are difficult. Therefore set values below pwmMinValue to zero

    switch(prog) {
    case 0U: // automatic change - no program
      break;

    case 1U:
      // one on after the other all 100ms (all others off)
      leds = 0x01<<((now/100UL) % NUM_OF_LEDS);
      break;

    case 2U:
      // one off after the other all 100ms (all others on)
      leds = ~(0x01<<((now/100UL) % NUM_OF_LEDS));
      break;

    case 3U:
      // all 100ms randomly only one LED on
      if ((now - lastProgStep) > 100U) {
        lastProgStep = now;
        leds = 0x01<<random(5);
      }
      break;

    case 4U:
      // all 100ms randomly which are on or off (random patterns every 100ms)
      if ((now - lastProgStep) > 100U) {
        lastProgStep = now;
        leds = random(0xFF);
      }
      break;

    case 5U:
      // dimming up and down all LEDs together every 2s
      {
        float value = (1+sin(now/318.0))/2 * pwmFactor; // 2000/(2pi) = 318
        if (value < pwmMinValue) value = 0U; // Set values below pwmMinValue to zero
        if (value > (nowUs % pwmFactor)) {
          leds = 0xFF;
        } else {
          leds = 0x00;
        }
      }
      break;

    case 6U:
      // dimming up and down all LEDs in a circle every 2s (go around in a circly dimming up and down)
      {
        const float two_pi_6th = 2.0 * 3.1415926535897932384626433 / 6.0;
        leds = 0x00;
        for (uint8_t i=0; i<6U; i++) {
          leds = leds<<1;
          // float value = (1 + sin(now/318.0 + two_pi_6th*i))/2*25; // 2000/(2pi) = 318
          float value = ((1 + sin(now/318.0 + two_pi_6th*i))/2); // 2000/(2pi) = 318
          value = value * value; // square for more low level
          value = value * pwmFactor; // scale with PWM-value (60)
          if (value < pwmMinValue) value = 0U; // Set values below pwmMinValue to zero
          if (value > (nowUs % pwmFactor)) {
            leds |= 0x01;
          }
        }

        // this isn't faster
        // static const uint8_t lookup[20] = {15,26,38,49,57,60,57,49,38,26,15,7,3,1,0,0,0,1,3,7}; // offset sinus squared scaled by 60
        // leds = 0x00;
        // for (uint8_t i=0; i<6U; i++) {
        //   leds = leds<<1;
        //   if (lookup[(now/100)%20] > (now%60)) {
        //     leds |= 0x01;
        //   }
        // }
      }
      break;

    default:
    break;      
  }
  setLED(leds);
}