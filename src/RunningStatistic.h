/*
  NAME:
  RunningStatistic

  DESCRIPTION:
  The library provides calculation of a running statistic value from
  recent sensor readings including the current one that are stored in the
  library instance object in 16-bit resolution.
  - The library calculates running median, average, minimum, and maximum
    as a statistic.
  - One library instance object can calculate running values just for one
    statistic, e.g., median or average, etc.
  - A running value substitutes the current sensor value.
  - As a current sensor data may be used a smoothed sensor value calculated
    by the library SmoothSensorData.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.
    
  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/RunningStatistic.git
 */
#ifndef RUNNINGSTATISTIC_H
#define RUNNINGSTATISTIC_H
#define RUNNINGSTATISTIC_VERSION "RunningStatistic 2.0.1"

#ifdef ARDUINO_ARCH_AVR
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  #include <inttypes.h>
#endif

// RunningStatistic limits
#define RUNNINGSTATISTIC_NAN 65535        // Undefined sensor data
#define RUNNINGSTATISTIC_MIN 0            // Minimal valid sensor data
#define RUNNINGSTATISTIC_MAX 65535        // Maximal valid sensor data
#define RUNNINGSTATISTIC_BUFFER_MIN 3     // Minimal number of values for running statistics
#define RUNNINGSTATISTIC_BUFFER_MAX 11    // Maximal number of values for running statistics
#define RUNNINGSTATISTIC_BUFFER_DEF 5     // Default number of values for running statistics

// RunningStatistic types
#define RUNNINGSTATISTIC_MEDIAN   1        // Median as a running statistic
#define RUNNINGSTATISTIC_AVERAGE  2        // Arithmetic mean as a running statistic
#define RUNNINGSTATISTIC_MINIMUM  4        // Minimum as a running statistic
#define RUNNINGSTATISTIC_MAXIMUM  8        // Maximum as a running statistic

// Macro functions
#define swap(a, b) { if (a > b){int16_t t = a; a = b; b = t;} }
#define divide(a, b) { (unsigned int)round((float)a/(float)b) }

