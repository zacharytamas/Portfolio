#!/usr/bin/env python
# @author: Zachary Jones <zacharytamas@gmail.com>

"""
   A class for handling conversions between a proprietary 
   fixed-point number representation and standard Float 
   representation.

   USAGE:
   
   >>> from FixedPoint import AFixedPoint

   # Instantiate number as integer, assumed to be packed 32-bit value
   >>> num = AFixedPoint(0x80008000)
   >>> num = AFixedPoint(-1.0) # or create FixedPoint from a float.
   
   # By default the library assumes integers you enter are packed-format values.
   # You can turn this off by setting isPackedValue to False like so:
   >>> num = AFixedPoint(-1, False)
   
   # You can also use a string.   
   >>> num = AFixedPoint("0x80008000")                         # hexadecimal
   >>> num = AFixedPoint("0b10000000000000001000000000000000") # binary
   >>> num = AFixedPoint("020000100000")                       # octal
   
   Standard representation is to show the value as a hexadecimal packed format,
   however you can access its Float value at any time.
   
   >>> num
   FixedPoint: 0x80008000
   >>> num.asFloat
   -1.0
   >>> num.asFixedPoint
   '0x80008000'
   
   This class supports using AFixedPoint objects in native Python arithmetic. 
   New AFixedPoint instances are returned initialized with the result of 
   the arithmetic.
   
   >>> num - 2
   FixedPoint: 0x80018000  # -1.0 - 2 = -3.0
   >>> num * 2
   FixedPoint: 0x80010000  # -1.0 * 2 = -2.0
   >>> num / 2
   FixedPoint: 0x80004000  # -1.0 / 2 = -0.5
   >>> num / 2 + 2
   FixedPoint: 0x0000c000  # -1.0 / 2 + 2 = 1.5
   >>> AFixedPoint(2.0) ** 5
   FixedPoint: 0x00100000  # 2^5 = 32

   This class also includes tests for verifying the integrity of its operations. 
   You can run these tests yourself by running this file from a command line 
   instead of importing its class.

"""

import unittest

