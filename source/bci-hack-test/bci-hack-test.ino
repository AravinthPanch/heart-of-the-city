/**
 * Author: Aravinth Panchadcharam
 * Email: me@aravinth.info
 * Date: 15/05/15.
 * Project: Heart of The City
 */

int prevPulse = 1;                             // Starting of the input pulses
int inputPin = 13;                             // Input pin that get the pulses from BCI Finger Clip Sensor
int blinkPin = 10;                             // Output pin that is used to blink LED when there is a heart beat
int fadePin = 9;                               // Output pin that is used to fade during the Dicrotic Notch phase
int fadeValue;                                 // Holds the current fade value
unsigned long timeDifference = 0;              // Holds time difference between the heart beats. Average is above 600 milliseconds
unsigned long lastPulseTime = millis();        // Holds the time a heart pulse was detected


//------ Arduino UNO pin setup ------//
void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(blinkPin, OUTPUT);
  pinMode(fadePin, OUTPUT);
}


//------ Main Thread ------//
void loop() {
  int sensorVal = digitalRead(inputPin);       // Read digital data from sensor

  // Since the main loop has higher sampling rate than the digital output from the sensor,
  // WRITE must be executed only when there is change in the pulse.
  if (prevPulse == sensorVal) {
    if (sensorVal == 1) {                      // Sensor by default sends 0 when there is a heat beat.
      digitalWrite(blinkPin, LOW);
    }
    else {
      digitalWrite(blinkPin, HIGH);
    }
  }
  else {
    prevPulse = sensorVal;                     // if there is a change in the input, update prevPulse with current input
    timeDifference = millis() - lastPulseTime; // Calculating Inter Pulse Interval is not needed.
    lastPulseTime = millis();
    Serial.println(timeDifference);

    if (sensorVal == 0 && fadeValue == 0) {    // Fade during dichrotic notch and if LED is not already in the process of fading
      fade();
    }
  }
}

void fade() {
  for (fadeValue = 125 ; fadeValue <= 255; fadeValue += 15) {  // Fade in from half brightness to full brightness
    analogWrite(fadePin, fadeValue);
    delay(5);
  }

  for (fadeValue = 255 ; fadeValue >= 0; fadeValue -= 15) {     // Fade out from full brightness to nil
    analogWrite(fadePin, fadeValue);
    delay(5);
  }
  fadeValue = 0;
}



