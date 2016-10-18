/*
  NAME:
  Full usage of RunningStatistic library with sensor data filtering

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
#define SKETCH "RunningTempAll 1.3.0"

// The multiplier of the sensor (degC/bit) for calculating temperature from reading
const float COEF_CONF = 0.107527;

// Pin for analog reading of the sensor LM35DZ
const byte PIN_LM35 = A0;

// Time in miliseconds between temperature readings
const word PERIOD_DELAY = 5000;

word sensorData;              // Data from sensor
word processedData[4];        // Statistically processed sensor data
float processedTemp;          // Statistically processed ambient temperature

// Create array of objects for running statistics of temperature
// Minimal and maximal sensor value for the reasonable temperature range 5 - 50 degC
const word SENSOR_DATA_MIN = 46;  // ~5/0.107527
const word SENSOR_DATA_MAX = 465; // ~50/0.107527
const word RUNNING_SAMPLES = 11;  // Specific number of subsequent values for running statistics
RunningStatistic stat[] = {
  RunningStatistic(RUNNINGSTATISTIC_MEDIAN,  SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES),
  RunningStatistic(RUNNINGSTATISTIC_AVERAGE, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES),
  RunningStatistic(RUNNINGSTATISTIC_MINIMUM, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES),
  RunningStatistic(RUNNINGSTATISTIC_MAXIMUM, SENSOR_DATA_MAX, SENSOR_DATA_MIN, RUNNING_SAMPLES)
};

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);  // Set analog reference voltage to 1.1V
  Serial.println(F(SKETCH));
  Serial.println(F(RUNNINGSTATISTIC_VERSION));
  Serial.print(F("Running statistics from items: "));
  Serial.println(stat[0].getBufferLen());
  Serial.print(F("Valid sensor data range: "));
  Serial.print(stat[0].getValueMin());
  Serial.print(F(" ~ "));
  Serial.println(stat[0].getValueMax());
  // Print header
  Serial.println(F("Reading\tMedian\tAverage\tMinimum\tMaximum"));
}

void loop() {
  sensorData = analogRead(PIN_LM35);

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
  processedTemp = sensorData * COEF_CONF;
  Serial.print(processedTemp, 1);
  Serial.print(F("\t"));
  for (byte i = 0; i < sizeof(processedData)/sizeof(processedData[0]); i++) {
    processedTemp = processedData[i] * COEF_CONF;
    Serial.print(processedTemp, 1);
    Serial.print(F("\t"));
  }
  Serial.println();
  Serial.println();
  delay(PERIOD_DELAY);
}
