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

#include "Arduino.h"

#include "EmuTextUILcdSSD1306.h"
#include "EmuTextUISimpleKbd.h"
#include "EEPROM.h"

#include "time.h"

EEPROMClass EEPROM(4096);

EmuTextUILcdSSD1306 *emuLcd;
EmuTextUISimpleKbd *emuSimpleKbd;

extern void setup( void);
extern void loop( void);

unsigned long millis() {

    return (long)clock() * 1000 / CLOCKS_PER_SEC;
}

class RunTest : public wxApp
{
    public:
        virtual bool OnInit();
};
 
class MyFrame : public wxFrame
{
    public:
        MyFrame();
 
    private:
        void OnClose(wxCloseEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void handleIdle(wxIdleEvent& event);
};

enum
{
    ID_Hello = 1
};
 
wxIMPLEMENT_APP(RunTest);
 
bool RunTest::OnInit()
{
    EEPROM.loadFromFile();

    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "RunTest")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );
 
    CreateStatusBar();
    SetStatusText("RunTest");
 
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    hbox->AddSpacer(10);
    emuLcd =  new EmuTextUILcdSSD1306( panel, wxID_ANY);
    hbox->Add( emuLcd);
    hbox->AddSpacer(10);
    emuSimpleKbd = new EmuTextUISimpleKbd( panel);
    hbox->Add( emuSimpleKbd);
    hbox->AddSpacer(10);

    panel->SetSizer(hbox);

    hbox->Fit( this);

    Bind(wxEVT_IDLE, &MyFrame::handleIdle, this);
}
 
void MyFrame::OnClose(wxCloseEvent& event)
{
    EEPROM.saveToFile();
    Destroy();
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close();
}
 
void MyFrame::OnAbout(wxCommandEvent& event)
{

}

void MyFrame::handleIdle(wxIdleEvent& event) 
{
    static bool setup_done = false;

    event.RequestMore();

    if( setup_done) {
        loop();
    } else {
        setup();
        setup_done = true;
    }
}
