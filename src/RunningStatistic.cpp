#include <RunningStatistic.h>

RunningStatistic::RunningStatistic(uint8_t runningType, uint8_t bufferLen)
{
  _runningType = runningType;
  _bufferLen = (bufferLen == RUNNINGSTATISTIC_BUFFER_NAN ? RUNNINGSTATISTIC_BUFFER_DEF : bufferLen);
  // Set buffer length
  switch(_runningType) {

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
  // Limit buffer length
  switch(_runningType) {
    case RUNNINGSTATISTIC_MEDIAN:
    case RUNNINGSTATISTIC_AVERAGE:
      _bufferLen = constrain(_bufferLen, RUNNINGSTATISTIC_BUFFER_MIN, RUNNINGSTATISTIC_BUFFER_MAX);
      break;
  }
  init();
}

// Initialize all status variables
void RunningStatistic::init()
{
  _bufferCnt = 0;
  switch(_runningType) {

    case RUNNINGSTATISTIC_MINIMUM:
      _buffer[0] = -1;  // Maximal possible value
      break;

      case RUNNINGSTATISTIC_MAXIMUM:
      _buffer[0] = 0;  // Minimal possible value
      break;
  }
}

/* Register data item into the buffer and return running value of the statistic.
 * The most recent (fresh) statistic is always in the 0 index of the buffer.
 */
uint16_t RunningStatistic::getStatistic(uint16_t currentValue)
{
  uint16_t statistic;

  // Statistical running calculation processing
  shiftRight(); // Shift buffer and set _bufferCnt to 1 at least
  switch(_runningType) {

    case RUNNINGSTATISTIC_MEDIAN:
      _buffer[0] = currentValue;
      for (uint8_t i = 0; i < _bufferCnt; i++) _sorter[i] = _buffer[i];
      sort();
      // Round down median index
      statistic = _sorter[(_bufferCnt - 1)/2];
      break;

    case RUNNINGSTATISTIC_AVERAGE:
      _buffer[0] = currentValue;
      statistic = 0;
      for (uint8_t i = 0; i < _bufferCnt; i++) statistic += _buffer[i];
      // Round up arithmetic mean
      statistic = (statistic + _bufferCnt - 1) / _bufferCnt;
      break;

    case RUNNINGSTATISTIC_MINIMUM:
      statistic = currentValue;
      if (currentValue < _buffer[0]) {
        _buffer[0] = currentValue; // Store new minimum
      } else {
        statistic = _buffer[0]; // Return old minimum
      }
      break;

    case RUNNINGSTATISTIC_MAXIMUM:
      statistic = currentValue;
      if (currentValue > _buffer[0]) {
        _buffer[0] = currentValue; // Store new maximum
      } else {
        statistic = _buffer[0]; // Return old maximum
      }
      break;
  }
  return statistic;
}

// Getters
uint8_t RunningStatistic::getBufferLen() { return _bufferLen; };
uint8_t RunningStatistic::getReadings()  { return _bufferCnt; };

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