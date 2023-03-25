
static const uint8_t NUM_OF_LEDS = 6;

void setLED(uint8_t ledState){
  // write the ports slow
  digitalWrite(5, ((ledState & 0x01) ? LOW : HIGH));
  digitalWrite(3, ((ledState & 0x02) ? LOW : HIGH));
  digitalWrite(2, ((ledState & 0x04) ? LOW : HIGH));
  digitalWrite(1, ((ledState & 0x08) ? LOW : HIGH));
  digitalWrite(0, ((ledState & 0x10) ? LOW : HIGH));
  digitalWrite(4, ((ledState & 0x20) ? LOW : HIGH));
}
