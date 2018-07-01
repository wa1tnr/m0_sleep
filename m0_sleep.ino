// Sun  1 Jul 23:15:08 UTC 2018

// Feather M0 Express - sleep

// volatile boolean wake_EVENT = false;
volatile boolean wake_EVENT = true;

void setup(void) {
    while(!Serial) { }
    Serial.begin(19200);
    delay(1100);
}

void loop(void) {
    while (!wake_EVENT) { // nothing awakening -- wants to be sleep
        Serial.println("!wake_EVENT\r\n");
        delay(400);
    }
    if (wake_EVENT) {  // Human presses PB switch
        Serial.println("wake_EVENT\r\n");
        delay(400);
    }
    Serial.println("Everybody - awake or asleep, reaches here.");
}
 
