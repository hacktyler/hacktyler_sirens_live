#include <SPI.h>
#include <Ethernet.h>
#include <PusherClient.h>

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

PusherClient client;

void setup() {
    Serial.begin(9600);

    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        for(;;)
        ;
    }

    Serial.print("My IP address: ");

    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        Serial.print(Ethernet.localIP()[thisByte], DEC);
        Serial.print("."); 
    }

    Serial.println();

    if(client.connect("d20fddb74c58823cd05d")) {
        client.bind("new_active_call", new_active_call);
        client.subscribe("presence-active-calls-test");

        Serial.println("Subscribed to channel");
    } else {
        while(1) {}
    }
}

void loop() {
    if (client.connected()) {
        client.monitor();
    }
}

void new_active_call(String data) {
    Serial.println("Test event received!");
}

