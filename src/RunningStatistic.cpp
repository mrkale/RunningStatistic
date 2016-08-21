#include "RunningStatistic.h"

RunningStatistic::RunningStatistic(uint8_t runningType, uint16_t valueMax, uint16_t valueMin, uint8_t bufferLen)
{
  _runningType = runningType;
  _bufferLen = bufferLen;
  switch(_runningType) {
    case RUNNINGSTATISTIC_AVERAGE:
    case RUNNINGSTATISTIC_MINIMUM:
    case RUNNINGSTATISTIC_MAXIMUM:
      break;
    case RUNNINGSTATISTIC_MEDIAN:
      _bufferLen = _bufferLen | 1;
      break;
    default:
      _runningType = RUNNINGSTATISTIC_AVERAGE;
  }
  // Sanitize arguments
  _valueMax  = constrain(valueMax, RUNNINGSTATISTIC_MIN, RUNNINGSTATISTIC_MAX);
  _valueMin  = constrain(valueMin, RUNNINGSTATISTIC_MIN, RUNNINGSTATISTIC_MAX);
  swap(_valueMin, _valueMax); // Sort valid range values
  _bufferLen = constrain(_bufferLen, RUNNINGSTATISTIC_BUFFER_MIN, RUNNINGSTATISTIC_BUFFER_MAX);
  init();
}

// Initialize all status variables
void RunningStatistic::init()
{
  _bufferCnt = 0;
}

/* Register data item into the buffer and return running value of the statistic.
 * The most recent (fresh) statistic is always in the 0 index of the buffer.
 */
uint16_t RunningStatistic::getStatistic(uint16_t currentValue)
{
  uint16_t statistic = RUNNINGSTATISTIC_NAN;

  // Test value against valid range
  if (currentValue < _valueMin) return statistic;
  if (currentValue > _valueMax) return statistic;

  // Statistical running calculation processing
  shiftRight(); // Shift buffer for current value and increase _bufferCnt
  _buffer[0] = currentValue;
  switch(_runningType) {

    case RUNNINGSTATISTIC_MEDIAN:
      for (uint8_t i = 0; i < _bufferCnt; i++) _sorter[i] = _buffer[i];
      sort();
      // Round down median index
      statistic = _sorter[(_bufferCnt - 1)/2];
      break;

    case RUNNINGSTATISTIC_AVERAGE:
      statistic = 0;
      for (uint8_t i = 0; i < _bufferCnt; i++) statistic += _buffer[i];
      // Round up arithmetic mean
      statistic = divide(statistic, _bufferCnt);
      break;

    case RUNNINGSTATISTIC_MINIMUM:
      statistic = _buffer[0];
      for (uint8_t i = 1; i < _bufferCnt; i++) statistic = min(statistic, _buffer[i]);
      break;

    case RUNNINGSTATISTIC_MAXIMUM:
      statistic = _buffer[0];
      for (uint8_t i = 1; i < _bufferCnt; i++) statistic = max(statistic, _buffer[i]);
      break;
  }
  return statistic;
}

//-------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------
uint8_t  RunningStatistic::getBufferLen()   { return _bufferLen; };
uint8_t  RunningStatistic::getReadings()    { return _bufferCnt; };
uint8_t  RunningStatistic::getRunningType() { return _runningType; };
uint16_t RunningStatistic::getValueMin()    { return _valueMin; };
uint16_t RunningStatistic::getValueMax()    { return _valueMax; };

//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------

// Sort array by bubblesort algorithm in ascending order
void RunningStatistic::sort()
{
  bool again = true;
  for(byte i = 0; i < _bufferCnt-1 && again; i++) {
    for(byte j = _bufferCnt-1, again = false; j > i; --j) {
      if(_sorter[j] < _sorter[j-1]) {
        uint16_t t = _sorter[j];
        _sorter[j] = _sorter[j-1];
        _sorter[j-1] = t;
        again = true;
      }
    }
  }
}

// Shift array to the right so that 0 index is reserved for the new value
void RunningStatistic::shiftRight()
{
  for(byte i = _bufferCnt; i > 0 ; --i) {
    // Forget the oldest (most right) value in the buffer if it is full
    if (i < _bufferLen) _buffer[i] = _buffer[i-1];
  }
  // Count the freed 0 indexed value. Normally the buffer is full.
  _bufferCnt++;
  _bufferCnt = constrain(_bufferCnt, 0, _bufferLen);
}