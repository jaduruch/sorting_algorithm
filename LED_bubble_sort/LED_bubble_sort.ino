/*******************************************************************************
* Bubble sort algorithm demo with LED-Stripe
* @note this software sorts randomly generated LED colors using bubble-sort
* @version 0.2

* @explanation:
use this color picker: https://www.rapidtables.com/web/color/RGB_Color.html 
set S and V to 100%, select an angle for H between 0° and 360°

this is how the colors are composed:
H   0°     60°      120°     180°     240°     300°     360°

    --------|¬¬      |        |        |      ¬¬|--------
R           |  ¬¬    |        |        |    ¬¬  |
            |    ¬¬  |        |        |  ¬¬    |
            |      ¬¬|________|________|¬¬      |
            |        |        |        |        |           
          ¬¬|--------|--------|¬¬      |        |
        ¬¬  |        |        |  ¬¬    |        |
G     ¬¬    |        |        |    ¬¬  |        |
    ¬¬      |        |        |      ¬¬|________|________
            |        |        |        |        |         
            |        |      ¬¬|--------|--------|¬¬
            |        |    ¬¬  |        |        |  ¬¬
B           |        |  ¬¬    |        |        |    ¬¬
    ________|________|¬¬      |        |        |      ¬¬
            |        |        |        |        |          
     type 1 | type 2 | type 3 | type 4 | type 5 | type 6
*******************************************************************************/

#include <Adafruit_NeoPixel.h>                                                  // include library for adressable LEDs

// LED-Stripe configuration
#define PIN_LED_STRIPE 12                                                       // hardware pin of LEDs
#define NUMPIXELS 20                                                            // amount of pixels you wanna use
Adafruit_NeoPixel LED = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_STRIPE, NEO_GRB + NEO_KHZ800);		// LEDs used in this simulation
// Adafruit_NeoPixel LED = Adafruit_NeoPixel(NUMPIXELS, PIN_LED_STRIPE, NEO_RGB + NEO_KHZ800);	// most LEDs use this configuration

// defines
#define BUTTON_PIN 7                                                            // pin your button is connected to
#define DELAY_TIME 2500                                                         // delay time in us between sorting steps

// variables
uint32_t color_array[NUMPIXELS];                                                // array for the color of each pixel
uint32_t TEMP; 																	                                // temporary varable to swap numbers in array
bool oldButtonState;															                              // store previous state of button for edge detection
bool generateRandomColor = true;												                        // if true generate random color else sort colors


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
  Serial.begin(115200);                                                         // start serial port @115200 baud
  pinMode(BUTTON_PIN, INPUT_PULLUP);                                            // set the pin of the button to internal pullup
  LED.begin();                                                                  // inizialize LED-stripe
  LED.clear();                                                                  // delete all pixel
  LED.setBrightness(255);                                                       // set LED brightness to 15 if you build this yourself (max value = 255)
  LED.show();                                                                   // update LED-Stripe
}

void loop()                                                                     // forever...
{
  bool ButtonState = !digitalRead(BUTTON_PIN);                                  // read button pin and invert in due to internal pull-up

  if(generateRandomColor && ButtonState > oldButtonState)                       // positive edge detected
  {
    Serial.println("random color");
    for (int i = 0; i < NUMPIXELS; i++)                                         // count up all LED pixel
    {
      uint8_t color_type = random(1, 7);                                        // set color_type to random number between 1 and 6

    switch (color_type)                                                         // switch case for differet color types
      {
        case 1:   color_array[i] = 0xFF0000 + random(0xFF + 1) * 0x000100;      // 0xFF0000 ... 0xFFFF00
        break;

        case 2:   color_array[i] = 0x00FF00 + random(0xFF + 1) * 0x010000;      // 0x00FF00 ... 0xFFFF00
        break;

        case 3:   color_array[i] = 0x00FF00 + random(0xFF + 1) * 0x000001;      // 0x00FF00 ... 0x00FFFF
        break;

        case 4:   color_array[i] = 0x0000FF + random(0xFF + 1) * 0x000100;      // 0x0000FF ... 0x00FFFF
        break;

        case 5:   color_array[i] = 0x0000FF + random(0xFF + 1) * 0x010000;      // 0x0000FF ... 0xFF00FF
        break;

        case 6:   color_array[i] = 0xFF0000 + random(0xFF + 1) * 0x000001;      // 0xFF0000 ... 0xFF00FF
        break;
      }
      LED.setPixelColor(i, color_array[i]);                                     // set pixel to correct color
    }
    LED.show();                                                                 // update LED-Stripe
  }

  // bubble sort-algorithm
  if(!generateRandomColor && ButtonState > oldButtonState)                      // positive edge detected
  {
    Serial.println("sort!");                                                    
    for (int i = 0; i < NUMPIXELS - 1; i++)                                     // count up all pixel (exept the last)                     
    {
      for (int j = 0; j < NUMPIXELS - 1 - i; j++) 
      {
        uint8_t color_type_j = getColorType(color_array[j]);                    // get color_type from function getColorType()
        uint8_t color_type_j1 = getColorType(color_array[j + 1]);               // get color_type from next pixel       

        // compare and swap
        if(color_type_j > color_type_j1)
        {
          TEMP = color_array[j];
          color_array[j] = color_array[j + 1];
          color_array[j + 1] = TEMP; 
          delayMicroseconds(DELAY_TIME);                                      	// wait for a define delay time
        }

        if(color_type_j == color_type_j1)										                    // if they are the same color_type
        {
          if(color_type_j % 2 == 1)                                             // if color_type is odd
          {
            if (color_array[j] > color_array[j + 1])                            // if the next array value is smaller than the current value
            {
              TEMP = color_array[j];                                            // swap colors
              color_array[j] = color_array[j + 1];
              color_array[j + 1] = TEMP;
              delayMicroseconds(DELAY_TIME);                                    // wait for a define delay time
            }
          }else                                                                 // if color_type is even
          {
            if (color_array[j] < color_array[j + 1])                            // if the next array value is bigger than the current value
            {
              TEMP = color_array[j];                                            // swap colors
              color_array[j] = color_array[j + 1];
              color_array[j + 1] = TEMP;
              delayMicroseconds(DELAY_TIME);                                    // wait for a define delay time
            }
          }
        }

        LED.setPixelColor(j, color_array[j]);                                   // set LED color
        LED.setPixelColor(j + 1, color_array[j + 1]);
        LED.show();                                                             // update LED-Stripe                          

      }
    }
  }

  if(ButtonState > oldButtonState) generateRandomColor = !generateRandomColor;	// invert generateRandomColor @positive edge 

  oldButtonState = ButtonState;                                                 // reset oldButtonState
  delay(10);                                                                    // wait for 10ms
}
