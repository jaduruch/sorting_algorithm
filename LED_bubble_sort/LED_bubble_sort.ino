/*******************************************************************************
* Bubble sort algorithm demo with LED-Stripe
*******************************************************************************/

#include <Adafruit_NeoPixel.h>

// LED-Stripe configuration
#define PIN_LED_STRIPE 23
#define NUMPIXELS 40
#define BUTTON_PIN 12

// #define ButtonState !digitalRead(BUTTON_PIN)
#define DELAY_TIME 2500
bool oldButtonState = 0;
bool generateRandomColor = 1;

Adafruit_NeoPixel LED = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_STRIPE, NEO_RGB + NEO_KHZ800);

// variables
uint32_t color_array[NUMPIXELS];                                                // array for the color of each pixel
uint32_t TEMP;                                                                  // temporary varable to swap numbers in array

// function to get type of color
uint8_t getColorType(uint32_t color) {
  if ((color & 0xFF0000) == 0xFF0000 && (color & 0x0000FF) == 0) return 1;      // red and not blue
  if ((color & 0x00FF00) == 0x00FF00 && (color & 0x0000FF) == 0) return 2;      // green and not blue
  if ((color & 0x00FF00) == 0x00FF00 && (color & 0xFF0000) == 0) return 3;      // green and not red
  if ((color & 0x0000FF) == 0x0000FF && (color & 0xFF0000) == 0) return 4;      // blue and not red
  if ((color & 0x0000FF) == 0x0000FF && (color & 0x00FF00) == 0) return 5;      // blue and not green
  if ((color & 0xFF0000) == 0xFF0000 && (color & 0x00FF00) == 0) return 6;      // red and not green
  return 0;                                                                     // unknown type
}

void setup() 
{
  LED.begin();
  LED.clear();
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  LED.setBrightness(15);                                                        // set LED brightness to 20 (max value = 255)
  LED.show();                                                               // update LED-Stripe
}

void loop() 
{
  bool ButtonState = !digitalRead(BUTTON_PIN);
  // generate random colors
  if(generateRandomColor && ButtonState > oldButtonState)
  {
    Serial.println("random color");
    for (int i = 0; i < NUMPIXELS; i++) 
    {
      uint8_t color_type = random(1, 7);                                          // set color_type to random number between 1 and 6

      switch (color_type)                                                         // switch case for differet color types
      {
        case 1:   color_array[i] = 0xFF0000 + random(0xFF + 1) * 0x000100; 
        break;

        case 2:   color_array[i] = 0x00FF00 + random(0xFF + 1) * 0x010000; 
        break;

        case 3:   color_array[i] = 0x00FF00 + random(0xFF + 1) * 0x000001; 
        break;

        case 4:   color_array[i] = 0x0000FF + random(0xFF + 1) * 0x000100; 
        break;

        case 5:   color_array[i] = 0x0000FF + random(0xFF + 1) * 0x010000; 
        break;

        case 6:   color_array[i] = 0xFF0000 + random(0xFF + 1) * 0x000001; 
        break;
      }
      LED.setPixelColor(i, color_array[i]);                                       // set pixel to correct color
    }
    LED.show();                                                                   // update LED-Stripe
  }

  
  if(!generateRandomColor && ButtonState > oldButtonState)
  {
    Serial.println("sort!");
    for (int i = 0; i < NUMPIXELS - 1; i++)                                       // bubble sort-algorithm
    {
      for (int j = 0; j < NUMPIXELS - 1 - i; j++) 
      {
        uint8_t color_type_j = getColorType(color_array[j]);                      // get color_type from function getColorType()
        uint8_t color_type_j1 = getColorType(color_array[j + 1]);                 

        // compare and swap
        if(color_type_j > color_type_j1)
        {
          TEMP = color_array[j];
          color_array[j] = color_array[j + 1]
          color_array[j + 1] = TEMP; 
        }
      }
    }






        if(color_type_j == color_type_j1)
        {
          if(color_type_j % 2 == 1)                                               // if color_type is odd...
          {
            if (color_array[j] > color_array[j + 1])                              // if the next array value is smaller than the current value
            {
              TEMP = color_array[j];                                              // swap colors
              color_array[j] = color_array[j + 1];
              color_array[j + 1] = TEMP;
              delayMicroseconds(DELAY_TIME);                                             // wait 0.5ms
            }
          }else                                                                   // if color_type is even
          {
            if (color_array[j] < color_array[j + 1])                              // if the next array value is bigger than the current value
            {
              TEMP = color_array[j];                                              // swap colors
              color_array[j] = color_array[j + 1];
              color_array[j + 1] = TEMP;
              delayMicroseconds(DELAY_TIME);                                             // wait 0.5ms
            }
          }
        }

        LED.setPixelColor(j, color_array[j]);                                     // set LED color
        LED.setPixelColor(j + 1, color_array[j + 1]);
        LED.show();                                                               // update LED-Stripe                          

      }
    }
  }

  if(ButtonState > oldButtonState) 
  {
    generateRandomColor = !generateRandomColor;
    Serial.print("generateRandomColor: "); Serial.println(generateRandomColor);
  }
  // Serial.print("ButtomState: "); Serial.print(ButtonState);Serial.print(", old: "); Serial.println(oldButtonState);
  oldButtonState = ButtonState;
  delay(10);
}
