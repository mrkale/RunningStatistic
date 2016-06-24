<a id="library"></a>
# RunningStatistic
The library provides calculation of a running statistic value from previously calculated and stored ones in 16-bit resolution, i.e., median, average, minimum, and maximum. The library is usable for Arduino as well as for Particle Photon (and probably Electron and P1 as well).


<a id="credit"></a>
## Credit
The implementation has been inspired by the library *RunningMedian* from *Rob dot Tillaart*.


<a id="dependency"></a>
## Dependency
The library class includes following system header files for particular platform (Arduino/Particle) according to corresponding preprocessor constant.
##### Arduino
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.

##### Particle
- **Particle.h**: Core artifacts for Particle WebIDE. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="constants"></a>
## Constants
- **RUNNINGSTATISTIC\_VERSION**: Semantic library version
- **RUNNINGSTATISTIC\_BUFFER\_MIN**: Minimal number of values for running statistic
- **RUNNINGSTATISTIC\_BUFFER\_MAX**: Maximal number of values for running statistic
- **RUNNINGSTATISTIC\_BUFFER\_DEF**: Default number of values for running statistic
- **RUNNINGSTATISTIC\_BUFFER\_NAN**: Represents undefined number of values for running statistic


- **RUNNINGSTATISTIC\_MEDIAN**: Median (50% percetile) as a running statistic
- **RUNNINGSTATISTIC\_AVERAGE**: Average (arithetic mean) as a running statistic
- **RUNNINGSTATISTIC\_MINIMUM**: Minimum as a running statistic
- **RUNNINGSTATISTIC\_MAXIMUM**: Maximum as a running statistic

 
<a id="interface"></a>
## Interface
- [RunningStatistic()](#RunningStatistic)
- [init()](#init)
- [getStatistic()](#getStatistic)
- [getBufferLen()](#getBufferLen)
- [getReadings()](#getReadings)


<a id="RunningStatistic"></a>
## RunningStatistic()
#### Description
Constructor creates the data buffer within a class instance object, which holds a serie of running values of particular statistical type.

- For each type of running statistic a separate instance object has to be created with separated data buffer length, if the default value is not suitable.
- Running values of sensor readings or smoothed sensor readings reduce excesses and extremes in the physical value measurement.
- The class may be used for original digital data as well, where the running statistical processing is desirable.

#### Syntax
    RunningStatistic(uint8_t runningType, uint8_t bufferLen);

#### Parameters
<a id="runningType"></a>
- **runningType**: Type of the running statistic to be calculated.
  - It has to be one of statistic types defined by macros.
  - *Valid values*: enumerated value from RUNNINGSTATISTIC_MEDIAN, RUNNINGSTATISTIC_AVERAGE, RUNNINGSTATISTIC_MINIMUM, RUNNINGSTATISTIC_MAXIMUM
  - *Default value*: RUNNINGSTATISTIC_AVERAGE


<a id="prm_bufferLen"></a>
- **bufferLen**: Number of running statistics held in the data buffer used for calculating a new running statistic.
  - The constructor limits the input buffer length to allowed range.
  - For median the buffer length should be odd number. If it is not, the constructor adds 1 to it right before limiting the length in order to make it odd.
  - For extreme statistics (minimum, maximum) the library always uses 1 for the data buffer length regardless of the input or default value.
  - *Valid values*: positive integer in the range 3 to 11
  - *Default value*: 5

#### Returns
Object perfoming a running statistic calculation.

#### Example

``` cpp
avg = RunningStatistic();                           // Only default arguments - running average
mdn = RunningStatistic(RUNNINGSTATISTIC_MEDIAN, 7); // Running median with custom data buffer
max = RunningStatistic(RUNNINGSTATISTIC_MAXIMUM);   // Running maximum with default data buffer
```

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
  - *Valid values*: positive integer in the range 0 ~ 65535
  - *Default value*: none

#### Returns
Running statistic.

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
