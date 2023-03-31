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

#include "EmuTextUILcdST7735.h"

#define XY_TO_I( x, y) (x + y * width)

#define FONT_H (8 * fontSz)
#define FONT_W (6 * fontSz)

/* 128 pixel, Base font height 8
 *   8 * 16 = 128
 *  16 *  8 = 128
 *  32 *  4 = 128
 */
static const unsigned int lines[] =  { 0, 16, 8, 4 };

/* 160 pixel, Base font width 6
 *   6 * 26 = 156
 *  12 * 13 = 156
 *  24 *  6 = 144
 */
static const unsigned int columns[] = { 0, 26, 13, 6 };

/* standard ascii 5x7 font */
static const uint8_t  font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,   
    0x3E, 0x5B, 0x4F, 0x5B, 0x3E, 	
    0x3E, 0x6B, 0x4F, 0x6B, 0x3E, 	
    0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 
    0x18, 0x3C, 0x7E, 0x3C, 0x18, 
    0x1C, 0x57, 0x7D, 0x57, 0x1C, 
    0x1C, 0x5E, 0x7F, 0x5E, 0x1C, 
    0x00, 0x18, 0x3C, 0x18, 0x00, 
    0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 
    0x00, 0x18, 0x24, 0x18, 0x00, 
    0xFF, 0xE7, 0xDB, 0xE7, 0xFF, 
    0x30, 0x48, 0x3A, 0x06, 0x0E, 
    0x26, 0x29, 0x79, 0x29, 0x26, 
    0x40, 0x7F, 0x05, 0x05, 0x07, 
    0x40, 0x7F, 0x05, 0x25, 0x3F, 
    0x5A, 0x3C, 0xE7, 0x3C, 0x5A, 
    0x7F, 0x3E, 0x1C, 0x1C, 0x08, 
    0x08, 0x1C, 0x1C, 0x3E, 0x7F, 
    0x14, 0x22, 0x7F, 0x22, 0x14, 
    0x5F, 0x5F, 0x00, 0x5F, 0x5F, 
    0x06, 0x09, 0x7F, 0x01, 0x7F, 
    0x00, 0x66, 0x89, 0x95, 0x6A, 
    0x60, 0x60, 0x60, 0x60, 0x60, 
    0x94, 0xA2, 0xFF, 0xA2, 0x94, 
    0x08, 0x04, 0x7E, 0x04, 0x08, 
    0x10, 0x20, 0x7E, 0x20, 0x10, 
    0x08, 0x08, 0x2A, 0x1C, 0x08, 
    0x08, 0x1C, 0x2A, 0x08, 0x08, 
    0x1E, 0x10, 0x10, 0x10, 0x10, 
    0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 
    0x30, 0x38, 0x3E, 0x38, 0x30, 
    0x06, 0x0E, 0x3E, 0x0E, 0x06, 
    0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x5F, 0x00, 0x00, 
    0x00, 0x07, 0x00, 0x07, 0x00, 
    0x14, 0x7F, 0x14, 0x7F, 0x14, 
    0x24, 0x2A, 0x7F, 0x2A, 0x12, 
    0x23, 0x13, 0x08, 0x64, 0x62, 
    0x36, 0x49, 0x56, 0x20, 0x50, 
    0x00, 0x08, 0x07, 0x03, 0x00, 
    0x00, 0x1C, 0x22, 0x41, 0x00, 
    0x00, 0x41, 0x22, 0x1C, 0x00, 
    0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 
    0x08, 0x08, 0x3E, 0x08, 0x08, 
    0x00, 0x80, 0x70, 0x30, 0x00, 
    0x08, 0x08, 0x08, 0x08, 0x08, 
    0x00, 0x00, 0x60, 0x60, 0x00, 
    0x20, 0x10, 0x08, 0x04, 0x02, 
    0x3E, 0x51, 0x49, 0x45, 0x3E, 
    0x00, 0x42, 0x7F, 0x40, 0x00, 
    0x72, 0x49, 0x49, 0x49, 0x46, 
    0x21, 0x41, 0x49, 0x4D, 0x33, 
    0x18, 0x14, 0x12, 0x7F, 0x10, 
    0x27, 0x45, 0x45, 0x45, 0x39, 
    0x3C, 0x4A, 0x49, 0x49, 0x31, 
    0x41, 0x21, 0x11, 0x09, 0x07, 
    0x36, 0x49, 0x49, 0x49, 0x36, 
    0x46, 0x49, 0x49, 0x29, 0x1E, 
    0x00, 0x00, 0x14, 0x00, 0x00, 
    0x00, 0x40, 0x34, 0x00, 0x00, 
    0x00, 0x08, 0x14, 0x22, 0x41, 
    0x14, 0x14, 0x14, 0x14, 0x14, 
    0x00, 0x41, 0x22, 0x14, 0x08, 
    0x02, 0x01, 0x59, 0x09, 0x06, 
    0x3E, 0x41, 0x5D, 0x59, 0x4E, 
    0x7C, 0x12, 0x11, 0x12, 0x7C, 
    0x7F, 0x49, 0x49, 0x49, 0x36, 
    0x3E, 0x41, 0x41, 0x41, 0x22, 
    0x7F, 0x41, 0x41, 0x41, 0x3E, 
    0x7F, 0x49, 0x49, 0x49, 0x41, 
    0x7F, 0x09, 0x09, 0x09, 0x01, 
    0x3E, 0x41, 0x41, 0x51, 0x73, 
    0x7F, 0x08, 0x08, 0x08, 0x7F, 
    0x00, 0x41, 0x7F, 0x41, 0x00, 
    0x20, 0x40, 0x41, 0x3F, 0x01, 
    0x7F, 0x08, 0x14, 0x22, 0x41, 
    0x7F, 0x40, 0x40, 0x40, 0x40, 
    0x7F, 0x02, 0x1C, 0x02, 0x7F, 
    0x7F, 0x04, 0x08, 0x10, 0x7F, 
    0x3E, 0x41, 0x41, 0x41, 0x3E, 
    0x7F, 0x09, 0x09, 0x09, 0x06, 
    0x3E, 0x41, 0x51, 0x21, 0x5E, 
    0x7F, 0x09, 0x19, 0x29, 0x46, 
    0x26, 0x49, 0x49, 0x49, 0x32, 
    0x03, 0x01, 0x7F, 0x01, 0x03, 
    0x3F, 0x40, 0x40, 0x40, 0x3F, 
    0x1F, 0x20, 0x40, 0x20, 0x1F, 
    0x3F, 0x40, 0x38, 0x40, 0x3F, 
    0x63, 0x14, 0x08, 0x14, 0x63, 
    0x03, 0x04, 0x78, 0x04, 0x03, 
    0x61, 0x59, 0x49, 0x4D, 0x43, 
    0x00, 0x7F, 0x41, 0x41, 0x41, 
    0x02, 0x04, 0x08, 0x10, 0x20, 
    0x00, 0x41, 0x41, 0x41, 0x7F, 
    0x04, 0x02, 0x01, 0x02, 0x04, 
    0x40, 0x40, 0x40, 0x40, 0x40, 
    0x00, 0x03, 0x07, 0x08, 0x00, 
    0x20, 0x54, 0x54, 0x78, 0x40, 
    0x7F, 0x28, 0x44, 0x44, 0x38, 
    0x38, 0x44, 0x44, 0x44, 0x28, 
    0x38, 0x44, 0x44, 0x28, 0x7F, 
    0x38, 0x54, 0x54, 0x54, 0x18, 
    0x00, 0x08, 0x7E, 0x09, 0x02, 
    0x18, 0xA4, 0xA4, 0x9C, 0x78, 
    0x7F, 0x08, 0x04, 0x04, 0x78, 
    0x00, 0x44, 0x7D, 0x40, 0x00, 
    0x20, 0x40, 0x40, 0x3D, 0x00, 
    0x7F, 0x10, 0x28, 0x44, 0x00, 
    0x00, 0x41, 0x7F, 0x40, 0x00, 
    0x7C, 0x04, 0x78, 0x04, 0x78, 
    0x7C, 0x08, 0x04, 0x04, 0x78, 
    0x38, 0x44, 0x44, 0x44, 0x38, 
    0xFC, 0x18, 0x24, 0x24, 0x18, 
    0x18, 0x24, 0x24, 0x18, 0xFC, 
    0x7C, 0x08, 0x04, 0x04, 0x08, 
    0x48, 0x54, 0x54, 0x54, 0x24, 
    0x04, 0x04, 0x3F, 0x44, 0x24, 
    0x3C, 0x40, 0x40, 0x20, 0x7C, 
    0x1C, 0x20, 0x40, 0x20, 0x1C, 
    0x3C, 0x40, 0x30, 0x40, 0x3C, 
    0x44, 0x28, 0x10, 0x28, 0x44, 
    0x4C, 0x90, 0x90, 0x90, 0x7C, 
    0x44, 0x64, 0x54, 0x4C, 0x44, 
    0x00, 0x08, 0x36, 0x41, 0x00, 
    0x00, 0x00, 0x77, 0x00, 0x00, 
    0x00, 0x41, 0x36, 0x08, 0x00, 
    0x02, 0x01, 0x02, 0x04, 0x02, 
    0x3C, 0x26, 0x23, 0x26, 0x3C, 
    0x1E, 0xA1, 0xA1, 0x61, 0x12, 
    0x3A, 0x40, 0x40, 0x20, 0x7A, 
    0x38, 0x54, 0x54, 0x55, 0x59, 
    0x21, 0x55, 0x55, 0x79, 0x41, 
    0x21, 0x54, 0x54, 0x78, 0x41, 
    0x21, 0x55, 0x54, 0x78, 0x40, 
    0x20, 0x54, 0x55, 0x79, 0x40, 
    0x0C, 0x1E, 0x52, 0x72, 0x12, 
    0x39, 0x55, 0x55, 0x55, 0x59, 
    0x39, 0x54, 0x54, 0x54, 0x59, 
    0x39, 0x55, 0x54, 0x54, 0x58, 
    0x00, 0x00, 0x45, 0x7C, 0x41, 
    0x00, 0x02, 0x45, 0x7D, 0x42, 
    0x00, 0x01, 0x45, 0x7C, 0x40, 
    0xF0, 0x29, 0x24, 0x29, 0xF0, 
    0xF0, 0x28, 0x25, 0x28, 0xF0, 
    0x7C, 0x54, 0x55, 0x45, 0x00, 
    0x20, 0x54, 0x54, 0x7C, 0x54, 
    0x7C, 0x0A, 0x09, 0x7F, 0x49, 
    0x32, 0x49, 0x49, 0x49, 0x32, 
    0x32, 0x48, 0x48, 0x48, 0x32, 
    0x32, 0x4A, 0x48, 0x48, 0x30, 
    0x3A, 0x41, 0x41, 0x21, 0x7A, 
    0x3A, 0x42, 0x40, 0x20, 0x78, 
    0x00, 0x9D, 0xA0, 0xA0, 0x7D, 
    0x39, 0x44, 0x44, 0x44, 0x39, 
    0x3D, 0x40, 0x40, 0x40, 0x3D, 
    0x3C, 0x24, 0xFF, 0x24, 0x24, 
    0x48, 0x7E, 0x49, 0x43, 0x66, 
    0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 
    0xFF, 0x09, 0x29, 0xF6, 0x20, 
    0xC0, 0x88, 0x7E, 0x09, 0x03, 
    0x20, 0x54, 0x54, 0x79, 0x41, 
    0x00, 0x00, 0x44, 0x7D, 0x41, 
    0x30, 0x48, 0x48, 0x4A, 0x32, 
    0x38, 0x40, 0x40, 0x22, 0x7A, 
    0x00, 0x7A, 0x0A, 0x0A, 0x72, 
    0x7D, 0x0D, 0x19, 0x31, 0x7D, 
    0x26, 0x29, 0x29, 0x2F, 0x28, 
    0x26, 0x29, 0x29, 0x29, 0x26, 
    0x30, 0x48, 0x4D, 0x40, 0x20, 
    0x38, 0x08, 0x08, 0x08, 0x08, 
    0x08, 0x08, 0x08, 0x08, 0x38, 
    0x2F, 0x10, 0xC8, 0xAC, 0xBA, 
    0x2F, 0x10, 0x28, 0x34, 0xFA, 
    0x00, 0x00, 0x7B, 0x00, 0x00, 
    0x08, 0x14, 0x2A, 0x14, 0x22, 
    0x22, 0x14, 0x2A, 0x14, 0x08, 
    0xAA, 0x00, 0x55, 0x00, 0xAA, 
    0xAA, 0x55, 0xAA, 0x55, 0xAA, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 
    0x10, 0x10, 0x10, 0xFF, 0x00, 
    0x14, 0x14, 0x14, 0xFF, 0x00, 
    0x10, 0x10, 0xFF, 0x00, 0xFF, 
    0x10, 0x10, 0xF0, 0x10, 0xF0, 
    0x14, 0x14, 0x14, 0xFC, 0x00, 
    0x14, 0x14, 0xF7, 0x00, 0xFF, 
    0x00, 0x00, 0xFF, 0x00, 0xFF, 
    0x14, 0x14, 0xF4, 0x04, 0xFC, 
    0x14, 0x14, 0x17, 0x10, 0x1F, 
    0x10, 0x10, 0x1F, 0x10, 0x1F, 
    0x14, 0x14, 0x14, 0x1F, 0x00, 
    0x10, 0x10, 0x10, 0xF0, 0x00, 
    0x00, 0x00, 0x00, 0x1F, 0x10, 
    0x10, 0x10, 0x10, 0x1F, 0x10, 
    0x10, 0x10, 0x10, 0xF0, 0x10, 
    0x00, 0x00, 0x00, 0xFF, 0x10, 
    0x10, 0x10, 0x10, 0x10, 0x10, 
    0x10, 0x10, 0x10, 0xFF, 0x10, 
    0x00, 0x00, 0x00, 0xFF, 0x14, 
    0x00, 0x00, 0xFF, 0x00, 0xFF, 
    0x00, 0x00, 0x1F, 0x10, 0x17, 
    0x00, 0x00, 0xFC, 0x04, 0xF4, 
    0x14, 0x14, 0x17, 0x10, 0x17, 
    0x14, 0x14, 0xF4, 0x04, 0xF4, 
    0x00, 0x00, 0xFF, 0x00, 0xF7, 
    0x14, 0x14, 0x14, 0x14, 0x14, 
    0x14, 0x14, 0xF7, 0x00, 0xF7, 
    0x14, 0x14, 0x14, 0x17, 0x14, 
    0x10, 0x10, 0x1F, 0x10, 0x1F, 
    0x14, 0x14, 0x14, 0xF4, 0x14, 
    0x10, 0x10, 0xF0, 0x10, 0xF0, 
    0x00, 0x00, 0x1F, 0x10, 0x1F, 
    0x00, 0x00, 0x00, 0x1F, 0x14, 
    0x00, 0x00, 0x00, 0xFC, 0x14, 
    0x00, 0x00, 0xF0, 0x10, 0xF0, 
    0x10, 0x10, 0xFF, 0x10, 0xFF, 
    0x14, 0x14, 0x14, 0xFF, 0x14, 
    0x10, 0x10, 0x10, 0x1F, 0x00, 
    0x00, 0x00, 0x00, 0xF0, 0x10, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
    0x38, 0x44, 0x44, 0x38, 0x44, 
    0x7C, 0x2A, 0x2A, 0x3E, 0x14, 
    0x7E, 0x02, 0x02, 0x06, 0x06, 
    0x02, 0x7E, 0x02, 0x7E, 0x02, 
    0x63, 0x55, 0x49, 0x41, 0x63, 
    0x38, 0x44, 0x44, 0x3C, 0x04, 
    0x40, 0x7E, 0x20, 0x1E, 0x20, 
    0x06, 0x02, 0x7E, 0x02, 0x02, 
    0x99, 0xA5, 0xE7, 0xA5, 0x99, 
    0x1C, 0x2A, 0x49, 0x2A, 0x1C, 
    0x4C, 0x72, 0x01, 0x72, 0x4C, 
    0x30, 0x4A, 0x4D, 0x4D, 0x30, 
    0x30, 0x48, 0x78, 0x48, 0x30, 
    0xBC, 0x62, 0x5A, 0x46, 0x3D, 
    0x3E, 0x49, 0x49, 0x49, 0x00, 
    0x7E, 0x01, 0x01, 0x01, 0x7E, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
    0x44, 0x44, 0x5F, 0x44, 0x44, 
    0x40, 0x51, 0x4A, 0x44, 0x40, 
    0x40, 0x44, 0x4A, 0x51, 0x40, 
    0x00, 0x00, 0xFF, 0x01, 0x03, 
    0xE0, 0x80, 0xFF, 0x00, 0x00, 
    0x08, 0x08, 0x6B, 0x6B, 0x08,
    0x36, 0x12, 0x36, 0x24, 0x36, 
    0x06, 0x0F, 0x09, 0x0F, 0x06, 
    0x00, 0x00, 0x18, 0x18, 0x00, 
    0x00, 0x00, 0x10, 0x10, 0x00, 
    0x30, 0x40, 0xFF, 0x01, 0x01, 
    0x00, 0x1F, 0x01, 0x01, 0x1E, 
    0x00, 0x19, 0x1D, 0x17, 0x12, 
    0x00, 0x3C, 0x3C, 0x3C, 0x3C, 
    0x00, 0x00, 0x00, 0x00, 0x00, 
};

