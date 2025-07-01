#include "Adafruit_ST77xx.h"

void Adafruit_ST7789::setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m & 3; // can't be higher than 3
  switch (rotation) {  
  case 0: _xstart = _colstart ; _ystart = _rowstart ; _width  = this->WIDTH; _height = this->HEIGHT; madctl = ST77XX_MADCTL_RGB | ST77XX_MADCTL_MX | ST77XX_MADCTL_MY; break; 
  case 1: _xstart = _rowstart ; _ystart = _colstart2; _height = this->WIDTH; _width  = this->HEIGHT; madctl = ST77XX_MADCTL_RGB | ST77XX_MADCTL_MY | ST77XX_MADCTL_MV; break; 
  case 2: _xstart = _colstart2; _ystart = _rowstart2; _width  = this->WIDTH; _height = this->HEIGHT; madctl = ST77XX_MADCTL_RGB; break; 
  case 3: _xstart = _rowstart2; _ystart = _colstart ; _height = this->WIDTH; _width  = this->HEIGHT; madctl = ST77XX_MADCTL_RGB | ST77XX_MADCTL_MX | ST77XX_MADCTL_MV; break; 
  }
  sendCommand(ST77XX_MADCTL, &madctl, 1);
}


/**************************************************************************/
/*!
    @brief  Initialization code common to all ST7789 displays
    @param  width  Display width
    @param  height Display height
    @param  mode   SPI data mode; one of SPI_MODE0, SPI_MODE1, SPI_MODE2 SPI_MODE3 (do NOT pass the numbers 0,1,2 or 3 -- use defines only, the values are NOT the same!)
*/
/**************************************************************************/
void Adafruit_ST7789::init(uint16_t width, uint16_t height, uint8_t mode) {
  // Save SPI data mode. commonInit() calls begin() (in Adafruit_ST77xx.cpp), which in turn calls initSPI() (in Adafruit_SPITFT.cpp), passing it the
  // value of spiMode. It's done this way because begin() really should not be modified at this point to accept an SPI mode -- it's a virtual
  // function required in every Adafruit_SPITFT subclass and would require updating EVERY such library...whereas, at the moment, we know that
  // certain ST7789 displays are the only thing that may need a non-default SPI mode, hence this roundabout approach...
  spiMode = mode;
  // (Might get added similarly to other display types as needed on a case-by-case basis.)
  commonInit(NULL);
  if (width == 240 && height == 240) { // 1.3", 1.54" displays (right justified)
    _rowstart = (320 - height);
    _rowstart2 = 0;
    _colstart = _colstart2 = (240 - width);
  } else if (width == 135 && height == 240) { // 1.14" display (centered, with odd size)
    _rowstart = _rowstart2 = (int)((320 - height) / 2); // This is the only device currently supported device that has different values for _colstart & _colstart2. You must ensure that the extra pixel lands in _colstart and not in _colstart2
    _colstart = (int)((240 - width + 1) / 2);
    _colstart2 = (int)((240 - width) / 2);
  } else { // 1.47", 1.69, 1.9", 2.0" displays (centered)
    _rowstart = _rowstart2 = (int)((320 - height) / 2);
    _colstart = _colstart2 = (int)((240 - width) / 2);
  }

  this->_width   = this->WIDTH   = width;
  this->_height  = this->HEIGHT  = height;

  displayInit(generic_st7789);
  setRotation(0);
}
