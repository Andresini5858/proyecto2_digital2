#define UP_IN 34
#define DOWN_IN 35
#define ACCEPT_IN 32

#define UP_OUT 27
#define DOWN_OUT 14
#define ACCEPT_OUT 12

void setup() {
  // put your setup code here, to run once:
  pinMode(UP_IN, INPUT);
  pinMode(DOWN_IN, INPUT);
  pinMode(ACCEPT_IN, INPUT);

  pinMode(UP_OUT, OUTPUT);
  pinMode(DOWN_OUT, OUTPUT);
  pinMode(ACCEPT_OUT, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(UP_IN) == LOW && digitalRead(DOWN_IN) == HIGH && digitalRead(ACCEPT_IN) == HIGH){
    delay(50);
    digitalWrite(UP_OUT, LOW);
    digitalWrite(DOWN_OUT, HIGH);
    digitalWrite(ACCEPT_OUT, HIGH);
  }

  else if (digitalRead(UP_IN) == HIGH && digitalRead(DOWN_IN) == LOW && digitalRead(ACCEPT_IN) == HIGH){
    delay(50);
    digitalWrite(UP_OUT, HIGH);
    digitalWrite(DOWN_OUT, LOW);
    digitalWrite(ACCEPT_OUT, HIGH);
  }

  else if (digitalRead(UP_IN) == HIGH && digitalRead(DOWN_IN) == HIGH && digitalRead(ACCEPT_IN) == LOW){
    delay(50);
    digitalWrite(UP_OUT, HIGH);
    digitalWrite(DOWN_OUT, HIGH);
    digitalWrite(ACCEPT_OUT, LOW);
  }

  else {
    digitalWrite(UP_OUT, HIGH);
    digitalWrite(DOWN_OUT, HIGH);
    digitalWrite(ACCEPT_OUT, HIGH);
  }

}
