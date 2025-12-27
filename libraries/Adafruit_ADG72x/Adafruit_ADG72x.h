/*!
 * @file Adafruit_ADG72x.h
 * This is the documentation for Adafruit's ADG72x driver for the
 * Arduino platform. It is designed specifically to work with the
 * ADG728 and ADG729 analog multiplexers.
 *
 * These devices use I2C to communicate, and require 2 pins (SCL+SDA)
 * Two more pins can be used for the optional address configuration,
 * allowing multiple devices to share the same I2C bus.
 *
 * Adafruit invests time and resources providing this open-source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 * MIT license, all text above must be included in any redistribution.
 */

#ifndef _ADAFRUIT_ADG72X_H
#define _ADAFRUIT_ADG72X_H

#include "Arduino.h"
#include <Adafruit_I2CDevice.h>

/*! @brief  Default I2C address for ADG728 */
#define ADG728_DEFAULT_ADDR 0x4C
/*! @brief  Default I2C address for ADG729 */
#define ADG729_DEFAULT_ADDR 0x44

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          ADG72x analog multiplexers
 */
class Adafruit_ADG72x {
public:
  Adafruit_ADG72x(); ///< Constructor for ADG72x class
  bool begin(uint8_t i2c_addr,
             TwoWire *wire = &Wire); ///< Initializes the ADG72x
  bool selectChannels(uint8_t bits); ///< Selects channels based on bits

private:
  Adafruit_I2CDevice *i2c_dev = nullptr; ///< Pointer to I2C device
};

#endif // _ADAFRUIT_ADG72X_H
