
#ifndef _EmuTextUILcdSSD1306_h_
#define _EmuTextUILcdSSD1306_h_

#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "TextUI.h"

typedef uint16_t pixel;

class EmuTextUILcdSSD1306 : public TextUILcd, public wxPanel
{
    private:
        wxWindow *parent;
        unsigned int width;
        unsigned int height;
        unsigned int pixSz;
        pixel *buffer;
        pixel fgCol565;
        pixel bgCol565;
		wxColor fgCol;
		wxColor bgCol;
        unsigned int fontSz; // 1 - 3
        unsigned int textX;
        unsigned int textY;

        void setPixelOnDC( wxDC &dc, unsigned int x, unsigned int y);
        pixel colToCol565( const wxColor &col);
        pixel rgbToCol565( unsigned char r, unsigned char g, unsigned char b);
        wxColor col565ToCol( pixel col565);
        void printChar( wxDC &dc, char ch);
		void charLine( wxDC &dc, char l);

    public:
        EmuTextUILcdSSD1306( wxWindow *parent, wxWindowID id);

        void OnSize( wxSizeEvent& event);
        void OnPaint( wxPaintEvent& event);

        bool colorSupport();

        /* Clear screen and move cursor to top-left position */
        void clear();
        /* Clear to end of line but do not move the cursor */
        void clearEOL();

        void setBg( unsigned char r, unsigned char g, unsigned char b);
        void setFg( unsigned char r, unsigned char g, unsigned char b);

        void normalColors();
        void selectedColors();

        void setInvert( bool inv);

        void setFontSize( uint8_t sz);
    
        uint16_t getRows();
        uint16_t getColumns();
    
        void setCursor( uint8_t r, uint8_t c);
        void setRow( uint8_t r);
        void setColumn( uint8_t c);

        void printChar( char ch);
};

#endif
