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

/* current mode */
const uint8_t COMMAND_MODE = 0;
const uint8_t PRINT_MODE = 1;

/* commands */
const char CMD_QUERY = '?';
const char CMD_COMMAND = '!';
/* Set cursor and switch to print mode */
const char CMD_SET_CURSOR = '@';
const char CMD_SET_ROW = 'R';
const char CMD_SET_COLUMN = 'C';
/* Switch to print mode */
const char CMD_PRINT = '.';
/* End of current command, switch to COMMAND_MODE */
const char CMD_END = '\n';

const char FIRST_PRINTABLE = ' ';

#define IS_PRINTABLE( ch)  (((ch) >= FIRST_PRINTABLE) && ((ch) != char(127)))

using commandType_t = uint16_t;
#define COMMAND_TYPE( a, b)  ((((commandType_t)a) << 8) +  b)

#define COMMAND_CH1(cmd)  ((char)((cmd >> 8) & 0xff))
#define COMMAND_CH2(cmd)  ((char)(cmd & 0xff))


#define COMMAND_CLEAR                COMMAND_TYPE( 'C','L' )
#define COMMAND_CLEAR_EOL            COMMAND_TYPE( 'C','E' )
#define COMMAND_NORMAL_COLORS        COMMAND_TYPE( 'N','C' )
#define COMMAND_SELECTED_COLORS      COMMAND_TYPE( 'S','C' )
#define COMMAND_EDIT_COLORS          COMMAND_TYPE( 'E','C' )
#define COMMAND_SET_INVERT           COMMAND_TYPE( 'I','V' )
#define COMMAND_SET_FONTSIZE         COMMAND_TYPE( 'F','S' )
#define COMMAND_SET_FG               COMMAND_TYPE( 'F','G' )
#define COMMAND_SET_BG               COMMAND_TYPE( 'B','G' )

#define COMMAND_GET_ROWS             COMMAND_TYPE( 'G','R' )
#define COMMAND_GET_COLUMNS          COMMAND_TYPE( 'G','C' )
#define COMMAND_GET_COLORSUPPORT     COMMAND_TYPE( 'G','M' )


/**
 * @brief A proxy driver that supports both TextUIInput, TextUILcd.
 * 
 */
class TextUIStreamProxy : public TextUIInput, public TextUILcd {
    
private:
    Stream& stream;

    uint8_t currentMode;

    void toCommandMode();
    void toPrintMode();

    void simpleCommand( commandType_t cmd);

    void byteCommand( commandType_t cmd, uint8_t u);
    void byteCommand2( commandType_t cmd, uint8_t u, uint8_t v);
    void byteCommand3( commandType_t cmd, uint8_t u, uint8_t v, uint8_t w);

    void queryCommand( commandType_t cmd);

    void send( char ch);
    void sendByte( uint8_t ch);

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

