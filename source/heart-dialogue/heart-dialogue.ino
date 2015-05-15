/// Heart Dialogue

/** This is the Telegraph utility "module".
  *
  * >> Pulse Sensor Amped 1.2 <<
  * This code is for Pulse Sensor Amped by Joel Murphy and Yury Gitman
  *     www.pulsesensor.com
  *     >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
  * Pulse Sensor sample aquisition and processing happens in the background via
  * Timer 2 interrupt.  2mS sample rate.
  * PWM on pins 3 and 11 will not work when using this code, because we are
  * using Timer 2!
  *
  * The following variables are automatically updated:
  * Signal : int that holds the analog signal data straight from the sensor.
  *          updated every 2mS.
  * IBI:
  *     int that holds the time interval between beats.  2mS resolution.
  * BPM:
  *     int that holds the heart rate value, derived every beat, from averaging
  *     previous 10 IBI values.
  * QS:
  *     boolean that is made true whenever Pulse is found and BPM is updated.
  *     user must reset.
  * Pulse:
  *     boolean that is true when a heartbeat is sensed then false in time with
  *     pin13 LED going out.
  *
  * This code is designed with output serial data to Processing sketch
  * "PulseSensorAmped_Processing-xx"
  * The Processing sketch is a simple data visualizer.
  * All the work to find the heartbeat and determine the heartrate happens in
  * the code below.
  * Pin 13 LED will blink with heartbeat.
  * If you want to use pin 13 for something else, adjust the interrupt handler.
  * It will also fade an LED on pin fadePin with every beat.  Put an LED and
  * series resistor from fadePin to GND.
  * Check here for detailed code walkthrough:
  * http://pulsesensor.myshopify.com/pages/pulse-sensor-amped-arduino-v1dot1
  *
  * Code Version 1.2 by Joel Murphy & Yury Gitman  Spring 2013
  * This update fixes the firstBeat and secondBeat flag usage so that realistic
  * BPM is reported.
  *
  * Adapted by Pablo Ripolles @ Birmingham, 2014-06-25
  */


// Pulse Sensor purple wire connected to analog pin 0
int pulsePin = 0;
// pin to blink led at each beat
//int blinkLedPin = 13;
// pin to do fancy classy fading blink at each beat -- channel (2)
int blinkPin = 12;//6;//8;//9;
int blinkRate = 0;
int blinkTime = 0;
// pin to do fancy classy fading blink at each beat -- channel (1)
int fadePin = 5;
// used to fade LED on with PWM on fadePin
int fadeRate = 0;
// pin to do fancy classy fading blink at each beat -- channel D
int fade1Pin = 10;
// used to fade LED on with PWM on fadePin
int fade1Rate = 0;

// used to hold the pulse rate
volatile int BPM;
// holds the incoming raw data
volatile int Signal;
// holds the time between beats, must be seeded!
volatile int IBI = 600;
// true when pulse wave is high, false when it's low
volatile boolean Pulse = false;
// becomes true when Arduoino finds a beat
volatile boolean QS = false;


void setup() {
  // pin that will blink to your heartbeat!
  //pinMode(blinkLedPin, OUTPUT);
  // pin that will blink to your heartbeat!
  pinMode(blinkPin, OUTPUT);
  // pin that will fade to your heartbeat!
  pinMode(fadePin, OUTPUT);
  // we agree to talk fast!
  Serial.begin(115200);
  // sets up to read Pulse Sensor signal every 2mS
  interruptSetup();
  // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW
  // VOLTAGE, AND APPLY THAT VOLTAGE TO THE A-REF PIN
  //analogReference(EXTERNAL);
}


void loop() {
  // send Processing the raw Pulse Sensor data
  sendDataToProcessing('S', Signal);
  // Quantified Self flag is true when arduino finds a heartbeat
  if (QS == true) {
    blinkTime = millis();
    // Set 'fadeRate' Variable to 255 to fade LED with pulse
    fadeRate = 255;
    // Set 'fadeRate' Variable to 255 to fade LED with pulse
    fade1Rate = 255;
    // send heart rate with a 'B' prefix
    sendDataToProcessing('B', BPM);
    // send time between beats with a 'Q' prefix
    sendDataToProcessing('Q', IBI);
    // reset the Quantified Self flag for next time
    QS = false;
  }

  ledBlinkToBeat();
  ledFadeToBeat();

  // take a break
  delay(25);
}


void sendDataToProcessing(char symbol, int data) {
  // symbol prefix tells Processing what type of data is coming
  Serial.print(symbol);
  // the data to send culminating in a carriage return
  Serial.println(data);
}


void ledBlinkToBeat() {
  // blink LED
  //analogWrite(blinkLedPin, fadeRate);
  if (millis() - blinkTime <= IBI / 2.0) {
    blinkRate = HIGH;
  }
  else {
    blinkRate = LOW;
  }
  // blink EL Wire
  digitalWrite(blinkPin, blinkRate);
  // blink EL Wire
  //analogWrite(blinkPin, blinkRate);
}


void ledFadeToBeat() {
  // set LED fade value
  fadeRate -= 10;
  // keep LED fade value from going into negative numbers!
  fadeRate = constrain(fadeRate, 0, 255);
  // fade EL Wire
  analogWrite(fadePin, fadeRate);
}
