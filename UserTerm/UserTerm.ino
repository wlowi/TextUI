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

#include "avr/sleep.h"

#include "TextUI.h"
#include "TextUIStreamProxy.h"
#include "ReliableStream.h"

#define USER_TERM_VERSION "0.3.0"


/* ****** LCD / TFT CONFIGURATION ************** */
/* Enable ONE of these: */

/* OLED 0.96 or 1.3 inch. SSD1306 or SH1106 controller */
// #define USE_SSD1306

/* TFT 1.8 inch. 128x128 / 160x80 / 160x128 */
#define USE_ST7735

/* TFT 320x240 */
// #define USE_ILI9341

/* ********************************************* */


/* ****** INPUT CONFIGURATION ****************** */
/* Enable ONE of these: */

/* Rotary encoder, UP, DOWN, ENTER/CLEAR */
#define USE_ROTARY_ENCODER

/* 3 Buttons, UP, DOWN, ENTER/CLEAR */
// #define USE_KEYBOARD

/* ********************************************* */


#ifdef USE_SSD1306
# include "TextUILcdSSD1306.h"
#endif


#ifdef USE_ST7735
# include "TextUILcdST7735.h"

/* Pins for Arduino Nano and Pro Mini:
 *  CS               10
 *  A0/DC             9
 *  RESET            -1 (unused, TFT connected to reset)
 *  SDA/MOSI         11
 *  SCK              13
 */
# define PIN_TFT_CS  10
# define PIN_TFT_DC   9
# define PIN_TFT_RST -1

#endif


#ifdef USE_ILI9341
# include "TextUILcdILI9341.h"

/* Pins for Arduino Nano and Pro Mini:
 *  CS               10
 *  A0/DC             9
 *  RESET            -1 (unused, TFT connected to reset)
 *  SDA/MOSI         11
 *  SCK              13
 */
# define PIN_TFT_CS  10
# define PIN_TFT_DC   9
# define PIN_TFT_RST -1

#endif


#ifdef USE_ROTARY_ENCODER
# include "TextUIRotaryEncoder.h"

/* Pins for Arduino Nano and Pro Mini:
 */
# define PIN_CLK     2
# define PIN_DIR     3
# define PIN_BUTTON  4

#endif


#ifdef USE_KEYBOARD
# include "TextUISimpleKbd.h"

/* Pins for Arduino Nano and Pro Mini:
 */
# define BUTTONS     3

# define PIN_UP      2
# define PIN_ENTER   3
# define PIN_DOWN    4

uint8_t ports[BUTTONS] = { PIN_UP, PIN_ENTER, PIN_DOWN };
uint8_t shortPress[BUTTONS] = { KEY_UP, KEY_ENTER, KEY_DOWN };
uint8_t longPress[BUTTONS] = { KEY_BACK, KEY_CLEAR, KEY_RESET };

#endif


/* *************** END CONFIGURATION **************** */


bool readByte(uint8_t* b);

TextUI textUI;
uint8_t currentMode;
TextUILcd* display;
ReliableStream *stream;

