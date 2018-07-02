// Sun  1 Jul 02:47:53 UTC 2018

// reverse logic - temporary.
// NICE semaphores in blinkie.  Helps to differentiate!
// + single-shot message
// + branch trapping/reset

// Feather M0 Express - sleep

#define LED 13      // internal red LED

// System will try to go to sleep if it sees no wake_EVENT
// volatile boolean wake_EVENT = false;
volatile boolean wake_EVENT = true; // TESTING - want 'false' here ordinarily.

void sleep_setup(void) {
    Serial.print("\r\nsleep_setup(); is executing..");
    Serial.println("\r\nsleep_setup(); is done executing.\r\n");
}

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
    sleep_setup();
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
 
/*
 * ARM Cortex™-M Programming Guide to Memory Barrier Instructions
 * 
 * Application Note 321
 */

/* http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBGGHF.html
 *
 * if the software requires that the next operation must not
 * be carried out until the effect of the access has taken
 * place, a DSB instruction must be used as shown in Example 1.
 * 
 * Example 1. Ensuring the effect of an SCS register-write is visible immediately
 * 
 *     SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Enable deepsleep //
 *     __DSB(); // Ensure effect of last store takes effect //
 *     __WFI(); // Enter sleep mode //
 * 
 *     Or
 * 
 *     void Device_IRQHandler(void) {
 *         software_flag = 1;   // Update software variable used in thread //
 *         SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Disable sleeponexit //
 *         __DSB();   // Ensure effect of last store takes effect //
 *         return;    // Exception return //}
 */
 
/* http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHBGGHF.html
 * 
 * In Cortex-M0, M0+ processors, the effect takes place
 * immediately after the access completes.
 * DSB is not strictly required after SCS accesses. // RESOLVE
 */

// volatile boolean wake_EVENT = false;
//     saw: while (!wake_EVENT) {} loop -- ONLY

// volatile boolean wake_EVENT = true;
//     saw: branched to ! - everybody -- while