class RunningStatistic
{
public:
//-------------------------------------------------------------------------
// Public methods
//-------------------------------------------------------------------------

/*
  Constructor
  
  DESCRIPTION:
  Constructor creates the data buffer within a class instance object, which
  holds a serie of running values of particular statistical type.
  - For each type of running statistic as well as each statistical variable
    (sensor) a separate instance object has to be created with separated
    data buffer.
  - Running values of sensor readings or smoothed sensor readings reduce
    excesses and extremes in the physical value measurement.
  - The class may be used for original digital data as well, where
    the running statistical processing is desirable.
  - The constructor has all arguments defaulted. If some argument after
    some defaulted arguments should have a specific value, use corresponding
    constants in place of those defaulted arguments.

  PARAMETERS:
  runningType - type of the running statistic to be calculated.
                - It has to be one of statistic types defined by macros.
                - Data type: positive integer
                - Default value: average type
                - Limited range: enumerated types for median, average,
                  minimum, maximum
  
  valueMax    - maximal valid sensor value for calculating.
                - Data type: non-negative integer
                - Default value: 65535
                - Limited range: 0 ~ 65535

  valueMin    - minimal valid sensor value for calculating.
                - Data type: non-negative integer
                - Default value: 0
                - Limited range: 0 ~ 65535

  bufferLen   - number of running statistics held in the data buffer used
                for calculating a new running statistic.
                - For median the buffer length should be odd number. If it
                  is not, the constructor adds 1 to it right before limiting
                  the length in order to make it odd.
                - The constructor limits the input buffer length to allowed range.
                - Data type: positive integer
                - Default value: by statistic type, see macros
                - Limited range: 1 ~ 11

  RETURN: object
*/ 
  RunningStatistic(uint8_t runningType = RUNNINGSTATISTIC_AVERAGE, \
                   uint16_t valueMax = RUNNINGSTATISTIC_MAX, \
                   uint16_t valueMin = RUNNINGSTATISTIC_MIN, \
                   uint8_t bufferLen = RUNNINGSTATISTIC_BUFFER_DEF);
  
/*
  Reset all counters and status flags
  
  DESCRIPTION:
  The method initiate all internal counters and status flags of a class
  instance object to default values as they are right after power up of
  a microcontroler.
  
  PARAMETERS: none

  RETURN: none
*/
  void init();

/*
  Calculate running statistic for current sensor value
  
  DESCRIPTION:
  The method calculates and returns a new running statistic from current
  input value and previous running statistics and stores it in the data
  buffer for the future calculation.
  - The method accepts only values within the valid range defined in the
    constructor.
  - Because every statistical variable (sensor) and its statistical type has
    to have dedicated class instance, it is unreasonable to dynamicly change
    the valid range for each sensor value.
  
  PARAMETERS:
  currentValue  - 16-bit value to be used for calculating a new running statistic.
                  - Data type: non-negative integer
                  - Default value: none
                  - Limited range: 0 ~ 65535

  RETURN: running statistic
*/ 
  uint16_t getStatistic(uint16_t currentValue);

//-------------------------------------------------------------------------
// Public getters
//-------------------------------------------------------------------------
  
/*
  Getter of the actual data buffer length
  
  DESCRIPTION:
  The method returns current length of the data buffer used for calculation.
  - Usually the returned value is the same as length put to the constructor.
  - If class has adjusted or limited the input buffer length, the method
    returns the actual length.
  - The method is useful if the length has been put to the constructor as
    a numeric literal and there is no variable of the length to use it in
    other statements.
  
  PARAMETERS: none

  RETURN: Actual length of the data buffer
*/ 
  uint8_t getBufferLen();
  
/*
  Getter of the actual number of running values
  
  DESCRIPTION:
  The method returns number of running values of a statistic in the
  data buffer, which are going to be used for calculating a new one.
  - The calculation of a new running value can be provided before
    the data buffer is full. In that case the method returns the
    number less than the length of the data buffer set in the constructor.
  - After a while after initialization the returned value is same as the length of the data buffer.
  
  PARAMETERS: none

  RETURN: Actual number of running values of a statistic in the data buffer
*/ 
  uint8_t getReadings();
  
/*
  Getter of the actual running statistic type
  
  DESCRIPTION:
  The method returns internal number code of running statistic type for
  the current instance object.
  - Returned code is the value of one class constant for running statistic types.
  
  PARAMETERS: none

  RETURN: One of the constants 
          - RUNNINGSTATISTIC_MEDIAN
          - RUNNINGSTATISTIC_AVERAGE
          - RUNNINGSTATISTIC_MINIMUM
          - RUNNINGSTATISTIC_MAXIMUM
*/ 
  uint8_t getRunningType();

/*
  Getters of the actual minimal and maximal valid sensor value

  DESCRIPTION:
  The method returns currently set minimal resp. maximal value valid for
  registering.

  PARAMETERS: none

  RETURN: Actual Minimal resp. maximal valid value
*/
  uint16_t getValueMin();
  uint16_t getValueMax();
  
private:
//-------------------------------------------------------------------------
// Private attributes
//-------------------------------------------------------------------------
  uint16_t _buffer[RUNNINGSTATISTIC_BUFFER_MAX];   // Data buffer
  uint16_t _sorter[RUNNINGSTATISTIC_BUFFER_MAX];   // Buffer for sorting
  uint16_t _valueMin = RUNNINGSTATISTIC_MIN;       // Minimal valid value
  uint16_t _valueMax = RUNNINGSTATISTIC_MAX;       // Maximal valid value
  uint8_t _bufferLen;   // Data buffer length in data items
  uint8_t _bufferCnt;   // Current number of data items in buffer
  uint8_t _runningType; // Running statistical type
    
//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------
  void sort();          // Sort values in the data buffer in ascending order
  void shiftRight();    // Shift data buffer to the right
};

#endif
