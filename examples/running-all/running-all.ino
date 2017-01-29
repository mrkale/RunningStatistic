/*
  NAME:
  Usage of RunningStatistic library with all statistics.

  DESCRIPTION:
  This sketch demonstrates the use of RunningStatistic with determining all
  arguments that it provides.
  * The sketch can be run either on Arduino platform or Particle platform
    (Photon, Electron, P1).
  * As a measured value the sketch utilizes the random integers within
    the range 0 to 1023.
  * The sketch calculates all running statistical types that the library provides.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "running-statistic.h"
#define SKETCH "RUNNING-ALL 1.0.0"

const unsigned int PERIOD_MEASURE = 3000;      // Time in miliseconds between measurements

// Limits of random values for mimicking real physical measurement
const unsigned int SENSOR_DATA_MIN = 0;
const unsigned int SENSOR_DATA_MAX = 1023;

// Variables and constants for measurement
const unsigned int RUNNING_SAMPLES = RUNNINGSTATISTIC_BUFFER_DEF;
unsigned int demoData;
unsigned int filterMed, filterAvg, filterMin, filterMax;  // Running statistics
RunningStatistic runningMed(RUNNINGSTATISTIC_MEDIAN,  SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES);
RunningStatistic runningAvg(RUNNINGSTATISTIC_AVERAGE, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES);
RunningStatistic runningMin(RUNNINGSTATISTIC_MINIMUM, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES);
RunningStatistic runningMax(RUNNINGSTATISTIC_MAXIMUM, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES);

void setup()
{
#if defined(ARDUINO)
  Serial.begin(9600);
  Serial.println(F(SKETCH));
  Serial.println(F(RUNNINGSTATISTIC_VERSION));
  Serial.print(F("Running statistics from items: "));
  Serial.println(runningMed.getBufferLen());
  Serial.print(F("Valid sensor data range: "));
  Serial.print(runningMed.getValueMin());
  Serial.print(F(" ~ "));
  Serial.println(runningMed.getValueMax());
  // Print header
  Serial.println(F("Data\tMedian\tAverage\tMinimum\tMaximum"));
#elif defined(PARTICLE)
  Particle.publish("Sketch",  String(SKETCH));
  Particle.publish("Library", String(RUNNINGSTATISTIC_VERSION));
  Particle.publish("Readings", String(runningMed.getBufferLen()));
  Particle.publish("Range", String::format("%4d ~ %4d", runningMed.getValueMin(), runningMed.getValueMax()));
#endif
}

void loop()
{
  processing();
}

void processing()
{
  static unsigned long timeStamp;
  if (millis() - timeStamp >= PERIOD_MEASURE)
  {
    timeStamp = millis();
    demoData  = random(SENSOR_DATA_MIN, SENSOR_DATA_MAX + 1);
    filterMed = runningMed.getStatistic(demoData);
    filterAvg = runningAvg.getStatistic(demoData);
    filterMin = runningMin.getStatistic(demoData);
    filterMax = runningMax.getStatistic(demoData);
    // Publish result
#if defined(ARDUINO)
    Serial.print(demoData);
    Serial.print(F("\t"));
    Serial.print(filterMed);
    Serial.print(F("\t"));
    Serial.print(filterAvg);
    Serial.print(F("\t"));
    Serial.print(filterMin);
    Serial.print(F("\t"));
    Serial.print(filterMax);
    Serial.println();
#elif defined(PARTICLE)
    Particle.publish("Data/Med/Avg/Min/Max", String::format("%4d/%4d/%4d/%4d/%4d", demoData, filterMed, filterAvg, filterMin, filterMax));
#endif
  }
}
