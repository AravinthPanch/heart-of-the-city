int ledPin = 10;
int ledPin2 = 9;

void setup() {
  pinMode(ledPin2, OUTPUT);
}

void loop() {

  digitalWrite(ledPin2, HIGH);
  delay(150);
  digitalWrite(ledPin2, LOW);

  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    delay(5);
  }

  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(10);
  }
}
