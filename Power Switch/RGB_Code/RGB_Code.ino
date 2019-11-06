// ATtinyCore 85 Optoboot bootloader
//akt code

//#define F_CPU 8000000

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define modeADDR        1  // EEPROM Adress for mode setting
#define colorADDR       2  // EEPROM Adress for color setting
#define mapADDR         3  // EEPROM Adress for color setting


#define NUMPIXELS       16

#define MODE_JP         4 //4nano |1at85
#define Button          5 // 5nano | 0at85 
#define PWM_PIN         2 // 2all
#define GATE_PIN        13 // 13nano | 3at85
#define WSLED_PIN       3 // 3nano | 4at85

#define longpresstime   1000  // in ms

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSLED_PIN, NEO_GRB + NEO_KHZ800);


int timer = 0;
int timer2 = 0;
int p = 0;
int looptime = 0;
int buttontimer = 1001;
int right = 0;
int left = 0;
unsigned long previousMillis = 0;
byte pwmupdate = 0;

int pressedbut = 0;
int i_butt = 0;
int mode_setting = 0;
int old_mode_setting = 0;
int color_setting = 0;
int old_color_setting = 0;
int map_setting = 0;
int old_map_setting = 0;
int pushedFlag = 0;
int jumperFlag = 0;

volatile int pwm_value = 0;
volatile int prev_time = 0;

int pwm_value_filtered = 1000;

void setup() {

  mode_setting = EEPROM.read(modeADDR);

  if (mode_setting >= 6) {
    mode_setting = 0;
  }

  old_mode_setting = mode_setting;

  color_setting = EEPROM.read(colorADDR);

  if (color_setting >= 6) {
    color_setting = 0;
  }

  old_color_setting = color_setting;

  map_setting = EEPROM.read(mapADDR);

  if (map_setting >= 5) {
    map_setting = 0;
  }
  old_map_setting = map_setting;


  pinMode(GATE_PIN, OUTPUT); //GATE
  pinMode(PWM_PIN, INPUT); //PWM
  pinMode(Button, INPUT_PULLUP); //Button
  pinMode(MODE_JP, INPUT_PULLUP); //Jumper

  digitalWrite(GATE_PIN, LOW);

  pixels.begin(); // This initializes the NeoPixel library.

  // when pin D2 goes high, call the rising function
  attachInterrupt(0, rising, RISING);

  if (digitalRead(Button) == LOW) { //decect if button was pressed on startup
    pushedFlag = 1;
  }
  else {
    pushedFlag = 0;
  }

   if (digitalRead(MODE_JP) == LOW) { //decect if jumer is soldered
    jumperFlag = 1;
  }
  else {
    jumperFlag = 0;
  }

   
  pixels.clear();

  //Serial.begin(9600);

  //map_setting = 2; //just for debudding
  //mode_setting = 1;
  //color_setting = 1;
}


void rising() {
  attachInterrupt(0, falling, FALLING);
  prev_time = micros();
}

void falling() {
  attachInterrupt(0, rising, RISING);
  pwm_value = micros() - prev_time;

}




byte buttoncheck()
{
  int i_butt = 0;
  byte buttonz = 0;
  if (digitalRead(Button) != 1)
  {
    while (digitalRead(Button) != 1)
    {
      delay(2);
      i_butt++;
    }
    buttonz = 1;
    buttontimer = 0;


    if (i_butt > (longpresstime / 2))
    {
      buttonz = 2;
      buttontimer = 0;
    }
  }

  pressedbut = buttonz;
  return buttonz;
}



