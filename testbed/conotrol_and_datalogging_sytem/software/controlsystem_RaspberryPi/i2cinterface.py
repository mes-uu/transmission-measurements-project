import smbus2
import RPi.GPIO as GPIO
import struct
import time

class I2C_Interface():
  """Implements write and read operations for the I2C bus connecting to the modules"""
  def __init__(self, bus_ind:int, i2c_address:hex, max_io_attempts:int=1, verbose:bool=False):
    self.VERBOSE = verbose
    self.BUS_IND = bus_ind
    self.I2C_ADDRESS = i2c_address
    self.MAX_IO_ATTEMPTS = max_io_attempts
    self.BUS = smbus2.SMBus(bus_ind)

  def writeIntegerAsByte(self, value:int) -> None:
    """Writes the provided integer value in the reange (0, 255) to the remote device"""
    if(0<=value<=255):
      self.writeByte(int.to_bytes(value))
    else:
      raise Exception("I/O I2C_Interface::writeIntegerAsByte value %d not in range (0,255)" % value)

  def writeByte(self, write_byte:bytes) -> None:
    """Write a byte to the remote device"""
    if self.VERBOSE:
      print("I/O write " + str(write_byte))
    write_int = int.from_bytes(write_byte)
    for i in range(self.MAX_IO_ATTEMPTS):
      try:
        self.BUS.write_byte(self.I2C_ADDRESS, write_int)
        return
      except:
        time.sleep(0.1)
    raise Exception("I/O I2C_Interface::writeByte failed")

  def readPrimitive(self, type:str, request_byte:bytes=b'\x00') -> any:
    """Reads the value of one primitive variable by invoking readPrimitiveArray"""
    return self.readPrimitiveArray([type], request_byte)[0]

  def readPrimitiveArray(self, types_array:list, request_byte:bytes=b'\x00') -> list:
    """Reads the values of multiple primitive variables from the remote device

    Parameters
    ----------
    types_array: list
      Specifies the sequence and types of values to be read from the device. The valid format for typestrings is defined in the documentation of the parameter 'format_str' for the method 'parsePrimitiveFormat'
    request_byte: bytes
      This byte will be written together with the read request to the I2C bus for context

    Returns
    -------
    list
      A list containing the parsed read data, according to 'types_array'
    """
    c_format = []
    n_bytes = []
    n_bytes_total = 0
    i_start = [0]
    for type in types_array:
      cf, nb = self.parsePrimitiveFormat(type)
      c_format += [cf]
      n_bytes += [nb]
      i_start += [i_start[-1]+nb]
      n_bytes_total += nb

    if self.VERBOSE:
      print("I/O read format %s n_bytes %d" % (str(c_format), n_bytes_total))
    bytestream = self.readBytestream(n_bytes_total, request_byte)

    return [struct.unpack(c_format[i], bytestream[i_start[i]:i_start[i]+n_bytes[i]])[0] for i in range(len(types_array))]

  def readByte(self, request_byte:bytes=b'\x00') -> bytes:
    """Read a byte from the remote device"""
    return self.readBytestream(1, request_byte)

  def readBytestream(self, n_bytes_read:int, request_byte:bytes=b'\x00'):
    """Requests and reads a variable number of bytes from the remote device"""
    if self.VERBOSE:
      print("I/O read request %d %s" % (n_bytes_read, str(request_byte)))
    request_int = int.from_bytes(request_byte)
    read_list_int = []
    read_success = False
    for i in range(self.MAX_IO_ATTEMPTS):
      try:
        read_list_int = self.BUS.read_i2c_block_data(self.I2C_ADDRESS, request_int, n_bytes_read)
        read_success = True
      except:
        time.sleep(0.1)
    if not read_success:
      raise Exception("I/O I2C_Interface::readBytestream failed")
    read_bytestream = b''.join([d.to_bytes() for d in read_list_int])
    if self.VERBOSE:
      print("I/O read response " + str(read_bytestream))
    return read_bytestream

  def parsePrimitiveFormat(self, format_str:str):
    """Translates typestrings into c-type format strings

    The method supports the following primitive types in big-endian and little-endian notation
    - boolean as binary8
    - integer singed and unsigned formats for binary8, binary16, binary32, binary64
    - floating-point IEEE 754 formats for binary16, binary32, binary64

    Parameters
    ----------
    format_str:str
       The format starts with an optional character '<' or '>' indicating the endianness, followed by a character 'b'oolean, 'i'nteger, or 'd'ecimal declaring the type, and ending with a character '1','2','4', or '8' for the size of the data in bytes. The only exception to this format is the format string "b" for booleans

    Returns
    -------
    str
      The c-type format string
    """
    dic_formats = {
      'b' : '?',
      'i1':'b','i2':'h','i4':'l','i8':'q',
      'u1':'B','u2':'H','u4':'L','u8':'Q',
      'd2':'e','d4':'f','d8':'d'
    }
    list_bitorders = ['>', '<']

    try:
      includes_bitorder = format_str[0] in list_bitorders
      bitorder = '>' if not includes_bitorder else format_str[0]
      format_str = format_str if not includes_bitorder else format_str[1:]
      n_bytes = int(format_str[1]) if not format_str[0]=='b' else 1
      c_format = bitorder + dic_formats[format_str]
      return c_format, n_bytes
    except:
      raise Exception("I/O I2C_Interface::parsePrimitiveFormat format not recognized or supported '%s'" % format_str)