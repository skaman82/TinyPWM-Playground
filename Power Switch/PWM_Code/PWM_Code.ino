// RC PWM SWITCH
// By Albert Kravcov
//
// DIY ATTINY10 Hardware Package: https://github.com/sleemanj/optiboot/blob/master/dists/README.md#attiny

byte PWM_PIN = 0; //PB0 - PIN1
byte SENS_PIN = 1; //PB1 - PIN3
byte GATE_PIN = 2; //PB2 - PIN4


int pwm_value;

void setup() {

  pinMode(PWM_PIN, INPUT);
  pinMode(GATE_PIN, OUTPUT);
  pinMode(SENS_PIN, INPUT_PULLUP);

  digitalWrite(GATE_PIN, LOW);
}

void loop() {
  pwm_value = pulseIn(PWM_PIN, HIGH);

  if (digitalRead(SENS_PIN) == LOW) {


    if (pwm_value > 1500) {
      digitalWrite(GATE_PIN, LOW);
    }
    else {
      digitalWrite(GATE_PIN, HIGH);
    }
  }

  if (digitalRead(SENS_PIN) != LOW) {

    if (pwm_value > 1500) {
      digitalWrite(GATE_PIN, HIGH);
    }
    else {
      digitalWrite(GATE_PIN, LOW);
    }
  }





}
