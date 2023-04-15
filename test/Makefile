#
#  MIT License
#
#  Copyright (c) 2023 wlowi
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

CXX = $(shell wx-config --cxx)
PROGRAM = RunTest

# wx-config --libs
WX_LIBS = $(shell wx-config --libs)

# wx-config --cxxflags
WX_CXXFLAGS_GEN = $(shell wx-config --cxxflags)
WX_CXXFLAGS = $(subst -I,-isystem,$(WX_CXXFLAGS_GEN))

CXXFLAGS += -g -O0 -Wall -Wpedantic -Wextra -Wno-unused-parameter
CXXINC += -I. -Iemu

# Objects

EMU_OBJ = emu/EEPROM.o emu/EmuTextUILcdSSD1306.o emu/EmuTextUILcdST7735.o emu/EmuTextUISimpleKbd.o
#EMU_OBJ = emu/EEPROM.o emu/EmuTextUILcdST7735.o emu/EmuTextUISimpleKbd.o

UI_OBJ = TextUI.o TextUIHandler.o TextUILcd.o TextUIMenu.o Cell.o

OBJECTS = TextUITest.o screen/HomeScreen.o screen/DataScreen.o screen/ChannelScreen.o screen/ConfigScreen.o $(UI_OBJ) 

# rules

.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(CXXINC) $(WX_CXXFLAGS) -c  -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS) $(EMU_OBJ) $(PROGRAM).o
	$(CXX) -g $(CXXINC) -o $(PROGRAM) $(PROGRAM).o $(EMU_OBJ) $(OBJECTS) $(WX_LIBS) 

clean:
	rm -f $(PROGRAM) $(PROGRAM).o $(OBJECTS) $(EMU_OBJ)

# dependencies

TextUI.o: TextUI.cpp TextUI.h Arduino.h

TextUIHandler.o: TextUIHandler.cpp TextUI.h Arduino.h

TextUILcd.o: TextUILcd.cpp TextUI.h Arduino.h

TextUIMenu.o: TextUIMenu.cpp TextUI.h Arduino.h

Cell.o: Cell.cpp TextUI.h Arduino.h


TextUITest.o: TextUI.h Arduino.h

RunTest.o: emu/EmuTextUILcdSSD1306.h emu/EmuTextUILcdST7735.h emu/EmuTextUISimpleKbd.h emu/EEPROM.h Arduino.h


