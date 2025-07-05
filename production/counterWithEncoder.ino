/*
CONSTANTS
*/

// switches
const int BUT1 = 1;
const int BUT2 = 2;
const int BUT3 = 3;
const int BUT4 = 4;

// leds
const int LED1 = 5;
const int LED2 = 6;
const int LED3 = 7;
const int LED4 = 8;

// encoder stuff
const int ENC_A = 9;
const int ENC_B = 10;
const int ENC_BTN = 11;

// below is the same as simple count
const int BUTTONS[4] = {BUT1, BUT2, BUT3, BUT4};
const int LEDS[4] = {LED1, LED2, LED3, LED4};

int currentButtonIndex = 0;
int count = 0;
bool overflow = false;

int lastEncA = HIGH;
unsigned long lastDebTime = 0; // debounce time
bool lastEncBtn = HIGH;

// setup stuff
void setup(){
    for (int i = 0; i < 4; i++){
        pinMode(BUTTONS[i], INPUT_PULLUP);
        pinMode(LEDS[i], OUTPUT);
        digitalWrite(LEDS[i], LOW);
    }

    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);
    pinMode(ENC_BTN, INPUT_PULLUP);
}

// loop
void loop(){
    if (overflow) {
        blinkAllLEDs();
    } else {
        handleButtonSequence();
        handleEncoderRotation();
        handleEncoderButton();
    }
}

// for button sequences
void handleButtonSequence(){
    if (digitalRead(BUTTONS[currentButtonIndex]) == LOW){
        delay(50); //required debounce as seen before
        if (digitalRead(BUTTONS[currentButtonIndex]) == LOW){
            currentButtonIndex++;

            if (currentButtonIndex >= 4){
                currentButtonIndex = 0;
                count++;
                if (count >= 15){
                    overflow = true;
                } else {
                    displayCount(count);
                }
            }

            // wait for the button releases
            while (digitalRead(BUTTONS[currentButtonIndex == 0 ? 3 : currentButtonIndex - 1]) == LOW);
            delay(50);

        }
    }
}

void handleEncoderRotation(){
    int encA = digitalRead(ENC_A);
    int encB = digitalRead(ENC_B);

    if (encA != lastEncA && encA == LOW){
        if (encB == HIGH){
            // work with clockwise
            if (!overflow && count < 15){
                count++;
                displayCount(count);
            } else if (count >= 15) {
                overflow = true;
            }
        } else {
            // counter clockwise
            if (overflow){
                overflow = false;
            }
            if (count > 0){
                count--;
                displayCount(count);
            }
        }
    }
    lastEncA = encA;
}

void handleEncoderButton(){
    bool encBtn = digitalRead(ENC_BTN);

    if (encBtn == LOW && lastEncBtn == HIGH && (millis() - lastDebTime) > 200) {
        resetState();
        lastDebTime = millis();
    }

    lastEncBtn = encBtn;
}

void resetState(){
    count = 0;
    currentButtonIndex = 0;
    overflow = false;
    displayCount(count);
}

void displayCount(int value){
    for (int i = 0; i < 4; i++){
        digitalWrite(LEDS[i], (value >> i) & i);
    }
}

void blinkAllLEDs(){
    static unsigned long lastToggle = 0;
    static bool state = false;

    if (millis() - lastToggle >= 500){
        lastToggle = millis();
        state = !state;
        for (int i = 0; i < 4; i++){
            digitalWrite(LEDS[i], state ? HIGH : LOW);
        }
    }
}