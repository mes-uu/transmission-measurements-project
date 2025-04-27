from module import Module
import time

class ArduinoModule(Module):
  """Example implementation of the interface for an Arduino UNO module"""

  def powerUp(self):
    self.setPower(True)
    time.sleep(5)

  def powerDown(self):
    self.setPower(False)

  def I2C_ack(self):
    cmd = b'\x00'
    ack = self.get_I2C().readByte(cmd)
    print("ack %s >> %s" % (str(cmd), str(ack)))

  def I2C_time_sync(self):
    cmd = int.to_bytes(1)
    t_arduino_millis = self.get_I2C().readPrimitive("u4",cmd)
    print("time_sync %s >> %s ms" % (str(cmd), str(t_arduino_millis)))
    return t_arduino_millis

  def I2C_set_read_write_register(self, int_value):
    cmd = int.to_bytes(2)
    self.get_I2C().writeByte(cmd)
    self.get_I2C().writeByte(int.to_bytes(int_value))

  def I2C_get_read_write_register(self):
    cmd = int.to_bytes(3)
    read_write_register = self.get_I2C().readPrimitive("u1",cmd)
    print("get_read_write_register %s >> %s" % (str(cmd), str(read_write_register)))
    return read_write_register

  def I2C_call_invokable_routine(self):
    cmd = int.to_bytes(5)
    self.get_I2C().writeByte(cmd)

  def I2C_example_for_adding_functionality(self):
    cmd = int.to_bytes(10)
    self.get_I2C().writeByte(cmd)
    returnvalue = self.get_I2C().readPrimitive("u1",cmd)
    print("example_for_adding_functionality %s >> %s" % (str(cmd), str(returnvalue)))

  def I2C_test(self):
    cmd = int.to_bytes(100)
    types_array = ["b","i1","u1","i2","u2","i4","u4","d4","d4","d4"]
    resp = self.get_I2C().readPrimitiveArray(types_array, cmd)
    print("test (%s, %s) >> %s" % (str(cmd), types_array, str(resp)))