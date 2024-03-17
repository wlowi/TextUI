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

#include "TextUIStreamProxy.h"

TextUIStreamProxy::TextUIStreamProxy( Stream &s) : stream( s) {

}


/* TextUIInput */
bool TextUIStreamProxy::pending() {

}

void TextUIStreamProxy::setEvent(Event* e) {

}


/* TextUILcd */
void TextUIStreamProxy::clear() {

}

void TextUIStreamProxy::clearEOL() {

}

bool TextUIStreamProxy::colorSupport() {

}

void TextUIStreamProxy::setBg(uint8_t r, uint8_t g, uint8_t b) {

}

void TextUIStreamProxy::setFg(uint8_t r, uint8_t g, uint8_t b) {

}

void TextUIStreamProxy::normalColors() {

}

void TextUIStreamProxy::selectedColors() {

}

void TextUIStreamProxy::editColors() {

}

void TextUIStreamProxy::setInvert(bool inv) {

}

/* FONT_SMALL .. FONT_LARGE */
void TextUIStreamProxy::setFontSize(FontSize_t sz) {

}

uint8_t TextUIStreamProxy::getRows() {

}

uint8_t TextUIStreamProxy::getColumns() {

}

/* row and column in characters */
void TextUIStreamProxy::setCursor(uint8_t r, uint8_t c) {

}

void TextUIStreamProxy::setRow(uint8_t r) {

}

void TextUIStreamProxy::setColumn(uint8_t c) {

}

void TextUIStreamProxy::printChar(char ch) {

}
