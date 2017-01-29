/*
  NAME:
  Basic usage of RunningStatistic library.

  DESCRIPTION:
  This sketch demonstrates the use of RunningStatistic without determining
  optional arguments so that it uses the default values of them.
  * The sketch can be run either on Arduino platform or Particle platform
    (Photon, Electron, P1).
  * As a measured value the sketch utilizes the random integers within
    the range 0 to 1023.
  * The sketch calculates just the running average from default number of
    previously calculated values.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "running-statistic.h"
#define SKETCH "RUNNING-AVERAGE 1.0.0"

const unsigned int PERIOD_MEASURE = 3000;      // Time in miliseconds between measurements

// Upper limit of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MAX = 1023;

// Variables and constants for measurement
unsigned int demoData, filterData;

// Create object for running averaged temperature
RunningStatistic avgRunning;

void setup()
{
#if defined(ARDUINO)
  Serial.begin(9600);
  Serial.println(F(SKETCH));
  Serial.println(F(RUNNINGSTATISTIC_VERSION));
  // Print header
  Serial.print(F("Running average from items: "));
  Serial.println(avgRunning.getBufferLen());
  Serial.println(F("Data\tAverage\tItems"));
#elif defined(PARTICLE)
  Particle.publish("Sketch",  String(SKETCH));
  Particle.publish("Library", String(RUNNINGSTATISTIC_VERSION));
  Particle.publish("Average Readings", String(avgRunning.getBufferLen()));
#endif
}

void loop()
{
  processing();
}

void processing()
{
  static unsigned long timeStamp;
  if (millis() - timeStamp >= PERIOD_MEASURE || timeStamp == 0)
  {
    timeStamp = millis();
    // New running value
    demoData = random(SENSOR_DATA_MAX + 1);
    filterData = avgRunning.getStatistic(demoData);
    // Publish result
#if defined(ARDUINO)
    Serial.print(demoData);
    Serial.print(F("\t"));
    Serial.print(filterData);
    Serial.print(F("\t"));
    Serial.print(avgRunning.getReadings());
    Serial.println();
#elif defined(PARTICLE)
    Particle.publish("Data/Average", String::format("%4d/%4d", demoData, filterData));
#endif
  }
}
