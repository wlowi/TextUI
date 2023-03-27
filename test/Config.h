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

#ifndef _Config_h_
#define _Config_h_

#include "Arduino.h"
#include "TextUI.h"

typedef uint16_t checksum_t;

typedef struct config_t
{
    uint16_t pulseValidMin_usec;
    uint16_t pulseValidMax_usec;

    uint16_t servoValidMin_usec;
    uint16_t servoValidMax_usec;

    uint16_t syncValidMin_usec;

    int8_t   vppmAdjust;
    int8_t   vccAdjust;

    /* */

    checksum_t checksum;

} config_t;

#define PPM_MAX_CHANNELS 16

typedef struct ppm_t {
  
    bool     sync;
    uint8_t  channels;
    uint16_t frameMin_usec;
    uint16_t frameMax_usec;
    uint16_t pulseMin_usec;
    uint16_t pulseMax_usec;
    fixfloat1_t vLevel_low;
    fixfloat2_t vLevel_high;
    bool     pulseLevel;
    uint32_t frames;
    uint16_t badFrames;
    uint16_t badCount;
    uint16_t badPulse;
    uint16_t channel_usec[PPM_MAX_CHANNELS];
    uint16_t channelMin_usec[PPM_MAX_CHANNELS];
    uint16_t channelMax_usec[PPM_MAX_CHANNELS];
} ppm_t;

#endif
