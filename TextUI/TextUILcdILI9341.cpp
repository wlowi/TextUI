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

#include "TextUILcdILI9341.h"

#include <SPI.h>

#define FONT_H (font_h)
#define FONT_W (font_w)

TextUILcdILI9341::TextUILcdILI9341( uint8_t tft_cs, uint8_t tft_dc, uint8_t tft_rst)
{
    tft = new Adafruit_ILI9341(tft_cs, tft_dc, tft_rst);
    initTFT();
}

void TextUILcdILI9341::initTFT()
{
    tft->begin();

    tft->setRotation( 3);
    
    width = tft->width();
    height = tft->height();

    textX = 0; // text cursor position
    textY = 0;

    setFg( 255, 255, 255);
    setBg( 0, 0, 0);
    setFontSize( TEXTUI_FONT_MEDIUM);
    invers = false;
    
    clear();
    setCursor(0, 0);
}

void TextUILcdILI9341::clear()
{
    tft->fillScreen( invers ? fgCol565 : bgCol565);
    textX = textY = 0;
}

void TextUILcdILI9341::clearEOL()
{
    tft->fillRect( textX, textY, width - textX, 8*fontSz, invers ? fgCol565 : bgCol565);
}

bool TextUILcdILI9341::colorSupport() {

  return true;
}

void TextUILcdILI9341::setBg( uint8_t r, uint8_t g, uint8_t b)
{
    bgCol565 = rgbToCol565( r, g ,b);
}

void TextUILcdILI9341::setFg( uint8_t r, uint8_t g, uint8_t b)
{
    fgCol565 = rgbToCol565( r, g ,b);
}

void TextUILcdILI9341::normalColors() {

    setBg(0,0,0);
    setFg(255,255,255);
}

void TextUILcdILI9341::selectedColors() {

    setBg(255,255,0);
    setFg(0,0,0);
}

void TextUILcdILI9341::editColors() {

    setBg(255,255,255);
    setFg(0,0,0);
}

void TextUILcdILI9341::setInvert( bool inv) {

    invers = inv;
}

void TextUILcdILI9341::setFontSize( FontSize_t sz)
{
    int16_t x1 = 0;
    int16_t y1 = 0;

    if( sz == TEXTUI_FONT_SMALL) {
        fontSz = 1;
    } else if( sz == TEXTUI_FONT_MEDIUM) {
        fontSz = 2;
    } else {
        fontSz = 3;
    }

    tft->setTextSize( fontSz);
    tft->getTextBounds("W", 0, 0, &x1, &y1, &font_w, &font_h);
}

uint8_t TextUILcdILI9341::getRows() {

    return height / FONT_H;
}

uint8_t TextUILcdILI9341::getColumns() {

    return width / FONT_W;
}

void TextUILcdILI9341::setCursor(  uint8_t r, uint8_t c)
{
    textX = c * FONT_W;
    textY = r * FONT_H;

    if( textX > width) { textX = width; }
    if( textY > height) { textY = height; }

    tft->setCursor( textX, textY);
}

void TextUILcdILI9341::setRow( uint8_t r)
{
    textY = r * FONT_H;
    if( textY > height) { textY = height; }
    tft->setCursor( textX, textY);
}

void TextUILcdILI9341::setColumn( uint8_t c)
{
    textX = c * FONT_W;
    if( textX > width) { textX = width; }
    tft->setCursor( textX, textY);
}

void TextUILcdILI9341::printChar( char ch)
{
    tft->drawChar( textX, textY, ch, invers ? bgCol565 : fgCol565, invers ? fgCol565 : bgCol565, fontSz);
    textX += FONT_W;
}

/* private */

pixel TextUILcdILI9341::rgbToCol565( uint8_t r, uint8_t g, uint8_t b)
{
    pixel col565 = ((r >> 3) << 11)
                | ((g >> 2) << 5)
                | (b >> 3);

    return col565;  
}
