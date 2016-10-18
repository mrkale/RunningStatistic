<a id="library"></a>
# RunningStatistic
The library provides calculation of a running statistic value from recent sensor readings including the current one that are stored in the library instance object in 16-bit resolution, i.e., median, average, minimum, and maximum. It is valuable to combine usage this library with the library *SmoothSensorData*, which should be a value source for the *RunningStatistic* library.


<a id="dependency"></a>
## Dependency
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="Constants"></a>
## Constants
- **RUNNINGSTATISTIC\_VERSION**: Name and semantic library version
- **RUNNINGSTATISTIC\_NAN**: Represents undefined sensor data
- **RUNNINGSTATISTIC\_MIN**: Minimal value for sensor data
- **RUNNINGSTATISTIC\_MAX**: Maximal value for sensor data


- **RUNNINGSTATISTIC\_BUFFER\_MIN**: Minimal number of values for running statistic
- **RUNNINGSTATISTIC\_BUFFER\_MAX**: Maximal number of values for running statistic
- **RUNNINGSTATISTIC\_BUFFER\_DEF**: Default number of values for running statistic


- **RUNNINGSTATISTIC\_MEDIAN**: Median (50% percetile) as a running statistic
- **RUNNINGSTATISTIC\_AVERAGE**: Average (arithetic mean) as a running statistic
- **RUNNINGSTATISTIC\_MINIMUM**: Minimum as a running statistic
- **RUNNINGSTATISTIC\_MAXIMUM**: Maximum as a running statistic

 
<a id="interface"></a>
## Interface
- [RunningStatistic()](#RunningStatistic)
- [init()](#init)
- [getStatistic()](#getStatistic)

#### Getters
- [getRunningType()](#getRunningType)
- [getValueMin()](#getValueRange)
- [getValueMax()](#getValueRange)
- [getBufferLen()](#getBufferLen)
- [getReadings()](#getReadings)


<a id="RunningStatistic"></a>
## RunningStatistic()
#### Description
Constructor creates the data buffer within a class instance object, which holds a serie of running values of particular statistical type.
- For each type of running statistic as well as each statistical variable (sensor) a separate instance object has to be created with separated data buffer.
- Running values of sensor readings or smoothed sensor readings reduce excesses and extremes in the physical value measurement.
- The class may be used for original digital data as well, where the running statistical processing is desirable.

#### Syntax
    RunningStatistic(uint8_t runningType, uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen);

#### Parameters
<a id="prm_runningType"></a>
- **runningType**: Type of the running statistic to be calculated.
  - It has to be one of statistic types defined by macros.
  - *Valid values*: enumerated value from [RUNNINGSTATISTIC\_MEDIAN, RUNNINGSTATISTIC\_AVERAGE, RUNNINGSTATISTIC\_MINIMUM, RUNNINGSTATISTIC\_MAXIMUM](#Constants)
  - *Default value*: [RUNNINGSTATISTIC\_AVERAGE](#Constants)


<a id="prm_valueMax"></a>
- **valueMax**: Maximal valid sensor value for registering.
  - *Valid values*: non-negative integer 0 to 65365 ([RUNNINGSTATISTIC\_MIN ~ RUNNINGSTATISTIC\_MAX](#Constants))
  - *Default value*: 65365 ([RUNNINGSTATISTIC\_MAX](#Constants))


<a id="prm_valueMin"></a>
- **valueMin**: Minimal valid sensor value for registering.
  - *Valid values*: non-negative integer 0 to 65365 ([RUNNINGSTATISTIC\_MIN ~ RUNNINGSTATISTIC\_MAX](#Constants))
  - *Default value*: 0 ([RUNNINGSTATISTIC\_MIN](#Constants))


<a id="prm_bufferLen"></a>
- **bufferLen**: Number of running statistics held in the data buffer used for calculating a new running statistic.
  - The constructor limits the input buffer length to allowed range.
  - For median the buffer length should be odd number. If it is not, the constructor adds 1 to it right before limiting the length in order to make it odd.
  - *Valid values*: positive integer in the range 3 to 11 ([RUNNINGSTATISTIC\_BUFFER\_MIN ~ RUNNINGSTATISTIC\_BUFFER\_MAX](#Constants))
  - *Default value*: 5 ([RUNNINGSTATISTIC\_BUFFER\_DEF](#Constants))

#### Returns
Object perfoming a running statistic calculation.

#### Example
The constructor has all arguments defaulted. The constructor instance without any parameters is equivalent to an instance with all arguments set by corresponding constant with default value:

``` cpp
RunningStatistic runningAvg();               // It is equivalent to
RunningStatistic runningAvg(RUNNINGSTATISTIC_AVERAGE, RUNNINGSTATISTIC_MAX, RUNNINGSTATISTIC_MIN, RUNNINGSTATISTIC_BUFFER_DEF);
```

If some argument after some defaulted arguments should have a specific value, use corresponding constants in place of those defaulted arguments, e.g.

``` cpp
RunningStatistic runningAvg(RUNNINGSTATISTIC_AVERAGE, RUNNINGSTATISTIC_MAX, RUNNINGSTATISTIC_MIN, 11); // Specific buffer length
```

Typical usage is just with setting a desired statistical type in the constructor:

``` cpp
RunningStatistic runningAvg();                                    // Only default arguments - running average
RunningStatistic runningMed(RUNNINGSTATISTIC_MEDIAN, 1023);       // Running median with maximal value filtering
RunningStatistic runningMax(RUNNINGSTATISTIC_MAXIMUM, 1023, 16);  // Running maximum with valid value range filtering
```

#### See also
[Back to interface](#interface)


<a id="init"></a>
## init()
#### Description
The method initiate all internal counters and status flags of a class instance object to default values as they are right after power up of a microcontroler.

#### Syntax
    void init();

#### Parameters
None

#### Returns
None

#### See also
[RunningStatistic()](#RunningStatistic)

[Back to interface](#interface)


<a id="getStatistic"></a>
## getStatistic()
#### Description
The method calculates and returns a new running statistic of a type defined in [constuctor](#RunningStatistic) from the current input value and previous running statistics and stores it in the data buffer for the future calculation.

#### Syntax
    uint16_t getStatistic(uint16_t currentValue);

#### Parameters
- **currentValue**: 16-bit value to be used for calculating a new running statistic.
  - *Valid values*: non-negative integer in the range 0 ~ 65535
  - *Default value*: none

#### Returns
Running statistic or [RUNNINGSTATISTIC\_NAN](#Constants) class constant representing undefined value, if the input value is outside the valid range.

#### See also
[RunningStatistic()](#RunningStatistic)

[Back to interface](#interface)


<a id="getRunningType"></a>
## getRunningType()
#### Description
The method returns internal number code (defined by corresponding constant) of running statistic type for the current instance object.
  - Returned code is the value of one class constant for running statistic types.

The method returns number of running values of a statistic in the data buffer, which are going to be used for calculating a new one.
- The calculation of a new running value can be provided before the data buffer is full. In that case the method returns the number less than the length of the data buffer set in the [constructor](#RunningStatistic).
- After a while after initialization the returned value is same as the length of the data buffer.

#### Syntax
    uint8_t getRunningType();

#### Parameters
None

#### Returns
Code of actual running statistic type defined by one constant from the list [RUNNINGSTATISTIC\_MEDIAN, RUNNINGSTATISTIC\_AVERAGE, RUNNINGSTATISTIC\_MINIMUM, RUNNINGSTATISTIC\_MAXIMUM](#Constants).

#### See also
[RunningStatistic()](#RunningStatistic)

[Back to interface](#interface)


<a id="getValueRange"></a>
## getValueMax(), getValueMin()
#### Description
The methods return set minimal or maximal value valid for calculating respectively.

#### Syntax
    uint16_t getValueMax();
    uint16_t getValueMin();

#### Parameters
None

#### Returns
Actual minimal and maximal value of the valid data range.

#### See also
[RunningStatistic()](#RunningStatistic)

[Back to interface](#interface)


<a id="getBufferLen"></a>
## getBufferLen()
#### Description
The method returns current length of the data buffer used for calculation.
- Usually the returned value is the same as length put to the [constructor](#RunningStatistic) as parameter [bufferLen](#prm_bufferLen).
- For extreme statistics (minimum, maximum) it is always 1.
- If class has adjusted or limited the input buffer length, the method returns the actual length.
- The method is useful if the length has been put to the constructor as a numeric literal and there is no variable of the length to use it in other statements.

#### Syntax
    uint8_t getBufferLen();

#### Parameters
None

#### Returns
Actual length of the data buffer.

#### See also
[RunningStatistic()](#RunningStatistic)

[getReadings()](#getReadings)

[Back to interface](#interface)


<a id="getReadings"></a>
## getReadings()
#### Description
The method returns number of running values of a statistic in the data buffer, which are going to be used for calculating a new one.
- The calculation of a new running value can be provided before the data buffer is full. In that case the method returns the number less than the length of the data buffer set in the [constructor](#RunningStatistic).
- After a while after initialization the returned value is same as the length of the data buffer.

#### Syntax
    uint8_t getReadings();

#### Parameters
None

#### Returns
Actual number of running values of a statistic in the data buffer.

#### See also
[RunningStatistic()](#RunningStatistic)

[getBufferLen()](#getBufferLen)

[Back to interface](#interface)
