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

#include "TextUILcdHD44780_I2C.h"

/* Flags for lcdWrite()
 *
 * DATA         => RS high
 * COMMAND      => RS low
 * COMMAND_HALF => RS low, write upper 4 bits only
 */
#define LCD2004I2C_DATA         ((byte)0)
#define LCD2004I2C_COMMAND      ((byte)1)
#define LCD2004I2C_COMMAND_HALF ((byte)2)

#define LCD2004I2C_BUSY_FLAG    ((byte)0x80)

// Switch to 4 bit mode and set or reset RE bit
#define LCD2004I2C_MODE8RE0     ((byte)0x30 | Nbit)

// Switch to 4 bit mode and set or reset RE bit
#define LCD2004I2C_MODE4RE0     ((byte)0x20 | Nbit)
#define LCD2004I2C_MODE4RE1     ((byte)0x24 | Nbit)
#define LCD2004I2C_ENTRYMODE    ((byte)0x04 | IDbit)
#define LCD2004I2C_DISPCTRL     ((byte)0x08 | BCDbit)
#define LCD2004I2C_DDRAM        ((byte)0x80)
#define LCD2004I2C_CGRAM        ((byte)0x40)

#define LCD2004I2C_CMD_CLEAR    ((byte)0x01)
#define LCD2004I2C_CMD_HOME     ((byte)0x02)


// I2C interface lower 4 bits
/* Register select: 0 = Command; 1 = Data */
#define LCD2004I2C_RS           ((byte)0x01)
/* Read/Write: 0 = write; 1 = read */
#define LCD2004I2C_RW           ((byte)0x02)
/* Clock enable: 0 ==> 1 ==> 0 */
#define LCD2004I2C_EN           ((byte)0x04)
/* Display backlight: 1 = on */
#define LCD2004I2C_BACKLIGHT    ((byte)0x08)
#define LCD2004I2C_NOBACKLIGHT  ((byte)0x00)


/* lcdControl Flags */
#define ARDLCD_F_NONE           ((byte)0x00)
#define ARDLCD_F_DISPLAY_ON     ((byte)0x01)
#define ARDLCD_F_CURSOR_ON      ((byte)0x02)
#define ARDLCD_F_CURSOR_BLINK   ((byte)0x04)


/* Line capabilites */
typedef struct lcd2004i2cLineCaps_t {
    byte lines;      // Number of lines
    byte maxColumns; // Max columns supported
    byte offset;     // DDRAM offset per line
} lcd2004i2cLineCaps_t;

static const lcd2004i2cLineCaps_t lcd2004i2cLineCaps[] = {

#define LCD2004I2C_LINE_CAPS ((byte)4)

  { 1, 52, 0x00 },
  { 2, 32, 0x40 },
  { 3, 20, 0x14 },
  { 4, 20, 0x54 }
};

LCD2004I2C::LCD2004I2C(TwoWire* ifc, byte addr) {

    i2cAddr = addr;
    twi = ifc;
}

boolean LCD2004I2C::init(byte columns, byte lines) {

    twi->begin();
    delay(50);

    /*
    if ((twi == NULL)
        || twi->checkDevice(i2cAddr) != TWI_RET_OK) {
        return false;
    }
    */

    if (!setCaps(lines, columns)) {
        return false;
    }

    backlight = LCD2004I2C_NOBACKLIGHT;
    dataWrite4Bits(0);
    delay(100);

    // Initialization according to HD44780 datasheet

    // Switch to 8 bit mode
    lcdWriteCommandHalf(LCD2004I2C_MODE8RE0);
    delay(5);
    lcdWriteCommandHalf(LCD2004I2C_MODE8RE0);
    delay(5);
    lcdWriteCommandHalf(LCD2004I2C_MODE8RE0);
    delay(1);

    // Switch to 4 bit mode
    lcdWriteCommandHalf(LCD2004I2C_MODE4RE0);
    delay(1);

    displayControl();

    backlight = LCD2004I2C_BACKLIGHT;

    clear();

    lcdWriteCommand(LCD2004I2C_MODE4RE0);
    lcdWriteCommand(LCD2004I2C_ENTRYMODE);

    home();

    return true;
}

