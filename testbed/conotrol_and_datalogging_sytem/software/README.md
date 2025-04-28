# Software for the Control and Datalogging System

Since the communication uses a shared I2C bus, this functionality must be enabled on the Pi. [This guide](https://thelinuxcode.com/configure-i2c-raspberry-pi/) explains the setup process, in particular the steps of the section "Enabling I2C in Raspberry Pi Configuration" should be followed.

The directory [controlsystem_RaspberryPi](./controlsystem_RaspberryPi/) contains the python scripts that run on the Raspberry Pi:

* [i2cinterface.py](./controlsystem_RaspberryPi/i2cinterface.py) implements messaging on the shared I2C bus

* [module.py](./controlsystem_RaspberryPi/module.py) contains a generic class Module that combines the I2C interface with the software power control

* [arduinomodule.py](./controlsystem_RaspberryPi/arduinomodule.py) gives an example of how the Module class can be extended for a module based on the Arduino UNO that runs the sketch below

* [example_ArduinoUNO.py](./controlsystem_RaspberryPi/example_ArduinoUNO.py) showcases the functionalities of the Arduino UNO implementation

The directory [interface_ArduinoUNO](./interface_ArduinoUNO/) contains an Arduino sketch that implements the module-side of the functionalities provided in 'arduinomodule.py':

* [interface_ArduinoUNO.ino](./interface_ArduinoUNO/interface_ArduinoUNO.ino) contains the standard Arduino routines 'setup{}' and 'loop{}'

* [module_interface.h](./interface_ArduinoUNO/module_interface.h) and [module_interface.cpp](./interface_ArduinoUNO/module_interface.cpp) implement the interface functionalities
