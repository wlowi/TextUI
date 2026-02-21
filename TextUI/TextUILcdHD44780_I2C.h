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

#ifndef _TextUIHD44780_I2C_h
#define _TextUIHD44780_I2C_h

#include "Wire.h"
#include "TextUI.h"


#define bitOff   ((byte)0x00)
#define NbitOn   ((byte)0x08)
#define DbitOn   ((byte)0x04)
#define CbitOn   ((byte)0x02)
#define BbitOn   ((byte)0x01)
#define IDbitOn  ((byte)0x02)


class LCD2004I2C {

private:
    byte i2cAddr;
    TwoWire* twi;

    /* Hardware capabilities of the display */
    byte hwLines;
    byte hwColumns;

    byte backlight;
    byte Nbit;   // 1 or 2 lines
    byte IDbit = IDbitOn; // Increment
    byte BCDbit = DbitOn;  // Display on; Cursor off; Cursor blink off

public:

    LCD2004I2C(TwoWire* ifc, byte addr);

    boolean init(byte columns, byte lines);

    byte lcdLines();
    byte lcdColumns();
    void clear();
    void home();
    void lcdSetCursor(byte line, byte col);
    void lcdSetInvertMode(bool inv);

    void lcdWriteData(byte data);
    byte lcdReadData();
    void lcdControl(byte flags);

    void lcdWriteCommand(byte data);

private:

    void displayControl();


    void lcdWriteCommandHalf(byte data);
    void lcdWrite(byte what, byte data);

    byte lcdReadCommand();
    byte lcdRead(byte what);

    boolean setCaps(byte lines, byte cols);

    void dataWrite4Bits(byte b);
    byte clockEnable(byte b, boolean doRead);
};


/**
 * @brief A driver for OLED displays with HD44780 and similar controller.
 *
 * Example:
 *
 *  #include "TextUI.h"
 *  #include "TextUILcdHD44780_I2C.h"
 *
 *  /o Raspberry RP 2040 support multiple pin configurations o/
 *  Wire.setSDA(PIN_LCD_SDA);
 *  Wire.setSCL(PIN_LCD_SCL);
 *
 *  textUI.setDisplay( new TextUILcdHD44780_I2C( &Wire, 0x27 /o i2c addr o/ );
 */
class TextUILcdHD44780_I2C : public TextUILcd {

private:
    LCD2004I2C* lcd;
    byte row;
    byte col;
    byte flags;

public:
  /**
   * @brief Construct a new TextUIHD44780_I2C driver.
   *
   * @param twi  The I2C device to use.
   * @param addr I2C Address
   */
    TextUILcdHD44780_I2C(TwoWire* twi, byte addr);

    void displayOn( boolean b);

    void commit() {}

    void cursorOn( boolean b);
    void cursorBlink( boolean b);

    void clear() final;
    void clearEOL() final;

    bool colorSupport() final;

    void setBg(uint8_t r, uint8_t g, uint8_t b) final;
    void setFg(uint8_t r, uint8_t g, uint8_t b) final;

    void normalColors() final;
    void selectedColors() final;
    void editColors() final;

    bool inverseSupport() final;
    void setInverse(bool inv) final;

    /* FONT_SMALL .. FONT_LARGE */
    void setFontSize(FontSize_t sz) final;

    uint8_t getRows() final;
    uint8_t getColumns() final;

    /* row and column in characters */
    void setCursor(uint8_t r, uint8_t c) final;
    void setRow(uint8_t r) final;
    void setColumn(uint8_t c) final;

    void printChar(char ch) final;
};

#endif
