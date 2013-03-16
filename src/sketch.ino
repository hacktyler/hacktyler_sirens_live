#include <SPI.h>
#include <Ethernet.h>
#include <PusherClient.h>

const int MAX_FREQ = 1500;
const int MIN_FREQ = 600;
const int DELAY = 20;
const int FREQ_SKIP = 15;

byte mac[] = { 0x01, 0xAA, 0xBC, 0xCC, 0xDE, 0x1D };

PusherClient client;
int ledPin = 9;
int buttonPin = 8;
int speakerPin = 6;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(speakerPin, OUTPUT);

    Serial.begin(9600);

    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.println("Starting");

    // DHCP
    /*if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        return;
    }*/

    // Static IP
    byte ip[] = { 192, 168, 100, 30 };
    byte dns[] = { 10, 0, 1, 1 };
    Ethernet.begin(mac, ip);

    Serial.print("Assigned IP address: ");

    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        Serial.print(Ethernet.localIP()[thisByte], DEC);
        Serial.print("."); 
    }

    Serial.println();

    if (client.connect("d20fddb74c58823cd05d")) {
        client.bind("new_active_call", new_active_call);
        client.subscribe("active-calls");

        Serial.println("Subscribed to channel.");
    } else {
        Serial.println("Failed to subscribe to channel.");
    }
}

void loop() {
    if (client.connected()) {
        client.monitor();
    }

    int buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) {
        Serial.println("button pushed");
        sound_siren();
    }
}

void new_active_call(String data) {
    Serial.println("new_active_call event received!");
    sound_siren();
}

void sound_siren() {
    digitalWrite(ledPin, HIGH);

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

    digitalWrite(ledPin, LOW);
}

