// Sun  1 Jul 23:29:54 UTC 2018

// + branch trapping/reset

// Feather M0 Express - sleep

// volatile boolean wake_EVENT = false;
volatile boolean wake_EVENT = true;

void pins_setup(void) { }

void setup(void) {
    pins_setup();
    while(!Serial) {
    }
    Serial.begin(19200);
}

void sleep_now(void) {}

void loop(void) {
    while (!wake_EVENT) { // nothing awakening -- wants to be sleep
        sleep_now();
        Serial.println("!wake_EVENT\r\n");
        delay(400);
    }
    if (wake_EVENT) {  // Human presses PB switch
        wake_EVENT = false; // reset
        if(wake_EVENT) {
            Serial.println("branched to wake_EVENT\r\n");
        } else {
            Serial.println("branched to !wake_EVENT\r\n");
        }
        delay(2400);
    }
    Serial.println("Everybody - awake or asleep, reaches here.");
}
 

 
