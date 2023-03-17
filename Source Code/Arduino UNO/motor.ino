const int feedback = 5;     // MKR's 10th pin = UNO's pin 5 
const int motorPin =  6;    // water pump Vcc = UNO's pin 6

int feedbackState = 0;      // variable for reading status

void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  pinMode(feedback, INPUT);
}

void loop() {
  feedbackState = digitalRead(feedback); // feedback's pin 5 value will be passed to feedbackState

  if (feedbackState == HIGH) {           // What if MKR 10th pin or UNO's 5th pin goes high ?
    digitalWrite(motorPin, HIGH);        // if abhove condition is true then set it HIGH
    Serial.println("HIGH");
  } else {                               // Otherwise
    digitalWrite(motorPin, LOW);         // set it LOW
    Serial.println("LOW");
  }
}
