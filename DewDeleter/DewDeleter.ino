/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2021 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "bme280.h"
#include <OneWire.h>
#include <DallasTemperature.h> 

/* BME-280 */
bfs::Bme280 bme;

OneWire oneWire(2); 
DallasTemperature sensors(&oneWire); 


void setup() {
  /* Serial monitor for showing status and data */
  Serial.begin(115200);
  sensors.begin(); 
  while (!Serial) {}
  /* Initialize the I2C bus */
  Wire.begin();
  Wire.setClock(400000);
  /* Wire at the primary I2C address */
  bme.Config(&Wire, bfs::Bme280::I2C_ADDR_PRIM);
  /* Initialize the BME-280 */
  if (!bme.Begin()) {
    Serial.println("Error initializing communication with BME-280");
    while (1) {}
  }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {

  DangerDew(AtmoSensorDew(), LongTempSensorDew());
  SerialPrinter(AtmoSensorDew(), LongTempSensorDew());

  delay(1000);
}

//Unit Converters
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float FreedomUnits(float celcius){
  float freedom = (celcius * 1.8) + 32;
  return freedom; 
}

//Dew Point getters
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

float LongTempSensorDew(){ 
  sensors.requestTemperatures(); 

  return (sensors.getTempCByIndex(0)-((100 - bme.humidity_rh())/5));
}

float AtmoSensorDew(){ 
  bme.Read();

  return bme.die_temp_c()-((100 - bme.humidity_rh())/5);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DangerDew(float AtmoDewPont, float SenseDewPont){

  boolean danger = AtmoDewPont + 3 > SenseDewPont;

  if (danger){
    digitalWrite(6, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(6, LOW);
    digitalWrite(LED_BUILTIN,LOW);
  }

}

void SerialPrinter(float AtmoDewPont, float SenseDewPont){
  bme.Read();
  sensors.requestTemperatures(); 

  Serial.print(bme.pres_pa());
  Serial.print(", Temp:");
  Serial.print(bme.die_temp_c());
  Serial.print(", FreedomTemp:");
  Serial.print(FreedomUnits(bme.die_temp_c()));
  Serial.print(", Humid:");
  Serial.print(bme.humidity_rh());
  Serial.print(", DewPont:");
  Serial.print(AtmoDewPont);

  Serial.print(", LongSenseC:"); 
  Serial.print(sensors.getTempCByIndex(0));

  Serial.print(", LongSenseF:"); 
  Serial.print(sensors.getTempFByIndex(0)); 

  Serial.print(", LongSenseDewPoint:");
  Serial.println(SenseDewPont);

}