byte LCD2004I2C::lcdLines() {

    return hwLines;
}

byte LCD2004I2C::lcdColumns() {

    return hwColumns;
}

void LCD2004I2C::clear() {

    lcdWriteCommand(LCD2004I2C_CMD_CLEAR);
    delay(2);
}

void LCD2004I2C::home() {

    lcdWriteCommand(LCD2004I2C_MODE4RE0);
    lcdWriteCommand(LCD2004I2C_CMD_HOME);
    delay(2);
}

void LCD2004I2C::lcdSetCursor(byte line, byte col) {

    byte addr = LCD2004I2C_DDRAM | (lcd2004i2cLineCaps[line].offset + col);

    lcdWriteCommand(LCD2004I2C_MODE4RE0);
    lcdWriteCommand(addr);
}

void LCD2004I2C::lcdSetInvertMode(bool inv) {

}

void LCD2004I2C::lcdWriteData(byte data) {

    lcdWrite(LCD2004I2C_DATA, data);
}

byte LCD2004I2C::lcdReadData() {

    return lcdRead(LCD2004I2C_DATA);
}

void LCD2004I2C::lcdControl(byte flags) {

    BCDbit = 0;

    BCDbit |= flags & ARDLCD_F_DISPLAY_ON ? DbitOn : 0;

    backlight = flags & ARDLCD_F_DISPLAY_ON ? LCD2004I2C_BACKLIGHT
        : LCD2004I2C_NOBACKLIGHT;

    BCDbit |= flags & ARDLCD_F_CURSOR_ON ? CbitOn : 0;
    BCDbit |= flags & ARDLCD_F_CURSOR_BLINK ? BbitOn : 0;

    displayControl();
}

/********************* PRIVATE ************************/

void LCD2004I2C::displayControl() {

    lcdWriteCommand(LCD2004I2C_MODE4RE0);
    lcdWriteCommand(LCD2004I2C_DISPCTRL);
}

/* Write upper 4 bits, then lower 4 bits.
 * RS = low
 */
void LCD2004I2C::lcdWriteCommand(byte data) {

    lcdWrite(LCD2004I2C_COMMAND, data);
}

/* Write upper 4 bits only.
 * RS = low
 */
void LCD2004I2C::lcdWriteCommandHalf(byte data) {

    lcdWrite(LCD2004I2C_COMMAND_HALF, data & 0xf0);
}

void LCD2004I2C::lcdWrite(byte what, byte data) {

    byte rs = (what == LCD2004I2C_DATA) ? LCD2004I2C_RS : 0;
    byte h = (data & 0xf0) | rs;
    byte l = ((data << 4) & 0xf0) | rs;

    dataWrite4Bits(h);
    clockEnable(h, false);

    if (what != LCD2004I2C_COMMAND_HALF) {
        dataWrite4Bits(l);
        clockEnable(l, false);
    }
}

byte LCD2004I2C::lcdReadCommand() {

    return lcdRead(LCD2004I2C_COMMAND);
}

byte LCD2004I2C::lcdRead(byte what) {

    byte data;

    byte rs = (what == LCD2004I2C_DATA) ? LCD2004I2C_RS : 0;

    /* 0xf0 makes sure the upper 4 bits are high so that the LCD controller can
     * pull them down when data it puts onto the bus.
     */
    byte b = 0xf0 | rs | LCD2004I2C_RW;

    dataWrite4Bits(b);
    delayMicroseconds(1);

    data = clockEnable(b, true) & 0xf0;
    data |= (clockEnable(b, true) >> 4) & 0x0f;

    return data;
}

boolean LCD2004I2C::setCaps(byte lines, byte cols) {

    const lcd2004i2cLineCaps_t* caps;

    for (byte i = 0; i < LCD2004I2C_LINE_CAPS; i++) {
        caps = &lcd2004i2cLineCaps[i];

        if (lines == caps->lines && cols <= caps->maxColumns) {
            hwLines = lines;
            hwColumns = cols;

            Nbit = (lines >= 2) ? NbitOn : bitOff;

            return true;
        }
    }

    return false;
}

