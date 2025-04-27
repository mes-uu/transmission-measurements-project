from i2cinterface import I2C_Interface
import RPi.GPIO as GPIO

class Module():
  """Generic class for modules that implements methods for power switching and I2C interactions"""

  def __init__(self, GPIO_power_pin:int, i2c_address:hex, bus_ind:int, verbose:bool=False):
    GPIO.setmode(GPIO.BOARD)
    self.GPIO_POWER_PIN = GPIO_power_pin
    GPIO.setup(self.GPIO_POWER_PIN, GPIO.OUT)
    self.setPower(False)
    self.I2C_device = I2C_Interface(bus_ind, i2c_address, verbose=verbose)

  def get_I2C(self):
    if not self.power_state:
      raise Exception("I/O Module::I2C module power is set low")
    return self.I2C_device

  def setPower(self, state:bool) -> None:
    GPIO.output(self.GPIO_POWER_PIN, GPIO.HIGH if state else GPIO.LOW)
    self.power_state = state
