/*
  TextUI. A simple text based UI.

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

#ifndef _TextUIStreamProxy_h_
#define _TextUIStreamProxy_h_

#include "TextUI.h"

/**
 * @brief A driver for buttons attached to Arduino input pins.
 * 
 * The driver distinguishes between short and long press of a button.
 * It also supports auto repeat. 
 * 
 * A short press (shorter than BUTTON_LONG_TIMEOUT_msec, currently 250 mSec) 
 * creates an Event with a corresponding short key defined during construction 
 * of the driver. 
 * 
 * A long press (between BUTTON_LONG_TIMEOUT_msec and BUTTON_REPEAT_TIMEOUT_msec)
 * creates an Event with a corresponding long key defined during construction
 * of the driver.
 * 
 * If the button is pressed longer than BUTTON_REPEAT_TIMEOUT_msec, the short key
 * Event is repeated every BUTTON_REPEAT_SPEED_msec (currently 50 mSec) until the
 * button is released.
 * 
 *  Example:   
 *  
 *      #include "TextUI.h"
 *      #include "TextUISimpleKbd.h"
 * 
 *      const uint8_t BUTTON_COUNT = 3;
 *      uint8_t buttons[BUTTON_COUNT] = { 14, 15, 16 };
 *      uint8_t shortKeys[BUTTON_COUNT] = { KEY_UP, KEY_ENTER, KEY_DOWN };
 *      uint8_t longKeys[BUTTON_COUNT] = { KEY_NONE, KEY_BACK, KEY_CLEAR };
 *
 *      TextUI textUI;
 *      textUI.setInput( new TextUISimpleKbd( BUTTON_COUNT, buttons, shortKeys, longKeys));
 */
class TextUIStreamProxy : public TextUIInput, TextUILcd {
    
private:
    Stream& stream;

public:
    TextUIStreamProxy( Stream& s);

    /* TextUIInput */
    bool pending();
    void setEvent( Event *e);


    /* TextUILcd */
    void clear();
    void clearEOL();

    bool colorSupport();
    
    void setBg( uint8_t r, uint8_t g, uint8_t b);
    void setFg( uint8_t r, uint8_t g, uint8_t b);

    void normalColors();
    void selectedColors();
    void editColors();

    void setInvert( bool inv);

    /* FONT_SMALL .. FONT_LARGE */
    void setFontSize( FontSize_t sz);
    
    uint8_t getRows();
    uint8_t getColumns();

    /* row and column in characters */
    void setCursor( uint8_t r, uint8_t c);
    void setRow( uint8_t r);
    void setColumn( uint8_t c);

    void printChar( char ch);
};

#endif

