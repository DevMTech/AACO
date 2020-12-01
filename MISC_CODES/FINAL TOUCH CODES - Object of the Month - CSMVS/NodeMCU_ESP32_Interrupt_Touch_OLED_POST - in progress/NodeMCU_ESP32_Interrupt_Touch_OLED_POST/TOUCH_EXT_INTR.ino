struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {16, 0, false};

void IRAM_ATTR isr() {
  button1.numberKeyPresses += 1;
  button1.pressed = true;
}

void MPR121_Touch_setup() {
//  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, RISING);
//  attachInterrupt(digitalPinToInterrupt(D1), IntCallback_sw1, RISING);
}

int MPR121_Touch_loop() {
  if (button1.pressed) {
      Serial.printf("Button 16 has been pressed %u times\n", button1.numberKeyPresses);
//      SSD1306_OLED_display();
      button1.pressed = false;
      return 1;
  }

//  //Detach Interrupt after 1 Minute
//  static uint32_t lastMillis = 0;
//  if (millis() - lastMillis > 60000) {
//    lastMillis = millis();
//    detachInterrupt(button1.PIN);
//  Serial.println("Interrupt Detached!");
//  }
  return 0;
}
