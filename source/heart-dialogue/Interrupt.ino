/// Interrupt


// array to hold last ten IBI values
volatile int rate[10];
// used to determine pulse timing
volatile unsigned long sampleCounter = 0;
// used to find IBI
volatile unsigned long lastBeatTime = 0;
// used to find peak in pulse wave, seeded
volatile int P =512;
// used to find trough in pulse wave, seeded
volatile int T = 512;
// used to find instant moment of heart beat, seeded
volatile int thresh = 512;
// used to hold amplitude of pulse waveform, seeded
volatile int amp = 100;
// used to seed rate array so we startup with reasonable BPM
volatile boolean firstBeat = true;
// used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;


void interruptSetup() {
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;  // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;  // DON'T FORCE COMPARE, 256 PRESCALER
  OCR2A = 0X7C;   // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;  // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();          // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED
}


// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE.
// Timer 2 makes sure that we take a reading every 2 miliseconds
// triggered when Timer2 counts to 124
ISR(TIMER2_COMPA_vect) {
  // disable interrupts while we do this
  cli();
  // read the Pulse Sensor
  Signal = analogRead(pulsePin);
  // keep track of the time in mS with this variable
  sampleCounter += 2;
  // monitor the time since the last beat to avoid noise
  int N = sampleCounter - lastBeatTime;

  // find the peak and trough of the pulse wave
  // avoid dichrotic noise by waiting 3/5 of last IBI
  if ((Signal < thresh) && (N > (IBI/5)*3)) {
    // T is the trough
    if (Signal < T) {
      // keep track of lowest point in pulse wave
      T = Signal;
    }
  }

  // thresh condition helps avoid noise
  if ((Signal > thresh) && (Signal > P)) {
    // P is the peak, keep track of highest point in pulse wave
    P = Signal;
  }

  // NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  // avoid high frequency noise
  if (N > 250) {
    if ((Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3)) {
      // set the Pulse flag when we think there is a pulse
      Pulse = true;
      // turn on pin 13 LED
      digitalWrite(blinkPin, HIGH);
      // measure time between beats in mS
      IBI = sampleCounter - lastBeatTime;
      // keep track of time for next pulse
      lastBeatTime = sampleCounter;

      // if this is the second beat, if secondBeat == TRUE
      if (secondBeat) {
        // clear secondBeat flag
        secondBeat = false;
        // seed the running total to get a realisitic BPM at startup
        for (int i = 0; i <= 9; i++) {
          rate[i] = IBI;
        }
      }

      // if it's the first time we found a beat, if firstBeat == TRUE
      if (firstBeat) {
        // clear firstBeat flag
        firstBeat = false;
        // set the second beat flag
        secondBeat = true;
        // enable interrupts again
        sei();
        // IBI value is unreliable so discard it
        return;
      }

      // keep a running total of the last 10 IBI values

      // clear the runningTotal variable
      word runningTotal = 0;

      // shift data in the rate array
      for (int i = 0; i <= 8; i++) {
        // and drop the oldest IBI value
        rate[i] = rate[i+1];
        // add up the 9 oldest IBI values
        runningTotal += rate[i];
      }

      // add the latest IBI to the rate array
      rate[9] = IBI;
      // add the latest IBI to runningTotal
      runningTotal += rate[9];
      // average the last 10 IBI values
      runningTotal /= 10;
      // how many beats can fit into a minute? that's BPM!
      BPM = 60000/runningTotal;
      // set Quantified Self flag
      QS = true;
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }
  }

  // when the values are going down, the beat is over
  if ((Signal < thresh) && (Pulse == true)) {
    // turn off pin 13 LED
    digitalWrite(blinkPin, LOW);
    // reset the Pulse flag so we can do it again
    Pulse = false;
    // get amplitude of the pulse wave
    amp = P - T;
    // set thresh at 50% of the amplitude
    thresh = amp/2 + T;
    // reset these for next time
    P = thresh;
    T = thresh;
  }

  // if 2.5 seconds go by without a beat
  if (N > 2500) {
    // set thresh default
    thresh = 512;
    // set P default
    P = 512;
    // set T default
    T = 512;
    // bring the lastBeatTime up to date
    lastBeatTime = sampleCounter;
    // set these to avoid noise
    firstBeat = true;
    // when we get the heartbeat back
    secondBeat = false;
  }

  // enable interrupts when youre done!
  sei();
}
