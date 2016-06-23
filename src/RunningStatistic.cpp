#include <RunningStatistic.h>

RunningStatistic::RunningStatistic(uint8_t runningType, uint8_t bufferLen)
{
  _runningType = runningType;
  _bufferLen = (bufferLen == RUNNINGSTATISTIC_BUFFER_NAN ? RUNNINGSTATISTIC_BUFFER_DEF : bufferLen);
  switch(runningType) {

    case RUNNINGSTATISTIC_MINIMUM:
    case RUNNINGSTATISTIC_MAXIMUM:
      _bufferLen = 1;
      break;

    case RUNNINGSTATISTIC_MEDIAN:
      _bufferLen = _bufferLen | 1;
      break;

    case RUNNINGSTATISTIC_AVERAGE:
    default:
      _runningType = RUNNINGSTATISTIC_AVERAGE;
      break;
  }
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
  uint16_t statistic;

  // Shift buffer, _bufferCnt set minimal to 1
  shiftRight();

  // Statistical running calculation processing
  switch(_runningType) {

    case RUNNINGSTATISTIC_MEDIAN:
      _buffer[0] = currentValue;
      for (uint8_t i = 0; i < _bufferCnt; i++) _sorter[i] = _buffer[i];
      sort();
      statistic = _sorter[_bufferCnt/2];
      break;

    case RUNNINGSTATISTIC_MINIMUM:
      statistic = (currentValue < _buffer[0] ? currentValue: _buffer[0]);
      break;

    case RUNNINGSTATISTIC_MAXIMUM:
      statistic = (currentValue > _buffer[0] ? currentValue: _buffer[0]);
      break;

    case RUNNINGSTATISTIC_AVERAGE:
      statistic = currentValue;
      for (uint8_t i = 1; i < _bufferCnt; i++) statistic += _buffer[i];
      statistic /= _bufferCnt;
      break;
  }
  return (_buffer[0] = statistic);  // Store new running value
}

// Getters
uint8_t RunningStatistic::getBufferLen() { return _bufferLen; };
uint8_t RunningStatistic::getReadings() { return _bufferCnt; };

// Sort array by bubblesort algorithm in ascending order
void RunningStatistic::sort()
{
  bool again = true;
  for(byte i = 0; i < _bufferCnt-1 && again; i++) {
    for(byte j = _bufferCnt-1, again = false; j > i; --j) {
      if(_sorter[j] < _sorter[j-1]) {
        uint16_t t = _sorter[j];
        _sorter[j] = _sorter[j-1];
        _sorter[j+1] = t;
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
  _bufferCnt = constrain(++_bufferCnt, 0, _bufferLen);
}