void loop() {

  //Serial.print("MAP");
  //Serial.println(map_setting);
  //Serial.print("MODE");
  //Serial.println(mode_setting);
  //Serial.print("COLOR");
  //Serial.println(color_setting);


  if (pushedFlag == 1) {
    configuration(); //if button was pressed on startup, go to LED mapping menu
  }

  else { //if not, do main sequence

    buttoncheck();
    colorchange();


            

    looptime++;



    //Saving the last state if no button is pressed for a hile
    if (buttontimer < 1000) {
      buttontimer++;
    }

    if (buttontimer == 1000) {
      if (color_setting != old_color_setting) {
        //save setting if it is changed
        EEPROM.write(colorADDR, color_setting);
        delay(10);
        //Serial.println("color_saved");
        old_color_setting = color_setting;
      }
       if (mode_setting != old_mode_setting) {
        //save setting if it is changed
        EEPROM.write(modeADDR, mode_setting);
        delay(10);
        //Serial.println("mode_saved");
        old_mode_setting = mode_setting;
      }
      buttontimer = 1001;
    }





    if (map_setting == 0) {


      for (int i = 0; i < 16; i++) {
        if (color_setting == 0) {
          pixels.setPixelColor(i, 255, 0, 0); // red
        }
        else if (color_setting == 1) {
          pixels.setPixelColor(i, 0, 255, 0); // green
        }
        else if (color_setting == 2) {
          pixels.setPixelColor(i, 0, 0, 255); // blue
        }
        else if (color_setting == 3) {
          pixels.setPixelColor(i, 0, 255, 255); // cyan
        }
        else if (color_setting == 4) {
          pixels.setPixelColor(i, 255, 255, 0); // yellow
        }
        else if (color_setting == 5) {
          pixels.setPixelColor(i, 255, 0, 255); // pink
        }





        if (mode_setting == 0) {

          unsigned long currentMillis = millis();

          if (currentMillis - previousMillis >= 50) {
            previousMillis = currentMillis;

            pwm_update();
            pixels.show();
          }


        }

        else if (mode_setting == 1) {

          //wipe animation 
          unsigned long currentMillis = millis();

          if (currentMillis - previousMillis >= 50) {
            previousMillis = currentMillis;
            pwm_update();
            p++;

            if (p == 15) { //
              p = 0;
            }
            if (color_setting == 0) {
              pixels.setPixelColor(p, 0, 0, 0); // off
              pixels.setPixelColor(p - 1, 50, 0, 0); // off
              pixels.setPixelColor(p + 1, 50, 0, 0); // off
            }
            else if (color_setting == 1) {
              pixels.setPixelColor(p, 0, 0, 0); // off
              pixels.setPixelColor(p - 1, 0, 50, 0); // off
              pixels.setPixelColor(p + 1, 0, 50, 0); // off
            }

            else if (color_setting == 2) {
              pixels.setPixelColor(p, 0, 0, 0); // off
              pixels.setPixelColor(p - 1, 0, 0, 50); // off
              pixels.setPixelColor(p + 1, 0, 0, 50); // off
            }
            else if (color_setting == 3) {
              pixels.setPixelColor(p, 0, 0, 0); // off
              pixels.setPixelColor(p - 1, 0, 50, 50); // off
              pixels.setPixelColor(p + 1, 0, 50, 50); // off
            }
            else if (color_setting == 4) {
              pixels.setPixelColor(p, 0, 0, 0); // off
              pixels.setPixelColor(p - 1, 50, 50, 0); // off
              pixels.setPixelColor(p + 1, 50, 50, 0); // off
            }
            else if (color_setting == 5) {
              pixels.setPixelColor(p, 0, 0, 0); // off
              pixels.setPixelColor(p - 1, 50, 0, 50); // off
              pixels.setPixelColor(p + 1, 50, 0, 50); // off
            }

            pixels.show();
            //animation test END

          }
        }
        else {
          pixels.show();
        }
      }

    }







    if (looptime == 50) {




      if (map_setting == 1) {


        for (int i = 0; i < 8; i++) {
          if (color_setting == 0) {
            pixels.setPixelColor(i, 255, 0, 0); // red
          }
          else if (color_setting == 1) {
            pixels.setPixelColor(i, 0, 255, 0); // green
          }
          else if (color_setting == 2) {
            pixels.setPixelColor(i, 0, 0, 255); // blue
          }
          else if (color_setting == 3) {
            pixels.setPixelColor(i, 0, 255, 255); // cyan
          }
          else if (color_setting == 4) {
            pixels.setPixelColor(i, 255, 255, 0); // yellow
          }
          else if (color_setting == 5) {
            pixels.setPixelColor(i, 255, 0, 255); // pink
          }





          if (mode_setting == 0) {

            unsigned long currentMillis = millis();


            if (currentMillis - previousMillis >= 50) {
              previousMillis = currentMillis;

              pwm_update();
              pixels.show();
            }


          }

          else if (mode_setting == 1) {

            //wipe animation test


            unsigned long currentMillis = millis();


            if (currentMillis - previousMillis >= 50) {
              previousMillis = currentMillis;
              pwm_update();
              p++;

              if (p == 7) { //
                p = 0;
              }
              if (color_setting == 0) {
                pixels.setPixelColor(p, 0, 0, 0); // off
                pixels.setPixelColor(p - 1, 50, 0, 0); // off
                pixels.setPixelColor(p + 1, 50, 0, 0); // off
              }
              else if (color_setting == 1) {
                pixels.setPixelColor(p, 0, 0, 0); // off
                pixels.setPixelColor(p - 1, 0, 50, 0); // off
                pixels.setPixelColor(p + 1, 0, 50, 0); // off
              }

              else if (color_setting == 2) {
                pixels.setPixelColor(p, 0, 0, 0); // off
                pixels.setPixelColor(p - 1, 0, 0, 50); // off
                pixels.setPixelColor(p + 1, 0, 0, 50); // off
              }
              else if (color_setting == 3) {
                pixels.setPixelColor(p, 0, 0, 0); // off
                pixels.setPixelColor(p - 1, 0, 50, 50); // off
                pixels.setPixelColor(p + 1, 0, 50, 50); // off
              }
              else if (color_setting == 4) {
                pixels.setPixelColor(p, 0, 0, 0); // off
                pixels.setPixelColor(p - 1, 50, 50, 0); // off
                pixels.setPixelColor(p + 1, 50, 50, 0); // off
              }
              else if (color_setting == 5) {
                pixels.setPixelColor(p, 0, 0, 0); // off
                pixels.setPixelColor(p - 1, 50, 0, 50); // off
                pixels.setPixelColor(p + 1, 50, 0, 50); // off
              }

              pixels.show();
              //animation test END

            }
          }
          else {
            pixels.show();
          }
        }

        if (timer <= 2000) {


          pixels.setPixelColor(8, 0, 0, 0); // color set.
          pixels.setPixelColor(9, 0, 0, 0); // color set.
          pixels.setPixelColor(10, right, 0, 0); // color set.
          pixels.setPixelColor(11, right, 0, 0); // color set.
          pixels.setPixelColor(12, 0, 0, 0); // color set.
          pixels.setPixelColor(13, 0, left, 0); // color set.
          pixels.setPixelColor(14, 0, left, 0); // color set.
          pixels.setPixelColor(15, 0, 0, 0); // color set.

          timer++;


          if (timer <= 300) {
            timer2++;

            if (timer2 == 10) {
              right = right + 1;
              left = left + 1;
              timer2 = 0;
            }
          }
          else {
            right = 255;
            left = 255;
          }

        }
        if ((timer > 300) && (timer < 320)) { //change to speed up
          pixels.setPixelColor(8, 255, 255, 255); // w_strobe.
          pixels.setPixelColor(9, 255, 255, 255); // w_strobe.
          pixels.setPixelColor(10, 255, 0, 0); // red.
          pixels.setPixelColor(11, 255, 0, 0); // red.
          pixels.setPixelColor(12, 255, 255, 255); // w_strobe.
          pixels.setPixelColor(13, 0, 255, 0); // green.
          pixels.setPixelColor(14, 0, 255, 0); // green.
          pixels.setPixelColor(15, 255, 255, 255); // w_strobe.
          pixels.show();
        }

        if (timer == 350) {
          pixels.setPixelColor(8, 0, 0, 0); // w_strobe.
          pixels.setPixelColor(9, 0, 0, 0); // w_strobe.
          pixels.setPixelColor(10, 0, 0, 0); // red.
          pixels.setPixelColor(11, 0, 0, 0); // w_strobe.
          pixels.setPixelColor(12, 0, 0, 0); // red2.
          pixels.setPixelColor(13, 0, 0, 0); // green.
          pixels.setPixelColor(14, 0, 0, 0); // w_strobe.
          pixels.setPixelColor(15, 0, 0, 0); // green2.
          pixels.show();
        }

        if ((timer > 600) && (timer < 620))  {
          pixels.setPixelColor(8, 255, 255, 255); // w_strobe
          pixels.setPixelColor(9, 255, 255, 255); // w_strobe
          pixels.setPixelColor(10, 255, 0, 0); // red.
          pixels.setPixelColor(11, 255, 0, 0); // red2.
          pixels.setPixelColor(12, 255, 255, 255); // w_strobe
          pixels.setPixelColor(13, 0, 255, 0); // green.
          pixels.setPixelColor(14, 0, 255, 0); // green2.
          pixels.setPixelColor(15, 255, 255, 255); // w_strobe
          pixels.show();
        }



        if (timer == 2000) {
          right = 0;
          left = 0;
          timer = 0;
        }

        looptime = 0;
      }

    }





    if (map_setting == 2) {

            
      for (int i = 0; i < 8; i++) {
        if (color_setting == 0) {
          pixels.setPixelColor(i, 255, 0, 0); // red
        }
        else if (color_setting == 1) {
          pixels.setPixelColor(i, 255, 255, 0); // red
        }
        else if (color_setting == 2) {
        }
        else if (color_setting == 3) {
        }
        else if (color_setting == 4) {
        }
        else if (color_setting == 5) {
        }
      }
      for (int i = 8; i < 16; i++) {
        if (color_setting == 0) {
          pixels.setPixelColor(i, 0, 255, 0); // green
        }
        else if (color_setting == 1) {

          pixels.setPixelColor(i, 0, 255, 255); // red
        }
        else if (color_setting == 2) {
        }
        else if (color_setting == 3) {
        }
        else if (color_setting == 4) {
        }
        else if (color_setting == 5) {
        }
      }




      if (mode_setting == 0) {

              unsigned long currentMillis = millis();

          if (currentMillis - previousMillis >= 50) {
            previousMillis = currentMillis;
            pwm_update();
            
            pixels.show();
          }
          
        
      }

      else if (mode_setting == 1) {
        
        
           unsigned long currentMillis = millis();

          if (currentMillis - previousMillis >= 50) {
            previousMillis = currentMillis;
            pwm_update();
            
          }
        
        
        if (timer < 1260) {
          timer++;
        }
        //Serial.println(timer);

        if (timer < 60) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 255, 0, 0); // on last half
          }
          pixels.show();
        }

        if ((timer > 60) && (timer < 220)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // on last half
          }
          pixels.show();
        }

        if ((timer > 240) && (timer < 300)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 255, 0, 0); // on last half
          }
          pixels.show();
        }

        if ((timer > 300) && (timer < 480)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // on last half
          }
          pixels.show();
        }


        if ((timer > 580) && (timer < 640)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 255); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // on last half
          }
          pixels.show();
        }
        if ((timer > 640) && (timer < 820)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // on last half
          }
          pixels.show();
        }

        if ((timer > 820) && (timer < 880)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 255); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // on last half
          }
          pixels.show();
        }
        if ((timer > 880) && (timer < 1260)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // on last half
          }
          pixels.show();
          timer = 0;
        }


      }

    

    }


  }
}


