"""This example scetch showcases the functionalities for the provided Arduino UNO interface implementation"""

from arduinomodule import ArduinoModule

MODULE_I2C_ADDRESS = 0x1a
MODULE_ENABLE_GPIO_PIN = 11
I2C_BUS_NUMBER = 1

debugging = False
m = ArduinoModule(11, 0x1a, 1, True)

m.powerUp()
m.I2C_ack()
if not debugging:
  m.I2C_time_sync()
  m.I2C_set_read_write_register(123)
  m.I2C_get_read_write_register()
  m.I2C_call_invokable_routine()
  m.I2C_get_read_write_register()
  m.I2C_example_for_adding_functionality()
  m.I2C_test()
m.powerDown()