/*
CONSTANTS
*/

// switches for the buttons
const int BUT1 = 1;
const int BUT2 = 2;
const int BUT3 = 3;
const int BUT4 = 4;


// pinouts for the leds
const int LED1 = 5;
const int LED2 = 6;
const int LED3 = 7;
const int LED4 = 8;

// rotary encoder stuff
const int ENC_A = 9;
const int ENC_B = 10;
const int ENC_BTN = 11;


const int BUTTONS[4] = (BUT1, BUT2, BUT3, BUT4);
const int LEDS[4] = {LED1, LED2, LED3, LED4};

int count = 0;
bool gameRunning = false;
unsigned long gameStartTime = 0;
bool gameWon = false;
bool gameLost = false;

int lastEncA = HIGH;
bool lastEncBtn = HIGH;
unsigned long lastEncBtnDistance = 0;

void setup(){
    for (int i = 0; i < 4; i++){
        pinMode(BUTTONS[i], INPUT_PULLUP);
        pinMode(LEDS[i], OUTPUT);
        digitalWrite(LEDS[i], LOW);
    }

    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);
    pinMode(ENC_BTN, INPUT_PULLUP);

    indicateGameMode();


    startGame();
}


void loop(){
    if (!gameRunning) return;

    if (gameWon){
        victoryPulse();
        return;
    }

    if (gameLost){
        blinkAllLEDs();
        return;
    }

    handleButtonInputs();
    handleEncoderRotation();
    handleEncoderInput();

    if (count >= 15){
        gameWon = true;
        gameRunning = true;
        return;
    }

    if (millis() - gameStartTime >= 5000 && count < 15){
        gameLost = true;
        gameRunning = true;
        return;
    }
    displayCount();

}

void indicateGameMode(){
    // blink led1 and led3 alternately for approx 2 seconds
    for (int i = 0; i < 4; i++){
        digitalWrite(LED1, HIGH);
        digitalWrite(LED3, LOW);
        delay(250);

        digitalWrite(LED1, LOW);
        digitalWrite(LED3, HIGH);
        delay(250);
    }

    digitalWrite(LED1, LOW);
    digitalWrite(LED3, LOW);
}

void startGame(){
    count = 0;
    gameRunning = true;
    gameWon = false;
    gameLost = false;
    gameStartTime = millis();
    displayCount(count);
}

void handleButtonInputs(){
    for (int i = 0; i < 4; i++){
        if (digitalRead(BUTTONS[i]) == LOW){
            delay(50);
            if (digitalRead(BUTTONS[i]) == LOW){
                count++;
                displayCount();
                while (digitalRead(BUTTONS[i]) == LOW);
                delay(50);
            }
        }
    }
}

void handleEncoderInput(){
    bool encBtn = digitalRead(ENC_BTN);
    if (encBtn == LOW && lastEncBtn == HIGH && (millis() - lastEncBtnDistance) > 200){
        count++;
        displayCount(count);
        lastEncBtnDistance = millis();
    }
    lastEncBtn = encBtn;
}

void handleEncoderRotation(){
    int encA = digitalRead(ENC_A);
    int encB = digitalRead(ENC_B);

    if (encA != lastEncA && encA == LOW){
        if (encB == HIGH){
            resetGame();
        } else {
            resetGame();
        }
    }
    lastEncA = encA;
}

void resetGame(){
    count = 0;
    gameRunning = true;
    gameWon = false;
    gameLost = false;
    gameStartTime = millis();
    displayCount(count);
}

void displayCount(int value){
    for (int i = 0; i < 4; i++){
        digitalWrite(LEDS[i], (value >> 1) & 1);

    }
}

void blinkAllLEDs(){
    static unsigned long lastToggle = 0;
    static bool state = false;

    if (millis() - lastToggle >= 500){
        lastToggle = millis();
        state = !state
        for (int i = 0; i < 4; i++){
            digitalWrite(LEDS[i], state ? HIGH : LOW);
        }
    }
}

void victoryPulse(){
    static unsigned long lastStep = 0;
    static int currentLed = 0;

    if (millis() - lastStep >= 100){
        for (int i = 0; i < 4; i++){
            digitalWrite(LEDS[i], LOW);
        }
        digitalWrite(LEDS[currentLed], HIGH);
        currentLed = (currentLed + 1) % 4;
        lastStep = millis();
    }
}