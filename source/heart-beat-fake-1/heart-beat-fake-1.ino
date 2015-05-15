long heartBeatArray[] = {
  50, 100, 15, 1200
};
int hbeatIndex = 1;    // this initialization is not important
long prevMillis;

int LEDpin = 10;
int LEDpin2 = 9;


void setup()
{
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  pinMode(LEDpin2, OUTPUT);
}

void loop()
{
  heartBeat(0.5);     // try changing parameter
}

void heartBeat(float tempo) {
  if ((millis() - prevMillis) > (long)(heartBeatArray[hbeatIndex] * tempo)) {
    hbeatIndex++;
    if (hbeatIndex > 3) hbeatIndex = 0;

    if ((hbeatIndex % 2) == 0) {    // modulo 2 operator will be true on even counts
      digitalWrite(LEDpin, HIGH);
      digitalWrite(LEDpin2, LOW);
    }
    else {
      digitalWrite(LEDpin, LOW);
      digitalWrite(LEDpin2, HIGH);
    }
    //  Serial.println(hbeatIndex);
    prevMillis = millis();
  }
}


void stopHeatbeat() {
  digitalWrite(LEDpin, LOW);
  digitalWrite(LEDpin2, LOW);
}