class AFixedPoint:
   """
   A class for handling conversions between a proprietary 
   fixed-point number representation and standard Float 
   representation.
   """
   fixedPointValue = None
   floatValue = None
   fractionalBits = 15
   hasSignBit = True
   integerBits = 16
   isPackedValue = True
   rawValue = None
   wordSize = 32
   
   def __init__(self, value, isPackedValue=True, integerBits=16, 
    fractionalBits=15, hasSignBit=True, wordSize=32):
      self.integerBits = integerBits
      self.fractionalBits = fractionalBits
      self.hasSignBit = hasSignBit
      self.wordSize = wordSize
      self.isPackedValue = isPackedValue
      if isinstance(value, type(42.0)): # value is a float, it cannot be fixed-point
         self.isPackedValue = False
      if isinstance(value, type("42")): # value is a string
         value = self.__ConvertFromString(value)
      self.rawValue = value
      
      if self.isPackedValue:
         self.fixedPointValue = value
      
      if not self.__IsSaneValue():
         raise TypeError("Could not create FixedPoint object, the format " +
          "specified is invalid.")
   
   @property
   def asFloat(self):
      """Returns this FixedPoint object as a Python Float."""
      if self.floatValue is not None:
         return self.floatValue
      return self.ConvertToFloat()
   
   @property
   def asFixedPoint(self):
      """Returns this FixedPoint object as a FixedPoint value."""
      if self.fixedPointValue is not None:
         value = self.fixedPointValue
      else:
         value = self.ConvertToFixedPoint()
      return ("0x%0.{0}x".format(self.wordSize / 4)) % value
   
   def ConvertToFixedPoint(self):
      """Returns a fixed point represented integer of the value."""
      
      # If the object already has a calculated fixedPointValue, 
      # just return it.
      if self.isPackedValue or self.fixedPointValue:
         return self.fixedPointValue
      
      fixedPointValue = 0
      if self.hasSignBit and self.asFloat < 0:
         fixedPointValue = 1 << (self.wordSize - 1) # move a sign bit into position
      
      floatValue = abs(self.asFloat)
      
      # Create integer part of the fixed-point format
      integerPart = int(floatValue)
      if integerPart >= (1 << self.integerBits):
         raise TypeError("FixedPoint Number is too large to fit inside format.")
      # We've assured that the integer portion of the number can fit inside 
      # our format, now let's move it into its place, leaving room for the 
      # fractional bits.
      fixedPointValue += integerPart << self.fractionalBits
      
      # Create fractional portion of the fixed-point format
      fractionalPart = floatValue - int(floatValue)
      fractionalPartValue = 0
      fractionalPlaceValue = 0.5 # first binary fractional digit is worth 1/(2^1)
      for i in range(0, self.fractionalBits):
         # move the bits over one place to prepare for insertion
         fractionalPartValue = fractionalPartValue << 1         
         
         # if the fractionalPart is greater than the value of the current 
         # fractional bit, turn the bit on and subtract its value from the 
         # fractionalPart to account for it. NOTE: Some precision may be lost 
         # if it cannot be described with the fractional bits alloted by 
         # the format
         if fractionalPart >= fractionalPlaceValue:
            fractionalPartValue += 1
            fractionalPart -= fractionalPlaceValue
         fractionalPlaceValue /= 2 # the next bit is worth half as much
      
      fixedPointValue += fractionalPartValue
      self.fixedPointValue = fixedPointValue
      return fixedPointValue
   
   def ConvertToFloat(self, value=None):
      """Returns a Float representation of the AFixedPoint object."""
      
      if not self.isPackedValue: # the value given is not a packed value
         self.floatValue = float(self.rawValue)
         return self.floatValue
      
      if self.floatValue is not None:
         return self.floatValue
      
      if value is None:
         value = self.rawValue
      
      # Create a bitmask the size of our word so that we 
      # can use it to check individual bits in the number.
      bitMask = 2 ** (self.wordSize - 1)
      
      if self.hasSignBit:
         if _bitIsOn(value, bitMask):
            isNegative = True
         else:
            isNegative = False
         bitMask = bitMask >> 1
      
      # determine integer value
      intValue = 0
      for i in range(0, self.integerBits):
         intValue *= 2 # adjust integer value to accept the next bit's value
         if _bitIsOn(value, bitMask):
            intValue += 1
         bitMask = bitMask >> 1  # shift the bit mask to point to the next bit
      
      # determine fractional value
      fractionalValue = 0.0
      fractionalPlaceValue = 0.5 # first binary place value is decimal 0.5
      for i in range(0, self.fractionalBits):
         if _bitIsOn(value, bitMask):
            fractionalValue += fractionalPlaceValue
         # the next fractional place value is half this one
         fractionalPlaceValue = fractionalPlaceValue / 2
         bitMask = bitMask >> 1

      # combine the integer value with the fractional to get a float
      retValue = intValue + fractionalValue
      
      if self.hasSignBit and isNegative:
         retValue *= -1
      
      self.floatValue = retValue
      return retValue
   
   def __ConvertFromString(self, valueString):
      """Converts a string to a numerical value."""
      
      if valueString.startswith("0b"):
         value = int(valueString, 2)
      elif valueString.startswith("0x"):
         value = int(valueString, 16)
      elif valueString.startswith("0"):
         value = int(valueString, 8)
      else:
         value = float(valueString) if '.' in valueString else int(valueString)
      return value
   
   def __unicode__(self):
      return "AFixedPoint: %s" % self.asFixedPoint
   
   def __repr__(self):
      return self.__unicode__()
   
   # Python Math bindings to allow AFixedPoint objects to be used in 
   # expressions natively. A list of Python's special methods can be found at: 
   # http://www.siafoo.net/article/57#logical-and-mathematical-operators
   
   def __cmp__(self, other):
      return cmp(self.asFloat, self.__Normalize(other))
   
   def __add__(self, other):
      return AFixedPoint(self.asFloat + self.__Normalize(other))
   __radd__ = __add__
   
   def __sub__(self, other):
      return AFixedPoint(self.asFloat - self.__Normalize(other))
   
   def __rsub__(self, other):
      return AFixedPoint(self.__Normalize(other) - self.asFloat)
   
   def __mul__(self, other):
      return AFixedPoint(self.asFloat * self.__Normalize(other))
   __rmul__ = __mul__
   
   def __div__(self, other):
      return AFixedPoint(self.asFloat / self.__Normalize(other))
   
   def __rdiv__(self, other):
      return AFixedPoint(self.__Normalize(other) / self.asFloat)
   
   def __mod__(self, other):
      return AFixedPoint(self.asFloat % self.__Normalize(other))
   
   def __rmod__(self, other):
      return AFixedPoint(self.__Normalize(other) % self.asFloat)
   
   def __pow__(self, other, mod=None):
      if mod is not None:
         return AFixedPoint(self.asFloat ** self.__Normalize(other) % mod)
      return AFixedPoint(self.asFloat ** self.__Normalize(other))
   
   def __long__(self):
      return long(self.asFloat)
   
   def __float__(self):
      return self.asFloat
   
   def __int__(self):
      return int(self.asFloat)
   
   def __neg__(self):
      return -self.asFloat
   
   def __nonzero__(self):
      return self.asFloat != 0.0
   
   def __IsSaneValue(self):
      """
      Returns Boolean whether the given number format is sane. 
      Examples of insanity include when the fractional and 
      integer bits do not add up to the full wordsize or do not 
      fit within the wordsize.
      """
      # If it's not a packed value we know it is sane because it is 
      # native Python
      if not self.isPackedValue:
         return True
      
      if self.hasSignBit:
         return (self.wordSize == (1 + self.integerBits + self.fractionalBits))
      else:
         return (self.wordSize == (self.integerBits + self.fractionalBits))
   
   def __Normalize(self, term):
      """
      Converts a term to a Float and returns it. Can normalize 
      FixedPoints to vanilla Floats for use in Python 
      expressions natively.
      """
      if isinstance(term, type(AFixedPoint)):
         return term.asFloat
      try:
         return float(term)
      except:
         return 0.0
   

