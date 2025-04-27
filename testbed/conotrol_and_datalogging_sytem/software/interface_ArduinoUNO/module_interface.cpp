#include "module_interface.h"
#include <Arduino.h>
#include <Wire.h>

/** VARIABLE DECLARATIONS **/

// I2C interactions
const unsigned char I2C_STATE_INVALID = 0b11111111;
unsigned char i2c_received[2] = {I2C_STATE_INVALID, I2C_STATE_INVALID};
// One byte shared read write register
unsigned char read_write_register = 0;
// Remotely invokable routine
void (*invokable_routine)() = NULL;

/** FUNCTIONS **/

/**
 * Setter for read_write_register
 * @param value Sets the (local) value of the read write register
 */
void set_read_write_register(unsigned char value) {
  read_write_register = value;
}

/**
 * Getter for read_write_register
 * @return The (local) value of the read write register
 */
unsigned char get_read_write_register() {
  return read_write_register;
}

/**
 * Sets the routine that can be remotely invoked
 * @param invokable_routine The function that will be called. It must return void and take no arguments 
 */
void set_invokable_routine(void (*routine)()) {
  invokable_routine = routine;
}

// Early local function definitions
void receiveEvent(int data_size_chars);
void requestEvent();
void i2cWriteByte(byte write_char);
void i2cWriteBytes(byte* data, int no_bytes);

/**
 * Initializes the I2C device with the module interface functionalities, using the Arduino Wire.h library
 * @param I2C_device_addr HEX-address on the I2C bus. Example: 0x1a
 * @param I2C_clk_freq Configures the I2C clock speed
 */
void i2cInitialize(int I2C_device_addr, unsigned long I2C_clk_freq) {
  Wire.setClock(I2C_clk_freq);
  i2cInitialize(I2C_device_addr);
}

/**
 * Initializes the I2C device with the module interface functionalities, using the Arduino Wire.h library
 * @param I2C_device_addr HEX-address on the I2C bus. Example: 0x1a
 */
void i2cInitialize(int I2C_device_addr) {
  Wire.begin(I2C_device_addr);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  if(VERBOSE)Serial.begin(9600);
}

/**
 * Implementation of the standard void requestEvent() routine for the Wire.h library
 */
void requestEvent() {
  if(VERBOSE){Serial.print("I/O requestEvent() with i2c_received[0]=");Serial.println((unsigned char)i2c_received[0]);}
  switch(i2c_received[0]) {
    case 0: {// acknowledge: return 1
      if(VERBOSE)Serial.println("I/O requestEvent() - ack");
      i2cWriteByte(1);
      return;
    }
    case 1: {// time_sync: return local runtime in milliseconds
      if(VERBOSE)Serial.println("I/O requestEvent() - time sync");
      unsigned long t_local_millis = millis();
      i2cWriteBytes((byte*) &t_local_millis, sizeof t_local_millis);
      return;
    }
    case 3: {// return read_write_register value
      if(VERBOSE)Serial.println("I/O requestEvent() - return value of read_write_register");
      i2cWriteBytes((byte*) &read_write_register, sizeof read_write_register);
      return;
    }
    case 10: {// example for adding functionality
      if(VERBOSE)Serial.println("I/O requestEvent() - INFO: example for adding functionality");
      unsigned char returnvalue = 0;
      i2cWriteBytes((byte*) &returnvalue, sizeof returnvalue);
      return;
    }
    case 100: {
      if(VERBOSE)Serial.println("I/O requestEvent() - test");
      bool b = true;
      char i1 = -123;
      unsigned char u1 = 250;
      int i2 = -32100;
      unsigned int u2 = 65432;
      long i4 = -1234567890;
      unsigned long u4 = 4567890123;
      float d4_1 = -0.1234567890;
      float d4_2 = 12345.67890;
      double d4_3 = -0.1234567890; // is the same (the same (the same!)~)* datatype as float for Arduino UNOs
      i2cWriteBytes((byte*) &b, sizeof b);
      i2cWriteBytes((byte*) &i1, sizeof i1);
      i2cWriteBytes((byte*) &u1, sizeof u1);
      i2cWriteBytes((byte*) &i2, sizeof i2);
      i2cWriteBytes((byte*) &u2, sizeof u2);
      i2cWriteBytes((byte*) &i4, sizeof i4);
      i2cWriteBytes((byte*) &u4, sizeof u4);
      i2cWriteBytes((byte*) &d4_1, sizeof d4_1);
      i2cWriteBytes((byte*) &d4_2, sizeof d4_2);
      i2cWriteBytes((byte*) &d4_3, sizeof d4_3);
      return;
    }
    case I2C_STATE_INVALID: {
      if(VERBOSE)Serial.println("I/O requestEvent() - WARNING: i2c_received[0] has value I2C_STATE_INVALID");
      i2cWriteByte(I2C_STATE_INVALID);
      return;
    }
    default: {
      if(VERBOSE)Serial.println("I/O requestEvent() - WARNING: unknown value of i2c_received[0]");
      i2cWriteByte(I2C_STATE_INVALID);
      return;
    }
  }
}

