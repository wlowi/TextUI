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
#include "ReliableStream.h"

#include "HomeScreen.h"

TextUI textUI;

void setup()
{
    Serial.begin(57600, SERIAL_8N1);
    ReliableStream *stream = new ReliableStream( Serial, 256, 64);
    TextUIStreamProxy *streamProxy = new TextUIStreamProxy( *stream);

    textUI.setDisplay( streamProxy);
    textUI.getDisplay()->setFontSize( TEXTUI_FONT_MEDIUM);

    textUI.setInput( streamProxy);

    textUI.setHomeScreen( new HomeScreen());
}

void loop()
{
    Event *e = textUI.getEvent();

    textUI.handle(e);
}
