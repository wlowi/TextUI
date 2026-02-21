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

#ifndef _TextUILcdGxEPD2_h_
#define _TextUILcdGxEPD2_h_

#include "TextUI.h"

#include <GxEPD2_BW.h>
//#include <GxEPD2_3C.h>
//#include <GxEPD2_4C.h>
//#include <GxEPD2_7C.h>

//#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
//#include <Fonts/FreeMonoBold18pt7b.h>
//#include <Fonts/FreeMonoBold24pt7b.h>

#include <cstdint>
#include "TextUILcdGxEPD2_display_selection.h"

typedef uint16_t pixel;

/**
 * @brief A driver for ePaper displays.
 *
 * Example:
 *
 *      #include "TextUI.h"
 *      #include "TextUILcdGxEPD2.h"
 *
 *      #define TFT_CS        10
 *      #define TFT_DC         9
 *      #define TFT_RST       -1 // Or set to -1 and connect to Arduino RESET pin
 *      // It also uses the board specific hardware SPI pins.
 *
 *      TextUI textUi;
 *      textUi.setDisplay( new TextUILcdGxEPD2( TFT_CS, TFT_DC, TFT_RST));
 */
class TextUILcdGxEPD2 : public TextUILcd
{
    private:
        GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> *display;

        unsigned int width;
        unsigned int height;
        uint16_t font_h;
        uint16_t font_w;
        uint16_t subscript_h;

        pixel fgCol565;
        pixel bgCol565;
        bool invers;
        uint8_t fontSz; // 1 - 3
        unsigned int textX;
        unsigned int textY;

        // partial update area
        unsigned int updX;
        unsigned int updY;
        unsigned int updW;
        unsigned int updH;

        uint8_t partialUpdates;
#define PARTIAL_UPDATE_LIMIT 20

        pixel rgbToCol565( uint8_t r, uint8_t g, uint8_t b);
        void initDisplay();
        void extend(unsigned int x,unsigned int y,unsigned int w,unsigned int h);

    public:
      /**
       * @brief Construct a new TextUILcdGxEPD2 driver.
       *
       * This driver used the board specific hardware SPI interface
       * and its corresponding pins.
       *
       * @param tft_cs uint8_t: Chip select
       * @param tft_dc uint8_t: Data/Command select
       * @param tft_rst uint8_t: Reset pin
       * @param tft_bsy uint8_t: Busy pin
       */
      TextUILcdGxEPD2( uint8_t tft_cs, uint8_t tft_dc, uint8_t tft_rst, uint8_t tft_bsy);

      void displayOn( boolean b) {}

      void commit() final;

      void clear() final;
      void clearEOL() final;

      bool colorSupport() final;

      void setBg( uint8_t r, uint8_t g, uint8_t b) final;
      void setFg( uint8_t r, uint8_t g, uint8_t b) final;

      void normalColors() final;
      void selectedColors() final;
      void editColors() final;

      bool inverseSupport() final;
      void setInverse( bool inv) final;

      /* FONT_SMALL .. FONT_LARGE */
      void setFontSize( FontSize_t sz) final;

      uint8_t getRows() final;
      uint8_t getColumns() final;

      /* row and column in characters */
      void setCursor( uint8_t r, uint8_t c) final;
      void setRow( uint8_t r) final;
      void setColumn( uint8_t c) final;

      void printChar( char ch) final;
};

#endif
