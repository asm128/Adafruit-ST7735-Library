// Adafruit_ST7796S.cpp
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

#include "Adafruit_ST7796S.h"

/**
 * @brief Initialize the display.
 * @param width Display width in pixels.
 * @param height Display height in pixels.
 * @param rowOffset Row offset for display.
 * @param colOffset Column offset for display.
 * @param colorOrder Color order (RGB or BGR).
 */
void Adafruit_ST7796S::init(uint16_t width, uint16_t height, uint8_t rowOffset,
                            uint8_t colOffset, ST7796S_ColorOrder colorOrder) {
  _width = width;
  _height = height;
  _rowstart = rowOffset;
  _colstart = colOffset;
  _colorOrder = colorOrder;
  windowWidth = width;
  windowHeight = height;

  commonInit(NULL);
  displayInit(st7796s_init);
  invertOnCommand = ST77XX_INVOFF;
  invertOffCommand = ST77XX_INVON;
  invertDisplay(false);
  setRotation(0);
}

/**
 * @brief Set the display rotation.
 * @param m Rotation value (0-3).
 */
void Adafruit_ST7796S::setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m & 3; // can't be higher than 3

  switch (rotation) {
  case 0: _xstart = _colstart; _ystart = _rowstart; _width  = windowWidth; _height = windowHeight; madctl = _colorOrder | ST77XX_MADCTL_MX | ST77XX_MADCTL_RGB; break;
  case 1: _xstart = _rowstart; _ystart = _colstart; _height = windowWidth; _width  = windowHeight; madctl = _colorOrder | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB; break;
  case 2: _xstart = _colstart; _ystart = _rowstart; _width  = windowWidth; _height = windowHeight; madctl = _colorOrder | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB; break;
  case 3: _xstart = _rowstart; _ystart = _colstart; _height = windowWidth; _width  = windowHeight; madctl = _colorOrder | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB | ST77XX_MADCTL_MX  | ST77XX_MADCTL_MV; break;
  }
  Serial.println(madctl, HEX);
  sendCommand(ST77XX_MADCTL, &madctl, 1);
}