uint16_t _getPWM() {
  pwmupdate++;
  volatile uint32_t sum = 0;
  sum = pwm_value;
  delay(5);
  sum += pwm_value;
  delay(5);
  sum += pwm_value;
  delay(5);
  sum += pwm_value;
  
  return sum / 4;
}


void pwm_update() {

  pwm_value_filtered = _getPWM();
  //Serial.println(pwm_value_filtered);

  if (jumperFlag == 0) {
  if (pwm_value_filtered > 1500) {
    
    
    digitalWrite(GATE_PIN, HIGH);
 
  }

  else {
    digitalWrite(GATE_PIN, LOW);
  }
}

  else if (jumperFlag == 1) {
  if (pwm_value_filtered > 1500) {

    digitalWrite(GATE_PIN, LOW);
    
  }

  else {
    digitalWrite(GATE_PIN, HIGH);
  }
}
}


void colorchange() {

  

  if (pressedbut == 1) {

    if (color_setting < 5) {
      color_setting += 1;
    }
    else if (color_setting == 5) {
      color_setting = 0;
    }

  }

  if (pressedbut == 2) {
    if (mode_setting < 1) {
      mode_setting += 1;
    }
    else if (mode_setting == 1) {
      mode_setting = 0;
    }


  }
}


void configuration() {
  digitalWrite(GATE_PIN, HIGH);
  
  buttoncheck();


  for (int i = 0; i < 4; i++) {
    pixels.setPixelColor(i, 255, 255, 255); // white
  }

  if (map_setting == 0) {
    pixels.setPixelColor(0, 255, 0, 0); // red
  }

  else if (map_setting == 1) {
    pixels.setPixelColor(0, 255, 0, 0); // red
    pixels.setPixelColor(1, 255, 0, 0); // red
  }

  else if (map_setting == 2) {
    pixels.setPixelColor(0, 255, 0, 0); // red
    pixels.setPixelColor(1, 255, 0, 0); // red
    pixels.setPixelColor(2, 255, 0, 0); // red
  }
  
  if (map_setting == 3) {
    pixels.setPixelColor(0, 255, 0, 0); // red
    pixels.setPixelColor(1, 255, 0, 0); // red
    pixels.setPixelColor(2, 255, 0, 0); // red
    pixels.setPixelColor(3, 255, 0, 0); // red
  }


  if (pressedbut == 1) {

    if (map_setting < 3) {
      map_setting += 1;
      delay(5);
      buttontimer = 0;
    }
    
    else if (map_setting == 3) {
      map_setting = 0;
      delay(5);
      buttontimer = 0;
    }
    
  }




  if (buttontimer < 1000) {
    buttontimer++;
  }


  if (buttontimer == 1000) {

    if (map_setting != old_map_setting) {
      //save setting if it is changed
      EEPROM.write(mapADDR, map_setting);
      //Serial.println("Mapping saved!");
      //Serial.println(map_setting);

      pixels.setPixelColor(0, 0, 255, 0); // green
      pixels.setPixelColor(1, 0, 255, 0); // green
      pixels.setPixelColor(2, 0, 255, 0); // green
      pixels.setPixelColor(3, 0, 255, 0); // green

      pixels.show();
      delay(1000);
      old_map_setting = map_setting;
    }

    buttontimer = 1001;
  }


  pixels.show();
}
