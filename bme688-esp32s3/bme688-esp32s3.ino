/**
 * Copyright (C) 2021 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 */

#include "Arduino.h"
#include "bme68xLibrary.h"

#define NEW_GAS_MEAS (BME68X_GASM_VALID_MSK | BME68X_HEAT_STAB_MSK | BME68X_NEW_DATA_MSK)

#ifndef PIN_CS
#define PIN_CS 43
#endif

Bme68x bme;

/**
 * @brief Initializes the sensor and hardware settings
 */
void setup(void)
{
	SPI.begin();
	Serial.begin(115200);

	while (!Serial)
		delay(10);

	/* Initializes the sensor based on SPI library */
	bme.begin(PIN_CS, SPI);

	if(bme.checkStatus())
	{
		if (bme.checkStatus() == BME68X_ERROR)
		{
			Serial.println("Sensor error:" + bme.statusString());
			return;
		}
		else if (bme.checkStatus() == BME68X_WARNING)
		{
			Serial.println("Sensor Warning:" + bme.statusString());
		}
	}

	/* Set the default configuration for temperature, pressure and humidity */
	bme.setTPH();

	/* Heater temperature in degree Celsius */
	uint16_t tempProf[10] = { 100, 200, 320 };
	/* Heating duration in milliseconds */
	uint16_t durProf[10] = { 150, 150, 150 };

	bme.setSeqSleep(BME68X_ODR_250_MS);
	bme.setHeaterProf(tempProf, durProf, 3);
	bme.setOpMode(BME68X_SEQUENTIAL_MODE);

	Serial.println(", , , , , , ");
}

void loop(void)
{
	bme68xData data;
	uint8_t nFieldsLeft = 0;

	delay(150);

	if (bme.fetchData())
	{
		do
		{
			nFieldsLeft = bme.getData(data);
			//if (data.status == NEW_GAS_MEAS)
			{
				Serial.print("TimeStamp(ms)" + String(millis()) + ", ");
				Serial.print("Temperature(deg C)" + String(data.temperature) + ", ");
				Serial.print("Pressure(Pa)" + String(data.pressure) + ", ");
				Serial.print("Humidity(%)" + String(data.humidity) + ", ");
				Serial.print("Gas resistance(ohm)" + String(data.gas_resistance) + ", ");
				Serial.print("Status" + String(data.status, HEX) + ", ");
				Serial.println("Gas index" + data.gas_index);
				if(data.gas_index == 2) /* Sequential mode sleeps after this measurement */
					delay(250);
			}
		} while (nFieldsLeft);
	}
}