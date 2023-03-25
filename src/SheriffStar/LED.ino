
static const uint8_t NUM_OF_LEDS = 4;

void setLED(uint8_t ledState){
  // write the ports fast
  // ledState = ~ledState;
  // ledState &= 0x0F;
  // ledState <<= 1;
  // PORTB &= ~ledState;
  // PORTB |= ledState;

  // write the ports slow
  digitalWrite(1, ((ledState & 0x01) ? LOW : HIGH));
  digitalWrite(2, ((ledState & 0x02) ? LOW : HIGH));
  digitalWrite(3, ((ledState & 0x04) ? LOW : HIGH));
  digitalWrite(4, ((ledState & 0x08) ? LOW : HIGH));

    // digitalWrite(5, ((ledState & 0x10) ? LOW : HIGH));
    //digitalWrite(5, ((ledState & 0x20) ? LOW : HIGH));
}
