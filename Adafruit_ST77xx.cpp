/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include "Adafruit_ST77xx.h"
#include <limits.h>
#if !defined(ARDUINO_STM32_FEATHER) && !defined(ARDUINO_UNOR4_WIFI)
#if !defined(ARDUINO_UNOR4_MINIMA)
#include "pins_arduino.h"
#include "wiring_private.h"
#endif
#endif
#include <SPI.h>

#define SPI_DEFAULT_FREQ 32000000 ///< Default SPI data clock frequency

/**************************************************************************/
/*!
    @brief  Companion code to the initiliazation tables. Reads and issues
            a series of LCD commands stored in PROGMEM byte array.
    @param  addr  Flash memory array with commands and data to send
*/
/**************************************************************************/
void Adafruit_ST77xx::displayInit(const uint8_t *addr) {

  uint8_t numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++); // Number of commands to follow
  while (numCommands--) {              // For each command...
    cmd = pgm_read_byte(addr++);       // Read command
    numArgs = pgm_read_byte(addr++);   // Number of args to follow
    ms = numArgs & ST_CMD_DELAY;       // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;          // Mask out delay bit
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;

    if (ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if (ms == 255)
        ms = 500; // If 255, delay for 500 ms
      delay(ms);
    }
  }
}

/**************************************************************************/
/*!
    @brief  Initialize ST77xx chip. Connects to the ST77XX over SPI and
            sends initialization procedure commands
    @param  freq  Desired SPI clock frequency
*/
/**************************************************************************/
void Adafruit_ST77xx::begin(uint32_t freq) {
  _freq = freq ? freq : SPI_DEFAULT_FREQ;

  invertOnCommand = ST77XX_INVON;
  invertOffCommand = ST77XX_INVOFF;

  initSPI(_freq, spiMode);
}

/**************************************************************************/
/*!
  @brief  SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner y coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
/**************************************************************************/
void Adafruit_ST77xx::setAddrWindow(uint16_t x, uint16_t y, uint16_t w,
                                    uint16_t h) {
  x += _xstart;
  y += _ystart;
  uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
  uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

  writeCommand(ST77XX_CASET); // Column addr set
  SPI_WRITE32(xa);

  writeCommand(ST77XX_RASET); // Row addr set
  SPI_WRITE32(ya);

  writeCommand(ST77XX_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) and orientation of TFT display
    @param  m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Adafruit_ST77xx::setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m % 4; // can't be higher than 3

  switch (rotation) {
  case 0: _xstart = _colstart; _ystart = _rowstart; madctl = ST77XX_MADCTL_RGB | ST77XX_MADCTL_MX | ST77XX_MADCTL_MY; break;
  case 1: _ystart = _colstart; _xstart = _rowstart; madctl = ST77XX_MADCTL_RGB | ST77XX_MADCTL_MY | ST77XX_MADCTL_MV; break;
  case 2: _xstart = _colstart; _ystart = _rowstart; madctl = ST77XX_MADCTL_RGB; break;
  case 3: _ystart = _colstart; _xstart = _rowstart; madctl = ST77XX_MADCTL_RGB | ST77XX_MADCTL_MX | ST77XX_MADCTL_MV; break;
  }

  sendCommand(ST77XX_MADCTL, &madctl, 1);
}

////////// stuff not actively being used, but kept for posterity
/*

 uint8_t Adafruit_ST77xx::spiread(void) {
 uint8_t r = 0;
 if (_sid > 0) {
 r = shiftIn(_sid, _sclk, MSBFIRST);
 } else {
 //SID_DDR &= ~_BV(SID);
 //int8_t i;
 //for (i=7; i>=0; i--) {
 //  SCLK_PORT &= ~_BV(SCLK);
 //  r <<= 1;
 //  r |= (SID_PIN >> SID) & 0x1;
 //  SCLK_PORT |= _BV(SCLK);
 //}
 //SID_DDR |= _BV(SID);

 }
 return r;
 }

 void Adafruit_ST77xx::dummyclock(void) {

 if (_sid > 0) {
 digitalWrite(_sclk, LOW);
 digitalWrite(_sclk, HIGH);
 } else {
 // SCLK_PORT &= ~_BV(SCLK);
 //SCLK_PORT |= _BV(SCLK);
 }
 }
 uint8_t Adafruit_ST77xx::readdata(void) {
 *portOutputRegister(rsport) |= rspin;

 *portOutputRegister(csport) &= ~ cspin;

 uint8_t r = spiread();

 *portOutputRegister(csport) |= cspin;

 return r;

 }

 uint8_t Adafruit_ST77xx::readcommand8(uint8_t c) {
 digitalWrite(_rs, LOW);

 *portOutputRegister(csport) &= ~ cspin;

 spiwrite(c);

 digitalWrite(_rs, HIGH);
 pinMode(_sid, INPUT); // input!
 digitalWrite(_sid, LOW); // low
 spiread();
 uint8_t r = spiread();


 *portOutputRegister(csport) |= cspin;


 pinMode(_sid, OUTPUT); // back to output
 return r;
 }


 uint16_t Adafruit_ST77xx::readcommand16(uint8_t c) {
 digitalWrite(_rs, LOW);
 if (_cs)
 digitalWrite(_cs, LOW);

 spiwrite(c);
 pinMode(_sid, INPUT); // input!
 uint16_t r = spiread();
 r <<= 8;
 r |= spiread();
 if (_cs)
 digitalWrite(_cs, HIGH);

 pinMode(_sid, OUTPUT); // back to output
 return r;
 }

 uint32_t Adafruit_ST77xx::readcommand32(uint8_t c) {
 digitalWrite(_rs, LOW);
 if (_cs)
 digitalWrite(_cs, LOW);
 spiwrite(c);
 pinMode(_sid, INPUT); // input!

 dummyclock();
 dummyclock();

 uint32_t r = spiread();
 r <<= 8;
 r |= spiread();
 r <<= 8;
 r |= spiread();
 r <<= 8;
 r |= spiread();
 if (_cs)
 digitalWrite(_cs, HIGH);

 pinMode(_sid, OUTPUT); // back to output
 return r;
 }

 */
