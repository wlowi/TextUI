# TextUI

A text based user interface for Arduinos.

## Features

- Supports various OLED and TFT displays
- Supports simple buttons and/or rotary encoder
- Supports multiple input devices
- Supports external user terminal via serial line

## Documentation

For API users guide see: doc/html/index.html.

## External User Terminal

An external user terminal is a separate box that contains the display, the input device and a 
Arduino Nano or Arduino Pro Mini controller.

The main controller runs the user interface code and is connected to the user terminal via serial line.

To make use of an external user terminal, replace the TextUILcd... and TextUIInput... classes with TextUIStreamProxy. 

The code for the external user terminal is in directory UserTerm. An example that shows how to use it is in examples/StreamProxy.

NOTE:  
To compile UserTerm.ino, temporary copy all files from TextUI/* to UserTerm/

## Examples

See examples directory
