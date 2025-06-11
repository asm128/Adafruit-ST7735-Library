#ifndef _ADAFRUIT_ST7735H_
#define _ADAFRUIT_ST7735H_

#include "Adafruit_ST77xx.h"

/// Subclass of ST77XX for ST7735B and ST7735R TFT Drivers:
class Adafruit_ST7735 : public Adafruit_ST77xx {
  uint8_t tabcolor = 0;
public:
    inline Adafruit_ST7735(int8_t CS = -1, int8_t RS = -1, int8_t RST = -1)                         : Adafruit_ST77xx(ST7735_TFTWIDTH_80, ST7735_TFTHEIGHT_160, CS, RS, RST) {}
    inline Adafruit_ST7735(int8_t CS, int8_t RS, int8_t MOSI, int8_t SCLK, int8_t RST = -1)    : Adafruit_ST77xx(ST7735_TFTWIDTH_80, ST7735_TFTHEIGHT_160, CS, RS, MOSI, SCLK, RST) {}
#ifndef ESP8266
    inline Adafruit_ST7735(SPIClass * spiClass, int8_t CS, int8_t RS, int8_t RST)        : Adafruit_ST77xx(ST7735_TFTWIDTH_80, ST7735_TFTHEIGHT_160, spiClass, CS, RS, RST) {}
#endif
    // Differences between displays (usu. identified by colored tab on plastic overlay) are odd enough that we need to do this 'by hand':
    void initB();                             // for ST7735B displays
    void initR(uint8_t options = INITR_GREENTAB); // for ST7735R
    void setRotation(uint8_t m);
};

#endif // _ADAFRUIT_ST7735H_