void setup() {

    Serial.begin(57600, SERIAL_8N1);
    stream = new ReliableStream( Serial, 64, 256);

#ifdef USE_SSD1306
    /* SH1106 Controller */
    // textUI.setDisplay( new TextUILcdSSD1306( &SH1106_128x64));
    /* SSD1306 Controller */
    display = new TextUILcdSSD1306(&Adafruit128x64);
#endif

#ifdef USE_ST7735
    // INTR_144GREENTAB       TFT 128x128 pixel, 1.44 inch
    // INTR_BLACKTAB          TFT 160x128 pixel; 1.8 inch
    display = new TextUILcdST7735(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST, INITR_BLACKTAB);
#endif

#ifdef USE_ILI9341
    display = new TextUILcdILI9341(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
#endif

    textUI.setDisplay(display);
    display->setFontSize(TEXTUI_FONT_SMALL);


#ifdef USE_ROTARY_ENCODER
    textUI.setInput(new TextUIRotaryEncoder(PIN_CLK, PIN_DIR, PIN_BUTTON));
#endif

#ifdef USE_KEYBOARD
    textUI.setInput(new TextUISimpleKbd( BUTTONS, ports, shortPress, longPress));
#endif

    currentMode = COMMAND_MODE;

    display->clear();
    display->printStr("UserTerm ");
    display->printStr(USER_TERM_VERSION);
}

void loop() {

    cpuSleep();

    Event* e = textUI.getEvent();

    if (e->getType() == EVENT_TYPE_KEY) {
        stream->write(CMD_KBD);
        sendByte(e->getKey());
        sendByte(e->getCount());
        stream->write(CMD_END);
    }

    e->markProcessed();

    processSerial();
}

void processSerial() {

    while (stream->available()) {

        switch (currentMode) {
        case COMMAND_MODE:
            processCommandMode();
            break;

        case PRINT_MODE:
            processPrintMode();
            break;
        }
    }

    stream->handleComm();
}

void processCommandMode() {

    int ch = stream->read();
    uint8_t r;
    uint8_t c;

    switch (ch) {
    case CMD_ATTN:
        currentMode = COMMAND_MODE;
        break;

    case CMD_QUERY: // '?'
        processQueryCommand();
        break;

    case CMD_COMMAND: // '!'
        processSetCommand();
        break;

    case CMD_SET_CURSOR: // '@'
        if (readByte(&r) && readByte(&c)) {
            display->setCursor(r, c);
        }
        currentMode = PRINT_MODE;
        break;

    case CMD_SET_ROW: // 'R'
        if (readByte(&r)) {
            display->setRow(r);
        }
        currentMode = PRINT_MODE;
        break;

    case CMD_SET_COLUMN: // 'C' 
        if (readByte(&c)) {
            display->setColumn(c);
        }
        currentMode = PRINT_MODE;
        break;

    case CMD_SET_NORMAL: // 'N'
        display->normalColors();
        currentMode = PRINT_MODE;
        break;

    case CMD_SET_SELECTED: // 'S'
        display->selectedColors();
        currentMode = PRINT_MODE;
        break;

    case CMD_SET_EDIT: // 'E'
        display->editColors();
        currentMode = PRINT_MODE;
        break;

    case CMD_CLEAR: // 'X'
        display->clear();
        currentMode = PRINT_MODE;
        break;

    case CMD_CLEAREOL: // 'L'
        display->clearEOL();
        currentMode = PRINT_MODE;
        break;

    case CMD_PRINT: // '.'
        currentMode = PRINT_MODE;
        break;
    }
}

void processPrintMode() {

    int ch = stream->read();

    if (ch == CMD_ATTN) {
        currentMode = COMMAND_MODE;
    }
    else {
        if (IS_PRINTABLE(ch)) {
            display->printChar(ch);
        }
    }
}

void processSetCommand() {

    char c1;
    char c2;

    commandType_t cmd;

    uint8_t r;
    uint8_t g;
    uint8_t b;

    if (readChar(&c1) && readChar(&c2)) {

        cmd = COMMAND_TYPE(c1, c2);

        switch (cmd) {
        case COMMAND_SET_INVERT:
            if (readByte(&b)) {
                display->setInvert((bool)b);
            }
            break;

        case COMMAND_SET_FONTSIZE:
            if (readByte(&b)) {
                display->setFontSize((FontSize_t)b);
            }
            break;

        case COMMAND_SET_FG:
            if (readByte(&r) && readByte(&g) && readByte(&b)) {
                display->setFg(r, g, b);
            }
            break;

        case COMMAND_SET_BG:
            if (readByte(&r) && readByte(&g) && readByte(&b)) {
                display->setBg(r, g, b);
            }
            break;
        }
    }
}

void processQueryCommand() {

    char c1;
    char c2;

    commandType_t cmd;

    if (readChar(&c1) && readChar(&c2)) {

        cmd = COMMAND_TYPE(c1, c2);

        switch (cmd) {
        case COMMAND_GET_COLORSUPPORT:
            stream->write(CMD_QUERY_RESULT);
            sendByte(display->colorSupport());
            stream->write(CMD_END);
            break;

        case COMMAND_GET_ROWS:
            stream->write(CMD_QUERY_RESULT);
            sendByte(display->getRows());
            stream->write(CMD_END);
            break;

        case COMMAND_GET_COLUMNS:
            stream->write(CMD_QUERY_RESULT);
            sendByte(display->getColumns());
            stream->write(CMD_END);
            break;
        }
    }
}

bool readChar(char* b) {

    /* Read with timeout */
    return (stream->readBytes(b, 1) == 1);
}

void sendByte(uint8_t b) {

    if( b < 64) {
        stream->write(b + SINGLECHAR_OFFSET);
    } else {
        stream->write(((b >> 4) & 0x0f) + TWOCHAR_OFFSET);
        stream->write((b & 0x0f) + TWOCHAR_OFFSET);
    }
}

bool readByte(uint8_t* b) {

    uint8_t buffer[2];

    if( stream->readBytes( &buffer[0], 1) == 1) {
        if( buffer[0] >= SINGLECHAR_OFFSET) {
            *b = (buffer[0] - SINGLECHAR_OFFSET);
            return true;
        } else if( (buffer[0] >= TWOCHAR_OFFSET) && (stream->readBytes( &buffer[1], 1) == 1) ) {
            *b = ((buffer[0] - TWOCHAR_OFFSET) << 4) | (buffer[1] - TWOCHAR_OFFSET);
            return true;
        }
    }
    
    return false;
}

void cpuSleep()  {

    set_sleep_mode( SLEEP_MODE_IDLE);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}