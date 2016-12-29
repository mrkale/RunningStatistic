/*
  NAME:
  Basic usage of RunningStatistic library

  DESCRIPTION:
  This sketch demonstrates the use of RunningStatistic without determining
  optional arguments so that it uses the default values of them.
  * The sketch calculates just the running average from default number of
    previously calculated values.
  * As a measured value the sketch utilizes the random integers within
    the range 0 to 1023.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "RunningStatistic.h"
#define SKETCH "RunningAverage 1.4.0"

const word PERIOD_DELAY = 3000;
const word SENSOR_DATA_MIN = 0;
const word SENSOR_DATA_MAX = 1023;

word demoData, filterData;

// Create object for running averaged temperature
RunningStatistic avgRunning;

void setup()
{
  Serial.begin(9600);
  Serial.println(F(SKETCH));
  Serial.println(F(RUNNINGSTATISTIC_VERSION));
  // Print header
  Serial.print(F("Running average from items: "));
  Serial.println(avgRunning.getBufferLen());
  Serial.println(F("Data\tAverage\tItems"));
}

void loop()
{
  // New running values
  demoData = random(SENSOR_DATA_MIN, SENSOR_DATA_MAX + 1);
  filterData = avgRunning.getStatistic(demoData);
  // Show result
  Serial.print(demoData);
  Serial.print(F("\t"));
  Serial.print(filterData);
  Serial.print(F("\t"));
  Serial.println(avgRunning.getReadings());
  delay(PERIOD_DELAY);
}