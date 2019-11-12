// ATtinyCore 85 Optoboot bootloader
//Code by Albert Kravcov

//#define F_CPU 8000000

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define modeADDR        1  // EEPROM Adress for mode setting
#define colorADDR       2  // EEPROM Adress for color setting
#define mapADDR         3  // EEPROM Adress for color setting

#define NUMPIXELS       16

#define Button          5 // 5nano | 0at85 
#define GATE_PIN        13 // 13nano | 3at85
#define WSLED_PIN       3 // 3nano | 4at85

#define longpresstime   1000  // in ms

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSLED_PIN, NEO_GRB + NEO_KHZ800);


int timer = 0;
int timer2 = 0;
int p = 0, dir = 1; // Position, direction of "eye";
byte j = 15;
int looptime = 0;
int buttontimer = 1001;
byte right = 0;
byte left = 0;
unsigned long previousMillis = 0;

int pressedbut = 0;
int i_butt = 0;

int mode_setting = 0;
int old_mode_setting = 0;
int color_setting = 0;
int old_color_setting = 0;
int map_setting = 0;
int old_map_setting = 0;
int pushedFlag = 0;


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
  pinMode(Button, INPUT_PULLUP); //Button
  digitalWrite(GATE_PIN, LOW);

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


          if (currentMillis - previousMillis >= 30) {
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


            if (currentMillis - previousMillis >= 50) {
              previousMillis = currentMillis;
              //p++;
              p += dir;
              if (p < 0) {
                p = 1;
                dir = -dir;
              }
              else if (p >= 7) {
                p = 6;
                dir = -dir;
              }

              if (color_setting == 0) {
                pixels.setPixelColor(p - 1, 0x100000); // Dark red
                pixels.setPixelColor(p    , 255, 0, 0); // Center pixel is brightest
                pixels.setPixelColor(p + 1, 0x100000); // Dark red
              }
              else if (color_setting == 1) {
                pixels.setPixelColor(p, 0, 255, 0); // off
                pixels.setPixelColor(p - 1, 0, 10, 0); // off
                pixels.setPixelColor(p + 1, 0, 10, 0); // off
              }

              else if (color_setting == 2) {
                pixels.setPixelColor(p, 0, 0, 255); // off
                pixels.setPixelColor(p - 1, 0, 0, 10); // off
                pixels.setPixelColor(p + 1, 0, 0, 10); // off
              }
              else if (color_setting == 3) {
                pixels.setPixelColor(p, 0, 255, 255); // off
                pixels.setPixelColor(p - 1, 0, 10, 10); // off
                pixels.setPixelColor(p + 1, 0, 10, 10); // off
              }
              else if (color_setting == 4) {
                pixels.setPixelColor(p, 255, 255, 0); // off
                pixels.setPixelColor(p - 1, 10, 10, 0); // off
                pixels.setPixelColor(p + 1, 10, 10, 0); // off
              }
              else if (color_setting == 5) {
                pixels.setPixelColor(p, 255, 0, 255); // off
                pixels.setPixelColor(p - 1, 10, 0, 10); // off
                pixels.setPixelColor(p + 1, 10, 0, 10); // off
              }

              pixels.show();
              //animation END

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

          if (p == 7) { //
            p = 0;
          }


          j--;

          if (j == 8) { //
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
        //BLINK ANIMATION  --------------------------------------------------

        if (timer < 1260) {
          timer++;
        }
        //Serial.println(timer);

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
          timer = 0;
        }


      }

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
    if (mode_setting < 2) {
      mode_setting += 1;
    }
    else if (mode_setting == 2) {
      mode_setting = 0;
    }


  }
}


void configuration() {
  digitalWrite(GATE_PIN, HIGH);

  buttoncheck();


  for (int i = 0; i < 3; i++) {
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

 

  if (pressedbut == 1) {

    if (map_setting < 2) {
      map_setting += 1;
      delay(5);
      buttontimer = 0;
    }

    else if (map_setting == 2) {
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

      pixels.show();
      delay(1000);
      old_map_setting = map_setting;
    }

    buttontimer = 1001;
  }


  pixels.show();
}
