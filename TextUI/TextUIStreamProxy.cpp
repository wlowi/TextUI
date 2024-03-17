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

TextUIStreamProxy::TextUIStreamProxy(Stream &s) : stream(s)
{
  currentMode = COMMAND_MODE;
}

/* TextUIInput */
bool TextUIStreamProxy::pending()
{
  /** @todo */
  return false;
}

void TextUIStreamProxy::setEvent(Event *e)
{
  /** @todo */
}

/* TextUILcd */
void TextUIStreamProxy::clear()
{
  simpleCommand(COMMAND_CLEAR);
}

void TextUIStreamProxy::clearEOL()
{
  simpleCommand(COMMAND_CLEAR_EOL);
}

bool TextUIStreamProxy::colorSupport()
{
  queryCommand( COMMAND_GET_COLORSUPPORT);

  /** @todo */
  return true;
}

void TextUIStreamProxy::setBg(uint8_t r, uint8_t g, uint8_t b)
{
  byteCommand3(COMMAND_SET_BG, r, g, b);
}

void TextUIStreamProxy::setFg(uint8_t r, uint8_t g, uint8_t b)
{
  byteCommand3(COMMAND_SET_FG, r, g, b);
}

void TextUIStreamProxy::normalColors()
{
  simpleCommand(COMMAND_NORMAL_COLORS);
}

void TextUIStreamProxy::selectedColors()
{
  simpleCommand(COMMAND_SELECTED_COLORS);
}

void TextUIStreamProxy::editColors()
{
  simpleCommand(COMMAND_EDIT_COLORS);
}

void TextUIStreamProxy::setInvert(bool inv)
{
  byteCommand(COMMAND_SET_INVERT, (uint8_t)inv);
}

/* FONT_SMALL .. FONT_LARGE */
void TextUIStreamProxy::setFontSize(FontSize_t sz)
{
  byteCommand(COMMAND_SET_FONTSIZE, (uint8_t)sz);
}

uint8_t TextUIStreamProxy::getRows()
{
  queryCommand( COMMAND_GET_ROWS);

  /** @todo */
  return 8;
}

uint8_t TextUIStreamProxy::getColumns()
{
  queryCommand( COMMAND_GET_COLUMNS);

  /** @todo */
  return 13;
}

/* row and column in characters */
void TextUIStreamProxy::setCursor(uint8_t r, uint8_t c)
{
  toCommandMode();
  send(CMD_SET_CURSOR);
  sendByte(r);
  sendByte(c);
  currentMode = PRINT_MODE;
}

void TextUIStreamProxy::setRow(uint8_t r)
{
  toCommandMode();
  send(CMD_SET_ROW);
  sendByte(r);
  currentMode = PRINT_MODE;
}

void TextUIStreamProxy::setColumn(uint8_t c)
{
  toCommandMode();
  send(CMD_SET_COLUMN);
  sendByte(c);
  currentMode = PRINT_MODE;
}

void TextUIStreamProxy::printChar(char ch)
{
  toPrintMode();
  if (IS_PRINTABLE(ch))
  {
    send(ch);
  }
}

void TextUIStreamProxy::toCommandMode()
{
  if (currentMode != COMMAND_MODE)
  {
    send(CMD_END);
    currentMode = COMMAND_MODE;
  }
}

void TextUIStreamProxy::toPrintMode()
{
  if (currentMode != PRINT_MODE)
  {
    send(CMD_PRINT);
    currentMode = PRINT_MODE;
  }
}

void TextUIStreamProxy::simpleCommand(commandType_t cmd)
{
  toCommandMode();
  send(CMD_COMMAND);
  send(COMMAND_CH1(cmd));
  send(COMMAND_CH2(cmd));
  send(CMD_END);
}

void TextUIStreamProxy::byteCommand(commandType_t cmd, uint8_t u)
{
  toCommandMode();
  send(CMD_COMMAND);
  send(COMMAND_CH1(cmd));
  send(COMMAND_CH2(cmd));
  sendByte(u);
  send(CMD_END);
}

void TextUIStreamProxy::byteCommand2(commandType_t cmd, uint8_t u, uint8_t v)
{
  toCommandMode();
  send(CMD_COMMAND);
  send(COMMAND_CH1(cmd));
  send(COMMAND_CH2(cmd));
  sendByte(u);
  sendByte(v);
  send(CMD_END);
}

void TextUIStreamProxy::byteCommand3(commandType_t cmd, uint8_t u, uint8_t v, uint8_t w)
{
  toCommandMode();
  send(CMD_COMMAND);
  send(COMMAND_CH1(cmd));
  send(COMMAND_CH2(cmd));
  sendByte(u);
  sendByte(v);
  sendByte(w);
  send(CMD_END);
}

void TextUIStreamProxy::queryCommand( commandType_t cmd)
{
  toCommandMode();
  send(CMD_QUERY);
  send(COMMAND_CH1(cmd));
  send(COMMAND_CH2(cmd));
  send(CMD_END);
}

void TextUIStreamProxy::send(char ch)
{
  stream.write(ch);
}

void TextUIStreamProxy::sendByte(uint8_t ch)
{
  stream.print(ch);
  stream.write(';');
}