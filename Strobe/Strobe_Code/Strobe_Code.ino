// DIY ATtinyCore AT10
// TPI upload through USBASP

byte setting = 1;
byte configFlag = 0;
byte hue = 0;
byte dir = 0;

//PIN SETUP
byte GATE = 0; // 0 AT10 --
byte JP1 = 1; // 1 AT10 --
byte JP2 = 2; // 2 AT10 --

void setup() {
  pinMode(GATE, OUTPUT); //GATE PIN 1
  pinMode(JP1, INPUT_PULLUP); //PIN 3 >jumper
  pinMode(JP2, INPUT_PULLUP); //PIN 4 >jumper
  analogWrite(GATE, 0);
  jumpercheck();
}



void loop() {


  if (configFlag == 0) { //do default flash pattern

    analogWrite(GATE, 255);
    delay(10);
    analogWrite(GATE, 0);
    delay(100);
    analogWrite(GATE, 255);
    delay(10);
    analogWrite(GATE, 0);
    delay(100);
    analogWrite(GATE, 255);
    delay(10);
    analogWrite(GATE, 0);
    delay(800);

  }

  else if (configFlag == 1) { //do rampup flash pattern

    if (hue < 50) {
      hue++;
    }

    else if (hue == 50) {
      analogWrite(GATE, 255);
      delay(50);
      analogWrite(GATE, 0);
      delay(100);
      analogWrite(GATE, 255);
      delay(50);
      analogWrite(GATE, 0);
      delay(500);
      hue = 0;
    }

    analogWrite(GATE, hue);
    delay(10);

  }

  else if (configFlag == 2) { // do beacon pattern


    if (dir == 0) {
      hue++;
      delay(5);
    }

    else if (dir == 1) {
      hue--;
      delay(5);
    }

    if (hue == 150) {
      analogWrite(GATE, 255);
      delay(50);
    }

    if (hue == 150) {
      dir = 1;
    }

    else if (hue == 0) {
      analogWrite(GATE, hue);
      delay(250);
      dir = 0;
    }

    analogWrite(GATE, hue);

  }

  else if (configFlag == 3) { // do constant on
    analogWrite(GATE, 150);

  }

  else {
    analogWrite(GATE, 0);
  }

}

void jumpercheck() { //jumper setting detection

  if (digitalRead(JP1) == LOW) { //JP1 detect
    configFlag = 1;

    if (digitalRead(JP2) == LOW) {
      configFlag = 3;
    }

  }
  else if (digitalRead(JP2) == LOW) { //JP2 detect
    configFlag = 2;

    if (digitalRead(JP1) == LOW) {
      configFlag = 3;
    }
  }


  else { //if no jumper is connected
    configFlag = 0;

  }

}
