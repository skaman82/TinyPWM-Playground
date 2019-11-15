// DIY ATTINY85 Hardware Package: https://github.com/sleemanj/optiboot/blob/master/dists/README.md#attiny
// No Bootloader, no tone support - but millis

//RGB Code for the "CTRL RGB mini"
//Code by Albert Kravcov

//#define F_CPU 8000000

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define modeADDR        1   // EEPROM Adress for mode setting
#define colorADDR       2   // EEPROM Adress for color setting
#define mapADDR         3   // EEPROM Adress for map setting
#define NUMPIXELS       16
#define Button          3   // 5nano | 0at85 | 3 LEDmini
#define WSLED_PIN       4   // 3nano | 4at85 & mini
#define PWM_PIN         2   // 2all (only mini)

#define longpresstime   1000  // in ms

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSLED_PIN, NEO_GRB + NEO_KHZ800);

int timer = 0;
byte timer2 = 0;
int p = 0, dir = 1; // Position, direction of "eye";
byte j = 15;
int looptime = 0;
int buttontimer = 1001;
byte right = 0;
byte left = 0;
unsigned long previousMillis = 0;

byte pressedbut = 0;
byte i_butt = 0;

byte mode_setting = 0;
byte old_mode_setting = 0;
byte color_setting = 0;
byte old_color_setting = 0;
byte map_setting = 0;
byte old_map_setting = 0;
byte pushedFlag = 0;

int pwm_value;

