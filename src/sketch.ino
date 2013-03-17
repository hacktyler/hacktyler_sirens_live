#include <SPI.h>

const int MAX_FREQ = 1500;
const int MIN_FREQ = 600;
const int DELAY = 20;
const int FREQ_SKIP = 15;

int redPin = 9;
int bluePin = 8;
int buttonPin = 7;
int speakerPin = 6;

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
    int buttonState = digitalRead(buttonPin);

    if (controlByte == 'a') {
        Serial.println("Alert received");
        sound_siren();
    } else if (buttonState == LOW) {
        Serial.println("Button pushed");
        sound_siren();
    }
}

void sound_siren() {
    digitalWrite(redPin, HIGH);

    int freq;
    unsigned long siren_start_time = millis();

    do
    {
        for (freq = MIN_FREQ; freq <= MAX_FREQ; freq += FREQ_SKIP)
        {
            tone(speakerPin, freq);
            delay(DELAY);
        }
        for (freq = MAX_FREQ; freq >= MIN_FREQ; freq -= FREQ_SKIP)
        {
            tone(speakerPin, freq);
            delay(DELAY);
        }
    } while (millis() - siren_start_time < 3000L);

    noTone(speakerPin);

    digitalWrite(redPin, LOW);
}

