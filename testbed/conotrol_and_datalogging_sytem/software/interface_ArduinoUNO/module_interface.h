#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

#include <Arduino.h>

// Set to true for debugging and development
#define VERBOSE true

/**
 * Initializes the I2C device with the module interface functionalities, using the Arduino Wire.h library
 * @param I2C_device_addr HEX-address on the I2C bus. Example: 0x1a
 * @param I2C_clk_freq Configures the I2C clock speed
 */
 void i2cInitialize(int I2C_device_addr, unsigned long I2C_clk_freq);

/**
 * Initializes the I2C device with the module interface functionalities, using the Arduino Wire.h library
 * @param I2C_device_addr HEX-address on the I2C bus. Example: 0x1a
 */
void i2cInitialize(int I2C_device_addr);

/**
 * Setter for read_write_register
 * @param value Sets the (local) value of the read write register
 */
void set_read_write_register(unsigned char value);

/**
 * Getter for read_write_register
 * @return The (local) value of the read write register
 */
unsigned char get_read_write_register();

/**
 * Sets the routine that can be remotely invoked
 * @param invokable_routine The function that will be called. It must return void and take no arguments 
 */
void set_invokable_routine(void (*invokable_routine)());

#endif