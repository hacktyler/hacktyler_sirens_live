#include <SPI.h>

#include <SimpleTimer.h>

const int FREQ_MAX = 1500;
const int FREQ_MIN = 600;
const int FREQ_ASCENDING = 1;
const int FREQ_DESCENDING = 2;
const int FREQ_SKIP = 15;
const int SOUNDS_DELAY = 20;
const int LED_RED = 1;
const int LED_BLUE = 2;
const int LIGHTS_DELAY = 250;

int redPin = 9;
int bluePin = 8;
int buttonPin = 7;
int speakerPin = 6;

int freq;
int freqState = FREQ_ASCENDING; 
int buttonState;
int ledState = 0;

SimpleTimer timer;
int lightsTimer;
int soundsTimer;
int sirensTimer;

void setup() {
    pinMode(redPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(speakerPin, OUTPUT);

    Serial.begin(9600);

    while (!Serial) {
        ; // Wait for serial port to connect.
    }

    Serial.println("Ready");
}

void loop() {
    char controlByte = Serial.read();
    buttonState = digitalRead(buttonPin);

    if (controlByte == 'a') {
        Serial.println("Alert received");
        siren();
    } else if (buttonState == LOW) {
        Serial.println("Button pushed");
        siren();
    }

    timer.run();
}

void lights() {
    if (ledState == LED_RED) {
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, HIGH);

        ledState = LED_BLUE;
    } else {
        digitalWrite(redPin, HIGH);
        digitalWrite(bluePin, LOW);

        ledState = LED_RED;
    }
}

void sounds() {
    if (freqState == FREQ_ASCENDING) {
        freq += FREQ_SKIP;

        if (freq > FREQ_MAX) {
            freq = FREQ_MAX;
            freqState = FREQ_DESCENDING;
        }
    } else {
        freq -= FREQ_SKIP;

        if (freq < FREQ_MIN) {
            freq = FREQ_MIN;
            freqState = FREQ_ASCENDING;
        }
    }

    tone(speakerPin, freq);
}

void kill_siren() {
    timer.disable(lightsTimer);
    timer.deleteTimer(lightsTimer);  

    timer.disable(soundsTimer);
    timer.deleteTimer(soundsTimer); 

    noTone(speakerPin);
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
}

void siren() {
    // Debounce
    if (timer.isEnabled(sirensTimer)) {
        return;
    }

    freq = FREQ_MIN;

    lightsTimer = timer.setInterval(LIGHTS_DELAY, lights);
    soundsTimer = timer.setInterval(SOUNDS_DELAY, sounds);
    sirensTimer = timer.setTimeout(5000, kill_siren);
}

