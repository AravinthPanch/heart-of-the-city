unsigned char pin = 13;
unsigned char counter = 0;
unsigned int heart_rate = 0;
unsigned long temp[21];
unsigned long sub = 0;
volatile unsigned char state = LOW;
bool data_effect = true;
const int max_heartpluse_duty = 2000; //you can change it follow your system's request.2000 meams 2 seconds.
//System return error if the duty overtrip 2 second.

void setup()
{
  pinMode(pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Please ready your eat clip.");
  delay(500);
  array_init();
  Serial.println("Heart rate test begin.");
  attachInterrupt(1, interrupt, RISING);//set interrupt 0,digital port 2
}

void loop()
{
  digitalWrite(pin, state);
}

void sum()//calculate the heart rate
{
  if (data_effect)
  {
    heart_rate = 120000 / (temp[20] - temp[0]); //6*20*1000/20_total_time
    Serial.print("Heart_rate_is:\t");
    Serial.println(heart_rate);
  }
  data_effect = 1; //sign bit
}

void interrupt()
{
  temp[counter] = millis();
  state = !state;
  Serial.println(counter, DEC);
  Serial.println(temp[counter]);
  switch (counter)
  {
    case (0):
      sub = temp[counter] - temp[20];
      Serial.println(sub);
      break;
    default:
      sub = temp[counter] - temp[counter - 1];
      Serial.println(sub);
      break;
  }
  if (sub > max_heartpluse_duty) //set 2 seconds as max heart pluse duty
  {
    data_effect = 0; //sign bit
    counter = 0;
    Serial.println("Heart rate measure error,test will restart!" );
    array_init();
  }
  if (counter == 20 && data_effect)
  {
    counter = 0;
    sum();
  }
  else if (counter != 20 && data_effect)
    counter++;
  else
  {
    counter = 0;
    data_effect = 1;
  }
}

void array_init()
{
  for (unsigned char i = 0; i != 20; ++i)
  {
    temp[i] = 0;
  }
  temp[20] = millis();
}