void LCD2004I2C::dataWrite4Bits(byte data) {

    twi->beginTransmission(i2cAddr);
    twi->write(data | backlight);
    twi->endTransmission();
}

/* Flash EN line.
 * If withRead is set to true is also tries to read data from the i2c expander.
 */
byte LCD2004I2C::clockEnable(byte b, boolean withRead) {

    byte data = 0;

    dataWrite4Bits(b | LCD2004I2C_EN);
    delayMicroseconds(1);

    if (withRead) {
        twi->requestFrom(i2cAddr, 1);
    }

    dataWrite4Bits(b & ~LCD2004I2C_EN);
    delayMicroseconds(50);

    if (withRead) {
        data = twi->read();
    }

    return data;
}


TextUILcdHD44780_I2C::TextUILcdHD44780_I2C(TwoWire* twi, byte addr) {

    flags = ARDLCD_F_DISPLAY_ON;

    lcd = new LCD2004I2C(twi, addr);
    lcd->init(20, 4);

    delay(100);

    clear();
    row = col = 0;
    setCursor(row, col);

    displayOn( true);
    // cursorOn( true);
    cursorBlink( true);
}

void TextUILcdHD44780_I2C::displayOn( boolean b) {

  b ? flags |= ARDLCD_F_DISPLAY_ON : flags &= ~ARDLCD_F_DISPLAY_ON;
  lcd->lcdControl( flags);
}

void TextUILcdHD44780_I2C::cursorOn( boolean b) {

  b ? flags |= ARDLCD_F_CURSOR_ON : flags &= ~ARDLCD_F_CURSOR_ON;
  lcd->lcdControl( flags);
}

void TextUILcdHD44780_I2C::cursorBlink( boolean b) {

  b ? flags |= ARDLCD_F_CURSOR_BLINK : flags &= ~ARDLCD_F_CURSOR_BLINK;
  lcd->lcdControl( flags);
}

void TextUILcdHD44780_I2C::clear() {

    lcd->clear();
}

void TextUILcdHD44780_I2C::clearEOL() {

    for (byte c = col; c < lcd->lcdColumns(); c++) {
        lcd->lcdWriteData(' ');
    }

    setCursor(row, col);
}

bool TextUILcdHD44780_I2C::colorSupport() {

    return false;
}

void TextUILcdHD44780_I2C::setBg(uint8_t r, uint8_t g, uint8_t b) {

  /* not supported */
}

void TextUILcdHD44780_I2C::setFg(uint8_t r, uint8_t g, uint8_t b) {

  /* not supported */
}

void TextUILcdHD44780_I2C::normalColors() {

    setInverse(false);
}

void TextUILcdHD44780_I2C::selectedColors() {

    setInverse(true);
}

void TextUILcdHD44780_I2C::editColors() {

    setInverse(true);
}

bool TextUILcdHD44780_I2C::inverseSupport() {

    return false;
}

void TextUILcdHD44780_I2C::setInverse(bool inv) {

    lcd->lcdSetInvertMode(inv);
}

void TextUILcdHD44780_I2C::setFontSize(FontSize_t sz) {

    /* not supported */
}

uint8_t TextUILcdHD44780_I2C::getRows() {

    return lcd->lcdLines();
}

uint8_t TextUILcdHD44780_I2C::getColumns() {

    return lcd->lcdColumns();
}

void TextUILcdHD44780_I2C::setCursor(uint8_t r, uint8_t c) {

    row = r;
    col = c;
    lcd->lcdSetCursor(row, col);
}

void TextUILcdHD44780_I2C::setRow(uint8_t r) {

    row = r;
    lcd->lcdSetCursor(row, col);
}

void TextUILcdHD44780_I2C::setColumn(uint8_t c) {

    col = c;
    lcd->lcdSetCursor(row, col);
}

void TextUILcdHD44780_I2C::printChar(char ch) {

    lcd->lcdWriteData(ch);
    col++;

    if (col >= lcd->lcdColumns()) {
        row++;
        col = 0;
        if (row >= lcd->lcdLines()) {
            row = lcd->lcdLines() -1;
        }
        setCursor(row, col);
    }
}
