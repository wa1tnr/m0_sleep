// Mon  3 Jul 10:35:17 UTC 2018

// Feather M0 Express - sleep

// basically works - sleeps.  first_PASS keeps it viable for uploads.
// pb Switch allows you to put it to sleep the first time,
// and wake it up after it's been sleeping a while.

// +debounce  has correct debounce for type of switch used

#define LED 13      // internal red LED
#define WAKE_LINE 6 // PB switch on D6

// System will try to go to sleep if it sees no wake_EVENT
// volatile boolean wake_EVENT = true; // TESTING - want 'false' here ordinarily.
volatile boolean wake_EVENT = false;
volatile boolean first_PASS = true;

void PB_Switch_Handler(void) {  // Interrupt Service Routine (ISR) (isr)
    noInterrupts();
    wake_EVENT = true;          // flag: human requests a wake EVENT
    first_PASS = false;         // don't like doing this here. kludge.  fix.
    interrupts();

    // Q: where is a better location to reset first_PASS?
    // 
    //    first_PASS is only there to allow easy update of
    //    the firmware (by preventing sleep mode from being
    //    entered even one time).
}

void setup_pbSwitch(void) {
    interrupts();
    attachInterrupt(digitalPinToInterrupt(WAKE_LINE), PB_Switch_Handler, LOW);
    // LOW may be required, and this may in turn require the use of
    // a pullup on the WAKE_LINE (GPIO port pin, D6).

    // That would mean that the buttons on CPX cannot be used,
    // as they are already wired to complete the circuit to Vcc
    // (instead of ground, as is done, here).
}

void sleep_setup(void) {
    Serial.print("\r\nsleep_setup(); is executing..");
    setup_pbSwitch();
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; // system.h#L380
    PM->SLEEP.reg |= PM_SLEEP_IDLE_APB;
    Serial.println("\r\nsleep_setup(); is done executing.\r\n");
}

void pins_setup(void) {
    pinMode(LED, OUTPUT);
    pinMode(WAKE_LINE, INPUT_PULLUP);
    // digitalWrite(WAKE_LINE, HIGH);
}

void blinkon(void) {
    digitalWrite(LED, 1);
}

void blinkoff(void) {
    digitalWrite(LED, 0);
}

void pip(void) {
    blinkon();  delay(25);
    blinkoff(); delay(3400);
}

void pips(void) {
    blinkon();  delay(5);
    blinkoff(); delay(800);
}

void pipf(void) {
    blinkon();  delay(15);
    blinkoff(); delay(320);
}

void sleep_blink(void) {
    pips(); pips(); pips(); pips(); pips();
}

void wake_EVENT_payload(void) {
    // demo: blink the red LED
    Serial.println("\r\n\r\n\r\nHIT the payload.");
    for (int i=17; i>-1; i--) {
        pipf();
    }
}

/*
void sys_reset(void) {
    NVIC_SystemReset();
}
*/

#define SERIAL_WANTED
#undef SERIAL_WANTED

void setup(void) {
    Serial.begin(19200);
    delay(3000); // three second delay
    Serial.println("Hello.  signed on.");
    pins_setup(); sleep_setup();
#ifdef SERIAL_WANTED
    while(!Serial) {
        pip();  // blinkon(); flicker D13 LED
    }
#endif
}

#undef DSB_USED

void sleep_now(void) {
#ifdef DSB_USED
    __DSB(); // Data synchronization barrier
#endif
    __WFI(); // wait for interrupt
}

void debounce(void) {
    noInterrupts(); // disable interrupts
    delayMicroseconds(2900);
    interrupts();
}

void loop(void) {

    while (first_PASS) {
        pip();
    }

    while (!wake_EVENT) { // nothing awakening -- wants to be sleep
        sleep_now(); // ONLY place to sleep
    }
    if (wake_EVENT) {  // Human presses PB switch
        debounce();
        wake_EVENT = false; // reset
        wake_EVENT_payload();
    }
    Serial.println("Single-shot -- only see this once per reset.\r\n\r\n");
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

/*
 $ cat -n system.h | head -248 | tail -16
   233  //  
   234   * \brief Device sleep modes.
   235   *
   236   * List of available sleep modes in the device. A table of clocks available in
   237   * different sleep modes can be found in \ref asfdoc_sam0_system_module_overview_sleep_mode.
   238   //  
   239  enum system_sleepmode {
   240          //  IDLE 0 sleep mode. //
   241          SYSTEM_SLEEPMODE_IDLE_0,
   242          //  IDLE 1 sleep mode. //
   243          SYSTEM_SLEEPMODE_IDLE_1,
   244          //  IDLE 2 sleep mode. //
   245          SYSTEM_SLEEPMODE_IDLE_2,
   246          //  Standby sleep mode. //
   247          SYSTEM_SLEEPMODE_STANDBY,
   248  };
*/

/*
 $ cat -n system.h | head -382 | tail -7
   376          switch (sleep_mode) {
   377                  case SYSTEM_SLEEPMODE_IDLE_0:
   378                  case SYSTEM_SLEEPMODE_IDLE_1:
   379                  case SYSTEM_SLEEPMODE_IDLE_2:   // enumerated only in system.h
   380                          SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
   381                          PM->SLEEP.reg = sleep_mode;
   382                          break;
*/

