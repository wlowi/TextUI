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
#include "TextUI.h"
#include "Config.h"

#ifdef ARDUINO

  #include "TextUILcdSSD1306.h"
  #include "TextUISimpleKbd.h"
  
#else

  #if LCD_TYPE == SSD1306
    #include "EmuTextUILcdSSD1306.h"
    extern EmuTextUILcdSSD1306 *emuLcd;
  #elif LCD_TYPE == ST7735
    #include "EmuTextUILcdST7735.h"
    extern EmuTextUILcdST7735 *emuLcd;  
  #endif

  #include "EmuTextUISimpleKbd.h"
  extern EmuTextUISimpleKbd *emuSimpleKbd;

#endif

#include "screen/HomeScreen.h"
#include "screen/DataScreen.h"
#include "screen/ChannelScreen.h"
#include "screen/ConfigScreen.h"
 
TextUI textUI;

DataScreen dataScreen;
ChannelScreen channelScreen;
ConfigScreen configScreen;

const uint8_t BUTTON_COUNT = 3;
uint8_t buttons[BUTTON_COUNT] = { 14, 15, 16 };
uint8_t keys[BUTTON_COUNT] = { KEY_UP, KEY_ENTER, KEY_DOWN };

TextUILcd *lcd;

uint8_t testNo = 0;

void processKey( Event *e);
void processPPM();
void updateUI();

/*
void testPrintInt();
void testPrintIntNeg();
void testPrintUInt();
void testPrintLong();
void testPrintLongNeg();
void testPrintULong();
void testPrintStr();
void testPrintFixFloat();
*/

void setup() {

#ifdef ARDUINO
  Serial.begin(19200);
#endif

#ifdef ARDUINO
  textUI.setDisplay( new TextUILcdSSD1306());
  textUI.setInput( new TextUISimpleKbd( BUTTON_COUNT, buttons, keys));
#else
  textUI.setDisplay( emuLcd);
  textUI.setInput( emuSimpleKbd);
#endif

  textUI.setTimer( 500);
  
  textUI.setHomeScreen( new HomeScreen());
  
  lcd = textUI.getDisplay();
}

void loop() {

  Event *e = textUI.getEvent();

  textUI.handle( e);
  
  switch( e->getType()) {

    case EVENT_TYPE_IRQ:
      processPPM();
      break;
      
    case EVENT_TYPE_TIMER:
      updateUI();
      break;
      
    case EVENT_TYPE_KEY:
      processKey( e);
      break;
  }
}

void processKey( Event *e) {

#ifdef ARDUINO
  Serial.print( F("ProcessKey "));
  Serial.print( e->getKey());
  Serial.print( F("\n"));
#else
  LOGV("ProcessKey k=%d c=%d\n", e->getKey(), e->getCount());
#endif

/*
  switch( testNo) {
  case 0: testPrintInt(); break;
  case 1: testPrintIntNeg(); break;
  case 2: testPrintUInt(); break;
  case 3: testPrintLong(); break;
  case 4: testPrintLongNeg(); break;
  case 5: testPrintULong(); break;
  case 6: testPrintStr(); break;
  case 7: testPrintFixFloat(); break;
  }

  testNo = (testNo +1 ) % 8;
*/
}

void processPPM() {

#ifdef ARDUINO
  Serial.print( F("ProcessPPM\n"));
#else
  LOG("ProcessPPM\n");
#endif

}

void updateUI() {

#ifdef ARDUINO
  Serial.print( F("Timer: updateUI\n"));
#else
  LOG("Timer: updateUI\n");
#endif

  dataScreen.update();
}

/*
void testPrintInt() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printInt( 12345); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printInt( 12345, 7); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printInt( 12345, 7, '#'); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printInt( 12345, 5); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printInt( 12345, 5, '#'); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printInt( 12345, 4); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printInt( 12345, 4, '#'); lcd->printChar('\'');
}

void testPrintIntNeg() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printInt( -12345); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printInt( -12345, 7); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printInt( -12345, 7, '#'); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printInt( -12345, 5); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printInt( -12345, 5, '#'); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printInt( -12345, 4); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printInt( -12345, 4, '#'); lcd->printChar('\'');
}

void testPrintUInt() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printUInt( 65535); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printUInt( 65535, 7); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printUInt( 65535, 7, '#'); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printUInt( 65535, 5); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printUInt( 65535, 5, '#'); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printUInt( 65535, 4); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printUInt( 65535, 4, '#'); lcd->printChar('\'');
}

void testPrintLong() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printLong( 1234567890L); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printLong( 1234567890L, 12); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printLong( 1234567890L, 12, '#'); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printLong( 1234567890L, 10); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printLong( 1234567890L, 10, '#'); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printLong( 1234567890L, 9); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printLong( 1234567890L, 9, '#'); lcd->printChar('\'');
}

void testPrintLongNeg() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printLong( -1234567890L); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printLong( -1234567890L, 12); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printLong( -1234567890L, 12, '#'); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printLong( -1234567890L, 10); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printLong( -1234567890L, 10, '#'); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printLong( -1234567890L, 9); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printLong( -1234567890L, 9, '#'); lcd->printChar('\'');
}

void testPrintULong() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printULong( 4294967295L); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printULong( 4294967295L, 12); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printULong( 4294967295L, 12, '#'); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printULong( 4294967295L, 10); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printULong( 4294967295L, 10, '#'); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printULong( 4294967295L, 9); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printULong( 4294967295L, 9, '#'); lcd->printChar('\'');
}

void testPrintStr() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printStr( "abcdefg"); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printStr( "abcdefg", 10); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printStr( "abcdefg", 10, 0); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printStr( "abcdefg", 10, 2); lcd->printChar('\'');
}

void testPrintFixFloat() {

  lcd->clear();
  lcd->setCursor(0,0); lcd->printFixFloat1( 0, 6); lcd->printChar('\'');
  lcd->setCursor(1,0); lcd->printFixFloat1( 10000, 6); lcd->printChar('\'');
  lcd->setCursor(2,0); lcd->printFixFloat1( -10000, 7); lcd->printChar('\'');
  lcd->setCursor(3,0); lcd->printFixFloat1( -10000, 6); lcd->printChar('\'');
  lcd->setCursor(4,0); lcd->printFixFloat2( 0, 6); lcd->printChar('\'');
  lcd->setCursor(5,0); lcd->printFixFloat2( 10000, 6); lcd->printChar('\'');
  lcd->setCursor(6,0); lcd->printFixFloat2( -10000, 7); lcd->printChar('\'');
  lcd->setCursor(7,0); lcd->printFixFloat2( -10000, 6); lcd->printChar('\'');
}
*/
