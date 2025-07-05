const int BUT1 = 1;
const int BUT2 = 2;
const int BUT3 = 3;
const int BUT4 = 4;

const int LED1 = 5;
const int LED2 = 6;
const int LED3 = 7;
const int LED4 = 8;


// Array with all the buttons and led's so we can flash in sync and count using binary!!!!
const int BUTTONS[4] = {BUT1, BUT2, BUT3, BUT4};
const int LEDS[4] = {LED1, LED2, LED3, LED4};

int currButtonIndex = 0;
int count = 0;
bool overflow = false;


void setup(){
    for (int i = 0; i < 4; i++){
        pinMode(BUTTONS[i], INPUT_PULLUP);
        pinMode(LEDS[i], OUTPUT);
        digitalWrite(LEDS[i], LOW);
    }
}

void loop(){
    if (overflow){
        blinkAllLEDs();
        return;
    }

    if (digitalRead(BUTTONS[currButtonIndex]) == LOW){
        delay(50); // debounce needed
        if (digitalRead(BUTTONS[currButtonIndex]) == LOW){
            currButtonIndex++;

            if (currButtonIndex >= 4){
            currButtonIndex = 0;
            count++;
            }

            if (count > 15) {
                overflow = true;
            } else {
                displayCount(count);
            }
        }

        // wait for the button release for the debounce again
        while (digitalRead(BUTTONS[currButtonIndex == 0 ? 3 : currButtonIndex - 1]) == LOW);
        delay(50);

    }
}

void displayCount(int value){
    for (int i = 0; i < 4; i++){
        if (value & (1 << i)){
            digitalWrite(LEDS[i], HIGH);
        } else {
            digitalWrite(LEDS[i], LOW);
        }
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