// Adafruit_ST7796S.h
//
// MIT License
//
// Copyright (c) 2025 Limor Fried/Ladyada for Adafruit Industries
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _ADAFRUIT_ST7796S_H_
#define _ADAFRUIT_ST7796S_H_

#include "Adafruit_ST77xx.h"

/**
 * @brief Adafruit driver for the ST7796S TFT display.
 */
class Adafruit_ST7796S : public Adafruit_ST77xx {
    ST7796S_ColorOrder _colorOrder    = ST7796S_RGB;     ///< Color order setting.
    uint16_t            windowWidth   = ST7796S_TFTWIDTH;
    uint16_t            windowHeight  = ST7796S_TFTHEIGHT; ///< Dimensions of the display window.
public:
    inline Adafruit_ST7796S(int8_t CS, int8_t RS, int8_t RST)                            : Adafruit_ST77xx(ST7796S_TFTWIDTH, ST7796S_TFTHEIGHT, CS, RS, RST) {}
    inline Adafruit_ST7796S(int8_t CS, int8_t RS, int8_t MOSI, int8_t SCLK, int8_t RST)  : Adafruit_ST77xx(ST7796S_TFTWIDTH, ST7796S_TFTHEIGHT, CS, RS, MOSI, SCLK, RST) {}
#ifndef ESP8266
    inline Adafruit_ST7796S(SPIClass *spiClass, int8_t CS, int8_t RS, int8_t RST)        : Adafruit_ST77xx(ST7796S_TFTWIDTH, ST7796S_TFTHEIGHT, spiClass, CS, RS, RST) {}
#endif
    void init(uint16_t width = ST7796S_TFTWIDTH, uint16_t height = ST7796S_TFTHEIGHT, uint8_t rowOffset = 0, uint8_t colOffset = 0, ST7796S_ColorOrder colorOrder = ST7796S_RGB);
    void setRotation(uint8_t r);
};

#endif