/**
 * Implementation of the standard void receiveEvent(int) routine for the Wire.h library
 * @param data_size_chars See the documentation of the Wire.h library. Number of bytes available to read from the I2C bus
 */
void receiveEvent(int data_size_chars) {
  // update trail of received bytes
  i2c_received[1] = i2c_received[0];

  // the following section reads the i2c buffer, checking that exacly one char of data has been written (maybe too much checking)
  if(data_size_chars!=1) {
    i2c_received[0] = I2C_STATE_INVALID;
    return;
  }

  if(!Wire.available()) {
    i2c_received[0] = I2C_STATE_INVALID;
    return;
  }

  // actual read from the I2C bus
  i2c_received[0] = Wire.read();

  if (Wire.available()) {
    i2c_received[0] = I2C_STATE_INVALID;
    return;
  }

  if(VERBOSE){Serial.print("I/O receiveEvent() with i2c_received[0]=");Serial.println((unsigned char)i2c_received[0]);}

  // section to write to read write register
  // IMPORTANT: must be placed after setting i2c_received and before any further processing
  if(i2c_received[1]==2) {
    if(VERBOSE)Serial.println("receiveEvent() - write to read_write_register");
    read_write_register = i2c_received[0];
  }

  // routine to remotely call the invokable_routine
  if(i2c_received[0]==5) {
    if(VERBOSE)Serial.println("receiveEvent() - call invokable_routine");
    if(invokable_routine != NULL) {
      invokable_routine();
    } else {
      if(VERBOSE)Serial.println("receiveEvent() - WARNING: invokable_routine is NULL");
    }
  }

  // example for adding functionality
  if(i2c_received[0]==10) {
    if(VERBOSE)Serial.println("receiveEvent() - INFO: example for adding functionality");
  }
}

/**
 * Wrapper for the actual writing to the I2C bus
 * @param write_char byte to be written to the I2C bus
 */
void i2cWriteByte(byte write_char) {
  Wire.write(write_char);
}

/**
 * Write a memory section to the I2C bus. This is an abstraction to facilitate passing different types of data. Example: The value of a variable VAR of any primitive type can be written to the I2C bus as i2cWriteBytes((byte*) &VAR, sizeof VAR);
 * @param data pointer to the start location of the memory section to be written to the I2C bus
 * @param no_bytes number of bytes to be written to the I2C bus
 */
void i2cWriteBytes(byte* data, int no_bytes) {
  if(VERBOSE){Serial.print("I/O i2cWriteBytes no bytes ");Serial.println(no_bytes);}
  for(int i=no_bytes-1; i>=0; i--) {
    if(VERBOSE)Serial.print("I2C << ");Serial.println(data[i], BIN);
    i2cWriteByte(data[i]);
  }
}