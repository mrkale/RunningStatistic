/*
  NAME:
  Full usage of RunningStatistic library with sensor data filtering

  DESCRIPTION:
  This sketch demonstrates the use of RunningStatistic with determining all
  arguments that it provides with their default values.
  * The sketch calculates all running statistical types that the library provides.
  * As a measured value the sketch utilizes the random integers within
    the range 0 to 1023.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "RunningStatistic.h"
#define SKETCH "RunningAll 1.4.0"

const word PERIOD_DELAY = 3000;
const word SENSOR_DATA_MIN = 0;
const word SENSOR_DATA_MAX = 1023;
const word RUNNING_SAMPLES = 11;

word demoData;
word filterData[4];         // Array with processing objects

RunningStatistic stat[] = {
  RunningStatistic(RUNNINGSTATISTIC_MEDIAN,  SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES),
  RunningStatistic(RUNNINGSTATISTIC_AVERAGE, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES),
  RunningStatistic(RUNNINGSTATISTIC_MINIMUM, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES),
  RunningStatistic(RUNNINGSTATISTIC_MAXIMUM, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES)
};

void setup()
{
  Serial.begin(9600);
  Serial.println(F(SKETCH));
  Serial.println(F(RUNNINGSTATISTIC_VERSION));
  Serial.print(F("Running statistics from items: "));
  Serial.println(stat[0].getBufferLen());
  Serial.print(F("Valid sensor data range: "));
  Serial.print(stat[0].getValueMin());
  Serial.print(F(" ~ "));
  Serial.println(stat[0].getValueMax());
  // Print header
  Serial.println(F("Data\tMedian\tAverage\tMinimum\tMaximum"));
}

void loop()
{
  demoData = random(SENSOR_DATA_MIN, SENSOR_DATA_MAX + 1);

  // Demo data list
  Serial.print(demoData);
  Serial.print(F("\t"));
  for (byte i = 0; i < sizeof(filterData)/sizeof(filterData[0]); i++)
  {
    filterData[i] = stat[i].getStatistic(demoData);
    Serial.print(filterData[i]);
    Serial.print(F("\t"));
  }
  Serial.println();
  delay(PERIOD_DELAY);
}
