// Sun  1 Jul 23:12:43 UTC 2018

// reverse logic - temporary.
// NICE semaphores in blinkie.  Helps to differentiate!
// + single-shot message
// + branch trapping/reset

// Feather M0 Express - sleep

#define LED 13      // internal red LED

// System will try to go to sleep if it sees no wake_EVENT
// volatile boolean wake_EVENT = false;
volatile boolean wake_EVENT = true; // TESTING - want 'false' here ordinarily.

void pins_setup(void) {
    pinMode(LED, OUTPUT);
}

void blinkon(void) {
    digitalWrite(LED, 1);
}

void blinkoff(void) {
    digitalWrite(LED, 0);
}

void pip(void) {
    blinkon();  delay(5);
    blinkoff(); delay(2400);
}

void pips(void) {
    blinkon();  delay(5);
    blinkoff(); delay(800);
}

void pipf(void) {
    blinkon();  delay(5);
    blinkoff(); delay(400);
}

void sleep_blink(void) {
    pips(); pips(); pips(); pips(); pips();
}

void wake_EVENT_payload(void) {
    // demo: blink the red LED
    for (int i=17; i>-1; i--) {
        pipf();
    }
}

void setup(void) {
    pins_setup();
    while(!Serial) {
        pip();  // blinkon(); flicker D13 LED
    }
    Serial.begin(19200);
}

void sleep_now(void) {
    sleep_blink(); // non-existing
}

void loop(void) {
    while (!wake_EVENT) { // nothing awakening -- wants to be sleep
        Serial.println("while (!wake_EVENT) {} loop\r\n");
        sleep_now();
        delay(3400);
    }
    if (wake_EVENT) {  // Human presses PB switch
        wake_EVENT = false; // reset
        wake_EVENT_payload();
        // if(wake_EVENT) {
            // Serial.println("branched to wake_EVENT\r\n");
        // } else {
            // Serial.println("branched to !wake_EVENT\r\n");
        // }
        delay(2400);
    }
    Serial.println("Everybody - awake or asleep, reaches here, but not until escaping the while loop.");
    Serial.println("Single-shot -- only see this once per reset.");
}
 
// volatile boolean wake_EVENT = false;
//     saw: while (!wake_EVENT) {} loop -- ONLY

// volatile boolean wake_EVENT = true;
//     saw: branched to ! - everybody -- while
 
