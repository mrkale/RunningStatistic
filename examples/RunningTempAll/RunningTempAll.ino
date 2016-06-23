/*
  NAME:
  Full usage of RunningStatistic library

  DESCRIPTION:
  This sketch demonstrates the use of RunningStatistic with determining all
  arguments that it provides with their default values.
  * The sketch calculates all running statistical types that the library provides.
    In the first line under the header is sensor data for particular statistic.
    In the second line under the header is particular temperature statistic.
  * As a physical value the sketch employs the analog reading of LM35DZ
    temperature sensor and calculates temperature in centigrades.
  * The sketch uses internal reference voltage 1.1V for better resolution.
  * The sensitivity of LM35DZ is 10mV/degC with 0mV at 0degC. At 10-bit reading
    resolution the temperature convertion coeficient is 0.107527 degC/bit, so
    that the ambient temperature 20degC is read as 186 bits.
  * The measurement range of LM35DZ is 0 - 100degC, which is 0 - 100mV read
    in the range 0 - 930 bits at 10-bit resolution and 1100mV reference.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "RunningStatistic.h"
#define SKETCH_VERSION "1.1.0"

// The multiplier of the sensor (degC/bit) for calculating temperature from reading
const float convCoef = 0.107527;

// Pin for analog reading of the sensor LM35DZ
const byte pinLM35 = A0;

// Number of readings for calculating running value.
const byte sampleCount = 5;

// Time in miliseconds between temperature readings
const word runDelay = 5000;

word sensorData;              // Data from sensor
word processedData[4];        // Statistically processed sensor data
float processedTemp;          // Statistically processed ambient temperature

// Create array of objects for running statistics of temperature
RunningStatistic stat[] = {
  RunningStatistic(RUNNINGSTATISTIC_MEDIAN, sampleCount),
  RunningStatistic(RUNNINGSTATISTIC_AVERAGE, sampleCount),
  RunningStatistic(RUNNINGSTATISTIC_MINIMUM, sampleCount),
  RunningStatistic(RUNNINGSTATISTIC_MAXIMUM, sampleCount)
};

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);  // Set analog reference voltage to 1.1V
  Serial.print(F("Sketch v"));
  Serial.println(SKETCH_VERSION);
  Serial.print(F("RunningStatistic v"));
  Serial.println(RUNNINGSTATISTIC_VERSION);
  Serial.print(F("Running statistics from items: "));
  Serial.println(sampleCount);
  // Print header
  Serial.println(F("Reading\tMedian\tAverage\tMinimum\tMaximum"));
}

void loop() {
  sensorData = analogRead(pinLM35);

  // Sensor data list
  Serial.print(sensorData);
  Serial.print(F("\t"));
  for (byte i = 0; i < sizeof(processedData)/sizeof(processedData[0]); i++) {
    processedData[i] = stat[i].getStatistic(sensorData);
    Serial.print(processedData[i]);
    Serial.print(F("\t"));
  }
  Serial.println();

  // Temperature list
  processedTemp = sensorData * convCoef;
  Serial.print(processedTemp, 1);
  Serial.print(F("\t"));
  for (byte i = 0; i < sizeof(processedData)/sizeof(processedData[0]); i++) {
    processedTemp = processedData[i] * convCoef;
    Serial.print(processedTemp, 1);
    Serial.print(F("\t"));
  }
  Serial.println();
  Serial.println();
  delay(runDelay);
}
