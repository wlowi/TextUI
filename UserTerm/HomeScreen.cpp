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

#include "HomeScreen.h"

const char* KEYS[] = {
  "none",
  "up",
  "down",
  "enter",
  "clear"
};

HomeScreen::HomeScreen() {
  
  key = KEYS[0];
  count = 0;
}

void HomeScreen::activate( TextUI *ui) {

  TextUILcd *lcd = ui->getDisplay();
  rows = lcd->getRows();
  cols = lcd->getColumns();
}

void HomeScreen::handleEvent(TextUI *ui, Event *e) {

  if( e->getType() == EVENT_TYPE_KEY) {

    switch( e->getKey()) {
      case KEY_UP:
        key = KEYS[1];
        break;
      case KEY_DOWN:
        key = KEYS[2];
        break;
      case KEY_ENTER:
        key = KEYS[3];
        break;
      case KEY_CLEAR:
        key = KEYS[4];
        break;
    }

    count = e->getCount();
    
    e->markProcessed();

    refresh = true;
  }
}

bool HomeScreen::needsRefresh() {

  return refresh;
}

void HomeScreen::endRefresh() {

  refresh = false;
}

uint8_t HomeScreen::getRowCount() {
  return 3;
}

const char *HomeScreen::getRowName( uint8_t row) {
  return "";
}

uint8_t HomeScreen::getColCount( uint8_t row) {
  return 2;
}

void HomeScreen::getValue(uint8_t row, uint8_t col, Cell *cell) {

  if( col == 0) {
    if( row == 0) {
      cell->setLabel( 0, key, 5);
    } else if( row == 1) {
      cell->setLabel( 0, "rows", 5);
    } else if( row == 2) {
      cell->setLabel( 0, "cols", 5);
    }
  } else if( col == 1) {
    if( row == 0) {
      cell->setInt8( 6, count, 4, 0, 255);
    } else if( row == 1) {
      cell->setInt16( 6, rows, 4, 0, 255);
    } else if( row == 2) {
      cell->setInt16( 6, cols, 4, 0, 255);
    }
  }
}