def _bitIsOn(value, mask):
   """
   Given a value and bitmask returns Boolean whether the 
   bit masked is ON in the given value. Abstracted to 
   function to increase readability.
   """
   if value & mask == 0: # bit was not on
      return False
   return True # bit was on


class FixedPointTests(unittest.TestCase):
   """Tests for exercising FixedPoints"""
   
   def test_FloatInput(self):
      fp = AFixedPoint(-1.5, False)
      self.assertEqual(fp.asFixedPoint, '0x8000c000')
      
      fp = AFixedPoint(1.0, False)
      self.assertEqual(fp.asFixedPoint, '0x00008000')
   
   def test_IntegerInput(self):
      fp = AFixedPoint(-1, False)
      self.assertEqual(fp.asFixedPoint, '0x80008000')
      
      fp = AFixedPoint(1, False)
      self.assertEqual(fp.asFixedPoint, '0x00008000')
   
   def test_FixedInput(self):
      fp = AFixedPoint(0x00010000)
      self.assertEqual(fp.asFloat, 2.0)
      
      fp = AFixedPoint(0x80014000)
      self.assertEqual(fp.asFloat, -2.5)
   
   def test_StringInput(self):
      fp = AFixedPoint('0x00150000')
      self.assertEqual(fp.asFloat, 42.0)
      
      fp = AFixedPoint('42.0', False)
      self.assertEqual(fp.asFloat, 42.0)
      
      fp = AFixedPoint('42', False)
      self.assertEqual(fp.asFloat, 42)
      
      fp = AFixedPoint('0b10000000000000001000000000000000')
      self.assertEqual(fp.asFloat, -1.0)
   
   def test_IntegerOverflow(self):
      fp = AFixedPoint(50000000, False)
      with self.assertRaises(TypeError):
         fp.asFixedPoint
   
   def test_FixedPointNativeMath(self):
      fp = AFixedPoint('0x00150000')
      self.assertEqual(fp - 2, 40.0)
      self.assertEqual(2 - fp, -40.0)
      self.assertEqual(fp * 2, 84.0)
      self.assertEqual(2 * fp, 84.0)
      self.assertEqual(fp / 2, 21.0)
      self.assertEqual(84 / fp, 2)
      self.assertEqual(fp % 2, 0)
      self.assertEqual(-fp, -42.0)
      # test AFixedPoint objects can be used in equality (==) conditions
      self.assertEqual(fp, 42.0)
      self.assertEqual(fp > 0, True)
      self.assertEqual(fp < 100, True)

if __name__ == '__main__': # file was executed instead of imported
   print
   suite = unittest.TestLoader().loadTestsFromTestCase(FixedPointTests)
   unittest.TextTestRunner(verbosity=2).run(suite)
   print
