/*!
 * @file Adafruit_ADG72x.cpp
 * @author Limor Fried/Ladyada
 * @brief  This is a library for the ADG728/ADG729 analog multiplexers
 * @section license License
 *
 * MIT License, Please see the file called LICENSE for full details
 */

#include "Adafruit_ADG72x.h"

/*!
 *    @brief  Constructor for the ADG72x class.
 */
Adafruit_ADG72x::Adafruit_ADG72x() {}

/*!
 *    @brief  Begins communication with the ADG72x chip
 *    @param  i2c_addr I2C address of the device, no default given
 *    @param  wire Pointer to I2C interface object, defaults to &Wire
 *    @return True if I2C init was successful, otherwise false.
 */
bool Adafruit_ADG72x::begin(uint8_t i2c_addr, TwoWire *wire) {
  if (i2c_dev != nullptr) {
    delete i2c_dev; // Prevent memory leak if begin() is called again
  }
  i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
  return i2c_dev->begin();
}

/*!
 *    @brief  Selects the channels on the ADG72x chip based on the provided bits
 *            Each bit turns on a single channel, you can have more than 1
 * channel enabled to connect many analog signals together!
 *    @param  bits 8-bit value representing the channels to be
 * selected/deselected
 *    @return True if the I2C operation was successful, otherwise false.
 */
bool Adafruit_ADG72x::selectChannels(uint8_t bits) {
  if (!i2c_dev) {
    return false; // I2C device not initialized
  }
  return i2c_dev->write(&bits, 1);
}
