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

#include "TextUILcdGxEPD2.h"

#define LINEGAP (1*fontSz)

#define FONT_H (font_h+LINEGAP)
#define FONT_W (font_w+LINEGAP)

TextUILcdGxEPD2::TextUILcdGxEPD2( uint8_t tft_cs, uint8_t tft_dc, uint8_t tft_rst, uint8_t tft_bsy)
{
    display = new GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)>( GxEPD2_DRIVER_CLASS( tft_cs, tft_dc, tft_rst, tft_bsy));
    initDisplay();
}

void TextUILcdGxEPD2::initDisplay()
{
    //display->init(115200, true, 2, false);
    // Serial diag disabled
    display->init(0, true, 2, false);

    display->setRotation( 3);
    display->setFont(&FreeMonoBold12pt7b);
    display->setTextColor(GxEPD_BLACK);
    display->setFullWindow();

    width = display->width();
    height = display->height();

    textX = textY = 0; // text cursor position
    updX = updY = updW = updH = 0;
    partialUpdates = 0;

    setBg( 255, 255, 255);
    setFg( 0, 0, 0);
    setFontSize( TEXTUI_FONT_SMALL);
    invers = false;

    clear();
    setCursor(0, 0);
}

void TextUILcdGxEPD2::commit()
{
    if( updH > 0 && updW > 0) {
        if( partialUpdates >= PARTIAL_UPDATE_LIMIT) {

            /** @todo Full update does not work yet. */

            // Serial.println("!");
            // display->display(false);
            display->displayWindow(updX, updY, updW, updH);
            display->powerOff();
            partialUpdates = 0;
        } else {
            // Serial.printf("! %u %u %u %u\n", updX, updY, updW, updH);
            display->displayWindow(updX, updY, updW, updH);
            display->powerOff();
            partialUpdates++;
        }
        updX = updY = updW = updH = 0;
    }
}

void TextUILcdGxEPD2::clear()
{
    // Serial.println("C");
    display->fillScreen( invers ? fgCol565 : bgCol565);
    display->display(false);

    updX = updY = updW = updH = 0;
    textX = textY = 0;
}

void TextUILcdGxEPD2::clearEOL()
{
    // Serial.printf("E %u %u %u %u\n", textX, textY, width - textX, FONT_H);
    display->fillRect( textX, textY, width - textX, FONT_H, invers ? fgCol565 : bgCol565);
    extend( textX, textY, width - textX, FONT_H);
    //commit();
}

bool TextUILcdGxEPD2::colorSupport() {

  return false;
}

void TextUILcdGxEPD2::setBg( uint8_t r, uint8_t g, uint8_t b)
{
    bgCol565 = rgbToCol565( r, g ,b);
}

void TextUILcdGxEPD2::setFg( uint8_t r, uint8_t g, uint8_t b)
{
    fgCol565 = rgbToCol565( r, g ,b);
}

void TextUILcdGxEPD2::normalColors() {

    setBg(255,255,255);
    setFg(0,0,0);
}

void TextUILcdGxEPD2::selectedColors() {

    setBg(0,0,0);
    setFg(255,255,255);
}

void TextUILcdGxEPD2::editColors() {

    setBg(0,0,0);
    setFg(255,255,255);
}

bool TextUILcdGxEPD2::inverseSupport() {

    return true;
}

void TextUILcdGxEPD2::setInverse( bool inv) {

    invers = inv;
}

void TextUILcdGxEPD2::setFontSize( FontSize_t sz)
{
    int16_t x1 = 0;
    int16_t y1 = 0;
    uint16_t tmp_w;
    uint16_t tmp_h;

    if( sz == TEXTUI_FONT_SMALL) {
        fontSz = 1;
    } else if( sz == TEXTUI_FONT_MEDIUM) {
        fontSz = 2;
    } else {
        fontSz = 3;
    }

    display->setTextSize( fontSz);
    display->getTextBounds("W", 0, 0, &x1, &y1, &font_w, &tmp_h);
    display->getTextBounds("Wg", 0, 0, &x1, &y1, &tmp_w, &font_h);
    subscript_h = font_h - tmp_h;
}

uint8_t TextUILcdGxEPD2::getRows() {

    return height / FONT_H;
}

uint8_t TextUILcdGxEPD2::getColumns() {

    return width / FONT_W;
}

void TextUILcdGxEPD2::setCursor(  uint8_t r, uint8_t c)
{
    textX = c * FONT_W;
    textY = r * FONT_H;

    if( textX > width) { textX = width; }
    if( textY > height) { textY = height; }

    display->setCursor( textX, textY);
}

void TextUILcdGxEPD2::setRow( uint8_t r)
{
    textY = r * FONT_H;
    if( textY > height) { textY = height; }
    display->setCursor( textX, textY);
}

void TextUILcdGxEPD2::setColumn( uint8_t c)
{
    textX = c * FONT_W;
    if( textX > width) { textX = width; }
    display->setCursor( textX, textY);
}

void TextUILcdGxEPD2::printChar( char ch)
{
    // Serial.printf("P %c %u %u %u %u\n", ch, textX, textY, FONT_W, FONT_H);

    extend( textX, textY, FONT_W, FONT_H);

    display->fillRect( textX, textY, FONT_W, FONT_H, invers ? fgCol565 : bgCol565);
    display->drawChar( textX, textY +FONT_H -LINEGAP -subscript_h, ch,
                       invers ? bgCol565 : fgCol565,
                       invers ? fgCol565 : bgCol565,
                       fontSz);
    textX += FONT_W;
}

/* private */

pixel TextUILcdGxEPD2::rgbToCol565( uint8_t r, uint8_t g, uint8_t b)
{
    pixel col565 = ((r >> 3) << 11)
                | ((g >> 2) << 5)
                | (b >> 3);

    return col565;
}

void TextUILcdGxEPD2::extend(unsigned int x,unsigned int y,unsigned int w,unsigned int h)
{
    unsigned int updX2;
    unsigned int updY2;

    if( updW == 0 || updH == 0) {
        updX = x;
        updY = y;
        updW = w;
        updH = h;
    } else {
        updX2 = updX + updW;
        updY2 = updY + updH;
        if( x < updX) updX = x;
        if( y < updY) updY = y;
        if( x + w > updX2) updX2 = x + w;
        if( y + h > updY2) updY2 = y + h;
        updW = updX2 - updX;
        updH = updY2 - updY;
    }
    /** @todo clip to screen size */
}
