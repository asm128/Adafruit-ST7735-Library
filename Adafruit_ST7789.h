#ifndef _ADAFRUIT_ST7789H_
#define _ADAFRUIT_ST7789H_

#include "Adafruit_ST77xx.h"

/// Subclass of ST77XX type display for ST7789 TFT Driver
class Adafruit_ST7789 : public Adafruit_ST77xx {
protected:
  uint8_t _colstart2 = 0, ///< Offset from the right
      _rowstart2 = 0;     ///< Offset from the bottom
  uint16_t windowWidth  = 240;
  uint16_t windowHeight = 135;
public:
    inline Adafruit_ST7789(int8_t CS, int8_t RS, int8_t RST)                            : Adafruit_ST77xx(240, 135, CS, RS, RST) {}
    inline Adafruit_ST7789(int8_t CS, int8_t RS, int8_t MOSI, int8_t SCLK, int8_t RST)  : Adafruit_ST77xx(240, 135, CS, RS, MOSI, SCLK, RST) {}
#ifndef ESP8266
    inline Adafruit_ST7789(SPIClass *spiClass, int8_t CS, int8_t RS, int8_t RST)        : Adafruit_ST77xx(240, 135, spiClass, CS, RS, RST) {}
#endif
 // end !ESP8266

  void setRotation(uint8_t m);
  void init(uint16_t width, uint16_t height, uint8_t spiMode = SPI_MODE0);

};

#endif // _ADAFRUIT_ST7789H_
