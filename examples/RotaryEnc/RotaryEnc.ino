/*

  MIT License

  Copyright (c) 2023 wlowi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "TextUI.h"

/* Enable one of these: */
// #define USE_SSD1306
#define USE_ST7735


#ifdef USE_SSD1306
# include "TextUILcdSSD1306.h"
#endif

#ifdef USE_ST7735
# include "TextUILcdST7735.h"
# define PIN_TFT_CS  10
# define PIN_TFT_DC   9
# define PIN_TFT_RST -1

/* Pins for Arduino Nano:
 *  CS               10
 *  A0/DC             9
 *  RESET            -1 (unused, TFT connected to reset)
 *  SDA/MOSI         11
 *  SCK              13
 */
 
#endif

#include "TextUIRotaryEncoder.h"
#define PIN_CLK     2
#define PIN_DIR     3
#define PIN_BUTTON  4

#include "HomeScreen.h"

TextUI textUI;

void setup()
{
#ifdef USE_SSD1306
    // textUI.setDisplay( new TextUILcdSSD1306( &SH1106_128x64));
    /* SH1306 Controller */
    textUI.setDisplay( new TextUILcdSSD1306( &Adafruit128x64));
#endif
#ifdef USE_ST7735
    // INTR_144GREENTAB       TFT 128x128 pixel, 1.44 inch
    // INTR_BLACKTAB          TFT 160x128 pixel; 1.8 inch
    textUI.setDisplay( new TextUILcdST7735(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST, INITR_144GREENTAB));
#endif

    textUI.getDisplay()->setFontSize( TEXTUI_FONT_MEDIUM);

    textUI.setInput( new TextUIRotaryEncoder( PIN_CLK, PIN_DIR, PIN_BUTTON));

    textUI.setHomeScreen( new HomeScreen());
}

void loop()
{
    Event *e = textUI.getEvent();

    textUI.handle(e);
}
