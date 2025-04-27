#include "module_interface.h"

void setup() {
  Serial.begin(9600);
  const int MODULE_I2C_ADDRESS = 0x1a;
  i2cInitialize(MODULE_I2C_ADDRESS);
  set_invokable_routine(advance_rw_register);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned char rw_register = get_read_write_register();
  digitalWrite(LED_BUILTIN, (rw_register%2));
  delay(1000);
}

/**
 * Increases the value in the shared read_write_register
 */
void advance_rw_register() {
  Serial.println("advance_rw_register()");

  unsigned char rw_reg_old = get_read_write_register();
  unsigned char rw_reg_new = (rw_reg_old+1)%256;
  set_read_write_register(rw_reg_new);

  Serial.print("read_write_register from ");Serial.print(rw_reg_old);Serial.print(" to ");Serial.println(rw_reg_new);
}