void setup() {

  mode_setting = EEPROM.read(modeADDR);

  if (mode_setting >= 3) {
    mode_setting = 0;
  }

  old_mode_setting = mode_setting;

  color_setting = EEPROM.read(colorADDR);

  if (color_setting >= 5) {
    color_setting = 0;
  }

  old_color_setting = color_setting;

  map_setting = EEPROM.read(mapADDR);

  if (map_setting >= 4) {
    map_setting = 0;
  }
  old_map_setting = map_setting;


  pinMode(Button, INPUT_PULLUP); //Button
  pinMode(PWM_PIN, INPUT); //pwm

  pixels.begin(); // This initializes the NeoPixel library.


  if (digitalRead(Button) == LOW) { //decect if button was pressed on startup
    pushedFlag = 1;
  }
  else {
    pushedFlag = 0;
  }

  pixels.clear();

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
      //STATIC COLORS MAP0  --------------------------------------------------

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
          pixels.show();
        }


        //WIPE ANIMATION MAP0  --------------------------------------------------

        else if (mode_setting == 1) {

          //wipe animation
          unsigned long currentMillis = millis();

          if (currentMillis - previousMillis >= 50) {
            previousMillis = currentMillis;
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
            //animation  END

          }
        }


        else if (mode_setting == 2) {

          //LARSON ANIMATION MAP0 (first 8 pixels)  --------------------------------------------------


          for (int i = 0; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // red
          }

          unsigned long currentMillis = millis();


          if (currentMillis - previousMillis >= 50) {
            previousMillis = currentMillis;
            p += dir;
            if (p < 0) {
              p = 1;
              dir = -dir;
            }
            else if (p >= 16) {
              p = 14;
              dir = -dir;
            }

            if (color_setting == 0) {
              pixels.setPixelColor(p - 2, 5, 0, 0); // Dark red
              pixels.setPixelColor(p - 1, 25, 0, 0); // Medium red
              pixels.setPixelColor(p, 255, 0, 0); // red
              pixels.setPixelColor(p + 1, 25, 0, 0); // Medium red
              pixels.setPixelColor(p + 2, 5, 0, 0); // Dark red
            }
            else if (color_setting == 1) {
              pixels.setPixelColor(p - 2, 0, 5, 0); //
              pixels.setPixelColor(p - 1, 0, 25, 0); //
              pixels.setPixelColor(p, 0, 255, 0); //
              pixels.setPixelColor(p + 1, 0, 25, 0); //
              pixels.setPixelColor(p + 2, 0, 5, 0); //
            }
            else if (color_setting == 2) {
              pixels.setPixelColor(p - 2, 0, 0, 5); // off
              pixels.setPixelColor(p - 1, 0, 0, 25); // off
              pixels.setPixelColor(p, 0, 0, 255); // off
              pixels.setPixelColor(p + 1, 0, 0, 25); // off
              pixels.setPixelColor(p + 2, 0, 0, 5); // off
            }
            else if (color_setting == 3) {
              pixels.setPixelColor(p - 2, 0, 5, 5); // off
              pixels.setPixelColor(p - 1, 0, 25, 25); // off
              pixels.setPixelColor(p, 0, 255, 255); // off
              pixels.setPixelColor(p + 1, 0, 25, 25); // off
              pixels.setPixelColor(p + 2, 0, 5, 5); // off
            }
            else if (color_setting == 4) {
              pixels.setPixelColor(p - 2, 5, 5, 0); // off
              pixels.setPixelColor(p - 1, 25, 25, 0); // off
              pixels.setPixelColor(p, 255, 255, 0); // off
              pixels.setPixelColor(p + 1, 25, 25, 0); // off
              pixels.setPixelColor(p + 2, 5, 5, 0); // off
            }
            else if (color_setting == 5) {
              pixels.setPixelColor(p - 2, 5, 0, 5); // off
              pixels.setPixelColor(p - 1, 25, 0, 25); // off
              pixels.setPixelColor(p, 255, 0, 255); // off
              pixels.setPixelColor(p + 1, 25, 0, 25); // off
              pixels.setPixelColor(p + 2, 5, 0, 5); // off
            }

            pixels.show();
            //animation END

          }
        }


        else {
          pixels.show();
        }
      }

    }



    if (looptime == 50) {

      //STATIC COLORS MAP1  --------------------------------------------------

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
              pixels.show();
            }


          }

          else if (mode_setting == 1) {

            //WIPE ANIMATION MAP1 (first 8 pixels)  --------------------------------------------------

            unsigned long currentMillis = millis();

            if (currentMillis - previousMillis >= 50) {
              previousMillis = currentMillis;
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

          else if (mode_setting == 2) {

            //LARSON ANIMATION MAP1 (first 8 pixels)  --------------------------------------------------

            for (int i = 0; i < 8; i++) {
              pixels.setPixelColor(i, 0, 0, 0); // red
            }

            unsigned long currentMillis = millis();

            if (currentMillis - previousMillis >= 80) {
              previousMillis = currentMillis;


              p += dir;
              if (p < 0) {
                p = 1;
                dir = -dir;
              }
              else if (p >= 6) {
                p = 5;
                dir = -dir;
              }


              if (color_setting == 0) {
                pixels.setPixelColor(p - 2, 5, 0, 0); // Dark red
                pixels.setPixelColor(p - 1, 20, 0, 0); // Dark red
                pixels.setPixelColor(p    , 255, 0, 0); // Center pixel is brightest
                pixels.setPixelColor(p + 1, 20, 0, 0); // Dark red
                pixels.setPixelColor(p + 2, 5, 0, 0); // Dark red
              }
              else if (color_setting == 1) {
                pixels.setPixelColor(p - 2, 0, 5, 0); // off
                pixels.setPixelColor(p - 1, 0, 20, 0); // off
                pixels.setPixelColor(p, 0, 255, 0); // off
                pixels.setPixelColor(p + 1, 0, 20, 0); // off
                pixels.setPixelColor(p + 2, 0, 5, 0); // off
              }

              else if (color_setting == 2) {
                pixels.setPixelColor(p - 2, 0, 0, 5); // off
                pixels.setPixelColor(p - 1, 0, 0, 20); // off
                pixels.setPixelColor(p, 0, 0, 255); // off
                pixels.setPixelColor(p + 1, 0, 0, 20); // off
                pixels.setPixelColor(p + 2, 0, 0, 5); // off
              }
              else if (color_setting == 3) {
                pixels.setPixelColor(p - 2, 0, 5, 5); // off
                pixels.setPixelColor(p - 1, 0, 20, 20); // off
                pixels.setPixelColor(p, 0, 255, 255); // off
                pixels.setPixelColor(p + 1, 0, 20, 20); // off
                pixels.setPixelColor(p + 2, 0, 5, 5); // off
              }
              else if (color_setting == 4) {
                pixels.setPixelColor(p - 2, 5, 5, 0); // off
                pixels.setPixelColor(p - 1, 20, 20, 0); // off
                pixels.setPixelColor(p, 255, 255, 0); // off
                pixels.setPixelColor(p + 1, 20, 20, 0); // off
                pixels.setPixelColor(p + 2, 5, 5, 0); // off
              }
              else if (color_setting == 5) {
                pixels.setPixelColor(p - 2, 5, 0, 5); // off
                pixels.setPixelColor(p - 1, 20, 0, 20); // off
                pixels.setPixelColor(p, 255, 0, 255); // off
                pixels.setPixelColor(p + 1, 20, 0, 20); // off
                pixels.setPixelColor(p + 2, 5, 0, 5); // off
              }

              pixels.show();
              //animation END

            }
          }

          else {
            //pixels.show();
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
      //STATIC COLORS  --------------------------------------------------

      for (int i = 0; i < 8; i++) {
        if (color_setting == 0) {
          pixels.setPixelColor(i, 255, 0, 0); // red
        }
        else if (color_setting == 1) {
          pixels.setPixelColor(i, 255, 255, 0); // yellow
        }
        else if (color_setting == 2) {
          pixels.setPixelColor(i, 255, 100, 0); // orange
        }
        else if (color_setting == 3) {
          pixels.setPixelColor(i, 255, 0, 0); // red
        }
        else if (color_setting == 4) {
          pixels.setPixelColor(i, 255, 0, 200); // pink
        }
        else if (color_setting == 5) {
          pixels.setPixelColor(i, 255, 255, 255); // white
        }
      }
      for (int i = 8; i < 16; i++) {
        if (color_setting == 0) {
          pixels.setPixelColor(i, 0, 255, 0); // green
        }
        else if (color_setting == 1) {
          pixels.setPixelColor(i, 0, 255, 255); // bright blue
        }
        else if (color_setting == 2) {
          pixels.setPixelColor(i, 100, 0, 255); // purple
        }
        else if (color_setting == 3) {
          pixels.setPixelColor(i, 0, 0, 255); // blue
        }
        else if (color_setting == 4) {
          pixels.setPixelColor(i, 0, 255, 10); // light green
        }
        else if (color_setting == 5) {
          pixels.setPixelColor(i, 255, 255, 255); // white
        }
      }




      if (mode_setting == 0) {
        pixels.show();
      }



      else if (mode_setting == 1) {

        //WIPE ANIMATION MAP2 (8 & 8 pixels)  --------------------------------------------------

        for (int i = 0; i < 16; i++) {
          pixels.setPixelColor(i, 0, 0, 0); // off first half
        }

        unsigned long currentMillis = millis();


        if (currentMillis - previousMillis >= 80) {
          previousMillis = currentMillis;
          p++;

          if (p == 6) { //
            p = 0;
          }


          j--;

          if (j == 9) { //
            j = 15;
          }


          if (color_setting == 0) {
            //red
            pixels.setPixelColor(p - 2, 10, 0, 0); // off
            pixels.setPixelColor(p - 1, 50, 0, 0); // off
            pixels.setPixelColor(p, 255, 0, 0); // off
            pixels.setPixelColor(p + 1, 50, 0, 0); // off
            pixels.setPixelColor(p + 2, 10, 0, 0); // off

            //green
            pixels.setPixelColor(j - 2, 0, 10, 0); // off
            pixels.setPixelColor(j - 1, 0, 50, 0); // off
            pixels.setPixelColor(j, 0, 255, 0); // off
            pixels.setPixelColor(j + 1, 0, 50, 0); // off
            pixels.setPixelColor(j + 2, 0, 10, 0); // off
          }

          else if (color_setting == 1) {
            //yellow
            pixels.setPixelColor(p - 2, 10, 10, 0); // off
            pixels.setPixelColor(p - 1, 50, 50, 0); // off
            pixels.setPixelColor(p, 255, 255, 0); // off
            pixels.setPixelColor(p + 1, 50, 50, 0); // off
            pixels.setPixelColor(p + 2, 10, 10, 0); // off

            //light blue
            pixels.setPixelColor(j - 2, 0, 10, 10); // off
            pixels.setPixelColor(j - 1, 0, 50, 50); // off
            pixels.setPixelColor(j, 0, 255, 255); // off
            pixels.setPixelColor(j + 1, 0, 50, 50); // off
            pixels.setPixelColor(j + 2, 0, 10, 10); // off
          }

          else if (color_setting == 2) {
            //orange
            pixels.setPixelColor(p - 2, 10, 5, 0); // off
            pixels.setPixelColor(p - 1, 50, 20, 0); // off
            pixels.setPixelColor(p, 255, 100, 0); // off
            pixels.setPixelColor(p + 1, 50, 20, 0); // off
            pixels.setPixelColor(p + 2, 10, 5, 0); // off

            //purple
            pixels.setPixelColor(j - 2, 5, 0, 10); // off
            pixels.setPixelColor(j - 1, 20, 0, 50); // off
            pixels.setPixelColor(j, 100, 0, 255); // off
            pixels.setPixelColor(j + 1, 20, 0, 50); // off
            pixels.setPixelColor(j + 2, 5, 0, 10); // off
          }
          else if (color_setting == 3) {
            //red
            pixels.setPixelColor(p - 2, 10, 0, 0); // off
            pixels.setPixelColor(p - 1, 50, 0, 0); // off
            pixels.setPixelColor(p, 255, 0, 0); // off
            pixels.setPixelColor(p + 1, 50, 0, 0); // off
            pixels.setPixelColor(p + 2, 10, 0, 0); // off

            //blue
            pixels.setPixelColor(j - 2, 0, 0, 10); // off
            pixels.setPixelColor(j - 1, 0, 0, 50); // off
            pixels.setPixelColor(j, 0, 0, 255); // off
            pixels.setPixelColor(j + 1, 0, 0, 50); // off
            pixels.setPixelColor(j + 2, 0, 0, 10); // off
          }
          else if (color_setting == 4) {
            //pink
            pixels.setPixelColor(p - 2, 10, 0, 7); // off
            pixels.setPixelColor(p - 1, 50, 0, 40); // off
            pixels.setPixelColor(p, 255, 0, 200); // off
            pixels.setPixelColor(p + 1, 50, 0, 40); // off
            pixels.setPixelColor(p + 2, 10, 0, 7); // off

            //light green
            pixels.setPixelColor(j - 2, 0, 10, 3); // off
            pixels.setPixelColor(j - 1, 0, 50, 10); // off
            pixels.setPixelColor(j, 0, 255, 20); // off
            pixels.setPixelColor(j + 1, 0, 50, 10); // off
            pixels.setPixelColor(j + 2, 0, 10, 3); // off
          }
          else if (color_setting == 5) {
            //white
            pixels.setPixelColor(p - 2, 10, 10, 10); // off
            pixels.setPixelColor(p - 1, 50, 50, 50); // off
            pixels.setPixelColor(p, 255, 255, 255); // off
            pixels.setPixelColor(p + 1, 50, 50, 50); // off
            pixels.setPixelColor(p + 2, 10, 10, 10); // off

            //white
            pixels.setPixelColor(j - 2, 10, 10, 10); // off
            pixels.setPixelColor(j - 1, 50, 50, 50); // off
            pixels.setPixelColor(j, 255, 255, 255); // off
            pixels.setPixelColor(j + 1, 50, 50, 50); // off
            pixels.setPixelColor(j + 2, 10, 10, 10); // off
          }

          pixels.show();
          //animation test END

        }
      }


      else if (mode_setting == 2) {
        //BLINK ANIMATION MAP2  --------------------------------------------------

        if (timer < 1260) {
          timer++;
        }

        if (timer < 60) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half

          }
          for (int i = 0; i < 8; i++) {
            if (color_setting == 0) {
              pixels.setPixelColor(i, 255, 0, 0); // on last half RED
            }
            else if (color_setting == 1) {
              pixels.setPixelColor(i, 255, 255, 0); // yellow
            }
            else if (color_setting == 2) {
              pixels.setPixelColor(i, 255, 100, 0); // orange
            }
            else if (color_setting == 3) {
              pixels.setPixelColor(i, 255, 0, 0); // red
            }
            else if (color_setting == 4) {
              pixels.setPixelColor(i, 255, 0, 200); // pink
            }
            else if (color_setting == 5) {
              pixels.setPixelColor(i, 255, 255, 255); // white
            }
          }
          pixels.show();
        }

        if ((timer > 60) && (timer < 220)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off last half
          }
          pixels.show();

        }

        if ((timer > 240) && (timer < 300)) {
          for (int i = 8; i < 16; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off first half
          }
          for (int i = 0; i < 8; i++) {
            if (color_setting == 0) {
              pixels.setPixelColor(i, 255, 0, 0); // on last half RED
            }
            else if (color_setting == 1) {
              pixels.setPixelColor(i, 255, 255, 0); // yellow
            }
            else if (color_setting == 2) {
              pixels.setPixelColor(i, 255, 100, 0); // orange
            }
            else if (color_setting == 3) {
              pixels.setPixelColor(i, 255, 0, 0); // red
            }
            else if (color_setting == 4) {
              pixels.setPixelColor(i, 0, 255, 10); // light green
            }
            else if (color_setting == 5) {
              pixels.setPixelColor(i, 255, 255, 255); // white
            }
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
            if (color_setting == 0) {
              pixels.setPixelColor(i, 0, 255, 0); // green
            }
            else if (color_setting == 1) {
              pixels.setPixelColor(i, 0, 255, 255); // bright blue
            }
            else if (color_setting == 2) {
              pixels.setPixelColor(i, 100, 0, 255); // purple
            }
            else if (color_setting == 3) {
              pixels.setPixelColor(i, 0, 0, 255); // blue
            }
            else if (color_setting == 4) {
              pixels.setPixelColor(i, 255, 0, 200); // pink
            }
            else if (color_setting == 5) {
              pixels.setPixelColor(i, 255, 255, 255); // white
            }
          }
          for (int i = 0; i < 8; i++) {
            pixels.setPixelColor(i, 0, 0, 0); // off last half
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
            if (color_setting == 0) {
              pixels.setPixelColor(i, 0, 255, 0); // green
            }
            else if (color_setting == 1) {
              pixels.setPixelColor(i, 0, 255, 255); // bright blue
            }
            else if (color_setting == 2) {
              pixels.setPixelColor(i, 100, 0, 255); // purple
            }
            else if (color_setting == 3) {
              pixels.setPixelColor(i, 0, 0, 255); // blue
            }
            else if (color_setting == 4) {
              pixels.setPixelColor(i, 0, 255, 10); // light green
            }
            else if (color_setting == 5) {
              pixels.setPixelColor(i, 255, 255, 255); // white
            }
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
          delay(250);
          timer = 0;
        }
      }
    }

    if (map_setting == 3) {

      uint16_t pwm_value = _readPWM();

      uint16_t color_calc = pwm_value - 2000; //converts pwm to 0-1000 range

      if (color_calc < 0) {
        color_calc = 0;
      }
      if (color_calc > 1000) {
        color_calc = 1000;
      }

      byte color_calc_result = color_calc >> 2; //bitshift 10 to 8 bits
      //color_calc_result = color_calc_result, 0;


      for (int i = 0; i < 16; i++) {
        pixels.setPixelColor(i, Wheel(color_calc_result));
      }
      pixels.show();
      delay(10);
    }
  }
}







void colorchange() {

  if (pressedbut == 1) {

    if (color_setting < 4) {
      color_setting += 1;
    }
    else if (color_setting == 4) {
      color_setting = 0;
    }
  }

  if (pressedbut == 2) {
    if (mode_setting < 2) {
      mode_setting += 1;
    }
    else if (mode_setting == 2) {
      mode_setting = 0;
    }
  }
}


void configuration() {

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
  else if (map_setting == 3) {
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


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);

}

uint16_t _readPWM() {
  pwm_value = pulseIn(PWM_PIN, HIGH);

  volatile uint16_t sum = 0;
  sum = pwm_value;
  sum += pwm_value;
  return sum / 2;


}
