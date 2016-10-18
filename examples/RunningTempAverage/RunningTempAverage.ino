/*
  NAME:
  Basic usage of RunningStatistic library

  DESCRIPTION:
  This sketch demonstrates the use of RunningStatistic without determining
  optional arguments so that it uses the default values of them.
  * The sketch calculates just the running average from default number of
    previously calculated values.
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
#define SKETCH "RunningTempAverage 1.3.0"

// The multiplier of the sensor (degC/bit) for calculating temperature from reading
const float COEF_CONF = 0.107527;

// Pin for analog reading of the sensor LM35DZ
const byte PIN_LM35 = A0;

// Time in miliseconds between temperature readings
const word PERIOD_DELAY = 5000;

word sensorData;              // Read sensor data
word processedData;           // Statistically processed sensor data
float processedTemp;          // Statistically processed ambient temperature

// Create object for running averaged temperature
RunningStatistic avgRunningTemp;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);  // Set analog reference voltage to 1.1V
  Serial.println(F(SKETCH));
  Serial.println(F(RUNNINGSTATISTIC_VERSION));
  // Print header
  Serial.print(F("Running average from items: "));
  Serial.println(avgRunningTemp.getBufferLen());
  Serial.println(F("Reading\tAverage\tTemp\tItems"));
}

void loop() {
  // New running values
  sensorData = analogRead(PIN_LM35);
  processedData = avgRunningTemp.getStatistic(sensorData);
  processedTemp = processedData * COEF_CONF;
  // Show result
  Serial.print(sensorData);
  Serial.print(F("\t"));
  Serial.print(processedData);
  Serial.print(F("\t"));
  Serial.print(processedTemp, 1);
  Serial.print(F("\t"));
  Serial.println(avgRunningTemp.getReadings());
  delay(PERIOD_DELAY);
}