EmuTextUILcdST7735::EmuTextUILcdST7735( wxWindow *parent, wxWindowID id)
    : TextUILcd(), wxPanel( parent, id, wxDefaultPosition, wxSize(160*2,128*2))
{
    this->width = 160;
    this->height = 128;
    this->pixSz = 2;
    textX = 0; // text cursor position
    textY = 0;
    buffer = new pixel[width*height];

    Connect(wxEVT_PAINT, wxPaintEventHandler(EmuTextUILcdST7735::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(EmuTextUILcdST7735::OnSize)); 

    normalColors();
    setFontSize( TEXTUI_FONT_SMALL);
    clear();
}

void EmuTextUILcdST7735::OnSize( wxSizeEvent& event)
{
    Refresh();
}

void EmuTextUILcdST7735::OnPaint( wxPaintEvent& event)
{
    unsigned int x;
    unsigned int y;
    pixel col565 = buffer[0];

    wxPaintDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    wxColor col = col565ToCol( col565);
    dc.SetPen(wxPen(col));
    dc.SetBrush(wxBrush(col));

    for( x=0; x<width; x++) {
        for( y=0; y<height; y++) {

            if( col565 != buffer[ XY_TO_I(x,y) ]) {
                col565 = buffer[ XY_TO_I(x,y) ];
                col = col565ToCol( col565);
                dc.SetPen(wxPen(col));
                dc.SetBrush(wxBrush(col));
            }

            setPixelOnDC( dc, x, y);
        }
    }
}

void EmuTextUILcdST7735::clear()
{
    unsigned int x;
    unsigned int y;

    for( x=0; x<width; x++) {
        for( y=0; y<height; y++) {
            buffer[ XY_TO_I(x,y) ] = bgCol565;
        }
    }

    textX = textY = 0;
    
    Refresh();
}

void EmuTextUILcdST7735::clearEOL()
{
    unsigned int x;
    unsigned int y;

    for( x=textX; x<width; x++) {
        for( y=textY; y<textY + (8*fontSz); y++) {
            buffer[ XY_TO_I(x,y) ] = bgCol565;
        }
    }
    
    Refresh();
}

bool EmuTextUILcdST7735::colorSupport() {

    return true;
}

void EmuTextUILcdST7735::setBg( unsigned char r, unsigned char g, unsigned char b)
{
    bgCol565 = rgbToCol565( r, g ,b);
    bgCol = wxColor( r, g, b);
}

void EmuTextUILcdST7735::setFg( unsigned char r, unsigned char g, unsigned char b)
{
    fgCol565 = rgbToCol565( r, g ,b);
    fgCol = wxColor( r, g, b);
}

void EmuTextUILcdST7735::normalColors() {

    setBg(0,0,0);
    setFg(255,255,255);
}

void EmuTextUILcdST7735::selectedColors() {

    setBg(255,255,0);
    setFg(0,0,0);
}

void EmuTextUILcdST7735::setInvert( bool inv) {

    if( inv) {
        selectedColors();
    } else {
        normalColors();
    }

}

void EmuTextUILcdST7735::setFontSize( uint8_t sz)
{
    if( sz == TEXTUI_FONT_SMALL) {
        fontSz = 1;
    } else if( sz == TEXTUI_FONT_MEDIUM) {
        fontSz = 2;    
    } else {
        fontSz = 3;
    }
}

uint16_t EmuTextUILcdST7735::getRows() {

    return lines[fontSz];
}

uint16_t EmuTextUILcdST7735::getColumns() {

    return columns[fontSz];
}

void EmuTextUILcdST7735::setCursor( uint8_t r, uint8_t c)
{
    textX = c * FONT_W;
    textY = r * FONT_H;

    if( textX > width) { textX = width; }
    if( textY > height) { textY = height; }
}

void EmuTextUILcdST7735::setRow( uint8_t r)
{
    textY = r * FONT_H;
    if( textY > height) { textY = height; }
}

void EmuTextUILcdST7735::setColumn( uint8_t c)
{
    textX = c * FONT_W;
    if( textX > width) { textX = width; }
}

void EmuTextUILcdST7735::printChar( const char ch)
{
    wxClientDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    printChar( dc, ch);
}

/*
void EmuTextUILcdST7735::setPixel( unsigned int x, unsigned int y)
{
    if( x >= width || y >= height) {
        return;
    }

    wxClientDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    dc.SetPen(wxPen(fgCol));
    dc.SetBrush(wxBrush(fgCol));

    buffer[ XY_TO_I(x,y) ] = fgCol565;
    setPixelOnDC( dc, x, y);    
}

void EmuTextUILcdST7735::setPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
    if( x >= width || y >= height) {
        return;
    }

    wxClientDC dc(this);
    dc.SetLogicalScale( pixSz, pixSz);

    pixel col565 = rgbToCol565( r, g, b);
    wxColor col = wxColor( r, g, b);

    dc.SetPen(wxPen(col));
    dc.SetBrush(wxBrush(col));

    buffer[ XY_TO_I(x,y) ] = col565;
    setPixelOnDC( dc, x, y);
}

*/

/* private */

void EmuTextUILcdST7735::setPixelOnDC( wxDC &dc, unsigned int x, unsigned int y)
{
    dc.DrawRectangle( x, y, 1, 1);
}

pixel EmuTextUILcdST7735::colToCol565( const wxColor &col)
{
    pixel col565 = ((col.Red() >> 3) << 11)
                 | ((col.Green() >> 2) << 5)
                 | (col.Blue() >> 3);

    return col565;
}

pixel EmuTextUILcdST7735::rgbToCol565( unsigned char r, unsigned char g, unsigned char b)
{
  pixel col565 = ((r >> 3) << 11)
               | ((g >> 2) << 5)
               | (b >> 3);

  return col565;  
}

wxColor EmuTextUILcdST7735::col565ToCol( pixel col565)
{
    unsigned char r = ((col565 >> 11) & 0x1f) << 3;
    unsigned char g = ((col565 >> 5) & 0x3f) << 2;
    unsigned char b = (col565 & 0x1f) << 3;

    return wxColor( r, g, b);
}

void EmuTextUILcdST7735::printChar( wxDC &dc, char ch)
{
    unsigned int x;
    unsigned int sz;

    for( x = 0; x < 5; x++) {
        for( sz = 0; sz < fontSz; sz++) {
            if( textX >= width) { break; }
            charLine( dc, font5x7[ 5 * ((unsigned char)ch) + x ]);
        }
    }

    for( sz = 0; sz < fontSz; sz++) {
        charLine( dc, 0); // Gap to next character
    }
}

void EmuTextUILcdST7735::charLine( wxDC &dc, uint8_t l)
 {
    unsigned int y;
    unsigned int sz;

    for( y = 0; y < 8; y++)	{
        if( l & 1) {
            dc.SetPen(wxPen(fgCol));
            dc.SetBrush(wxBrush(fgCol));
        } else {
            dc.SetPen(wxPen(bgCol));
            dc.SetBrush(wxBrush(bgCol));
        }

        for( sz = 0; sz < fontSz; sz++) {
            unsigned int ty = textY + sz + fontSz * y;

            if( ty >= height) { break; }

            if( l & 1) {
                buffer[ XY_TO_I(textX,ty) ] = fgCol565;
            } else {
                buffer[ XY_TO_I(textX,ty) ] = bgCol565;
            }

            setPixelOnDC( dc, textX, ty);
        }
        l = l >> 1;
    }

    textX++;
}



