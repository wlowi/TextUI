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

/* ******************** */
/* Enable one of these: */

/* OLED 0.96 or 1.3 inch. SSD1306 or SH1106 controller */
// #define USE_SSD1306

/* TFT 128x128 / 160x80 / 160x128 */
#define USE_ST7735

/* TFT 320x240 */
// #define USE_ILI9341
/* ******************** */

#ifdef USE_SSD1306
# include "TextUILcdSSD1306.h"
#endif

#ifdef USE_ST7735
# include "TextUILcdST7735.h"
# define PIN_TFT_CS  10
# define PIN_TFT_DC   9
# define PIN_TFT_RST -1

/* Pins for Arduino Nano and Pro Mini:
 *  CS               10
 *  A0/DC             9
 *  RESET            -1 (unused, TFT connected to reset)
 *  SDA/MOSI         11
 *  SCK              13
 */

#endif

#ifdef USE_ILI9341
# include "TextUILcdILI9341.h"
# define PIN_TFT_CS  10
# define PIN_TFT_DC   9
# define PIN_TFT_RST -1

/* Pins for Arduino Nano and Pro Mini:
 *  CS               10
 *  A0/DC             9
 *  RESET            -1 (unused, TFT connected to reset)
 *  SDA/MOSI         11
 *  SCK              13
 */

#endif

#include "TextUIRotaryEncoder.h"
/* Pins for Arduino Nano and Pro Mini:
 */
#define PIN_CLK     2
#define PIN_DIR     3
#define PIN_BUTTON  4

bool readByte(uint8_t* b);

TextUI textUI;
uint8_t currentMode;
TextUILcd* display;

void setup() {

    Serial.begin(19200, SERIAL_8N1);
    Serial.setTimeout(100);

#ifdef USE_SSD1306
    /* SH1106 Controller */
    // textUI.setDisplay( new TextUILcdSSD1306( &SH1106_128x64));
    /* SSD1306 Controller */
    display = new TextUILcdSSD1306(&Adafruit128x64);
#endif

#ifdef USE_ST7735
    // INTR_144GREENTAB       TFT 128x128 pixel, 1.44 inch
    // INTR_BLACKTAB          TFT 160x128 pixel; 1.8 inch
    display = new TextUILcdST7735(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST, INITR_144GREENTAB);
#endif

#ifdef USE_ILI9341
    display = new TextUILcdILI9341(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
#endif

    textUI.setDisplay(display);
    display->setFontSize(TEXTUI_FONT_SMALL);

    textUI.setInput(new TextUIRotaryEncoder(PIN_CLK, PIN_DIR, PIN_BUTTON));

    currentMode = COMMAND_MODE;

    display->clear();
    display->printStr("UserTerm");
}

void loop() {

    Event* e = textUI.getEvent();

    if (e->getType() == EVENT_TYPE_KEY) {
        Serial.write(CMD_KBD);
        Serial.write(e->getKey());
        Serial.write(e->getCount());
        Serial.write(CMD_END);
    }

    e->markProcessed();

    processSerial();
}

void processSerial() {

    while (Serial.available()) {

        switch (currentMode) {
        case COMMAND_MODE:
            processCommandMode();
            break;

        case PRINT_MODE:
            processPrintMode();
            break;
        }
    }
}

void processCommandMode() {

    int ch = Serial.read();
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

    int ch = Serial.read();

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

    uint8_t c1;
    uint8_t c2;

    if (readByte(&c1) && readByte(&c2)) {

        commandType_t cmd = COMMAND_TYPE(c1, c2);

        uint8_t r;
        uint8_t g;
        uint8_t b;

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

    uint8_t c1;
    uint8_t c2;

    if (readByte(&c1) && readByte(&c2)) {

        commandType_t cmd = COMMAND_TYPE(c1, c2);

        switch (cmd) {
        case COMMAND_GET_COLORSUPPORT:
            Serial.write(CMD_QUERY_RESULT);
            Serial.write(display->colorSupport());
            Serial.write(CMD_END);
            break;

        case COMMAND_GET_ROWS:
            Serial.write(CMD_QUERY_RESULT);
            Serial.write(display->getRows());
            Serial.write(CMD_END);
            break;

        case COMMAND_GET_COLUMNS:
            Serial.write(CMD_QUERY_RESULT);
            Serial.write(display->getColumns());
            Serial.write(CMD_END);
            break;
        }
    }
}

bool readByte(uint8_t* b) {

    /* Read with timeout */
    return (Serial.readBytes(b, 1) == 1);
}
