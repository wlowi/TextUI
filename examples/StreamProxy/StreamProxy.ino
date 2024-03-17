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

#include "TextUIStreamProxy.h"

#include "TextUIRotaryEncoder.h"
#define PIN_CLK     2
#define PIN_DIR     3
#define PIN_BUTTON  4

#include "HomeScreen.h"

TextUI textUI;

void setup()
{
    delay(2000);
    
    Serial.begin(19200);
    
    TextUIStreamProxy *streamProxy = new TextUIStreamProxy( Serial);
    textUI.setDisplay( streamProxy);

    textUI.getDisplay()->setFontSize( TEXTUI_FONT_MEDIUM);

    textUI.setInput( new TextUIRotaryEncoder( PIN_CLK, PIN_DIR, PIN_BUTTON));

    textUI.setHomeScreen( new HomeScreen());
}

void loop()
{
    Event *e = textUI.getEvent();

    textUI.handle(e);
}
