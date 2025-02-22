/*
 *  GPIOKesme (interrupt) örneği,
 *  Bu örnekte karttaki butona veya harici bağlanan bir botuna basıldığında interrupt ile butona kaç defa basıldığı bilgisi seri port ekrane yazdırmaktadır. 
 *  
 */
struct Button {
    const uint8_t PIN;        
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {0, 0, false};  //Kartlardaki dahili buton 
Button button2 = {18, 0, false}; //GPIO18 bağlı pin

void ARDUINO_ISR_ATTR isr(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}

void ARDUINO_ISR_ATTR isr() {
    button2.numberKeyPresses += 1;
    button2.pressed = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterruptArg(button1.PIN, isr, &button1, FALLING);
    pinMode(button2.PIN, INPUT_PULLUP);
    attachInterrupt(button2.PIN, isr, FALLING);
}

void loop() {
    if (button1.pressed) {
        Serial.printf("1. Butona %u kez basıldı\n", button1.numberKeyPresses);
        button1.pressed = false;
    }
    if (button2.pressed) {
        Serial.printf("2. Butona %u kez basıldı\n", button2.numberKeyPresses);
        button2.pressed = false;
    }
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 10000) {
      lastMillis = millis();
      detachInterrupt(button1.PIN);
    }
}
