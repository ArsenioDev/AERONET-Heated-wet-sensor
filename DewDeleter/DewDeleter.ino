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
//#include <DS3231.h>

/* BME-280 */
bfs::Bme280 bme;

OneWire oneWire(2); 
DallasTemperature sensors(&oneWire); 
//DS3231 myRTC;

// byte year;
// byte month;
// byte date;
// byte dOW;
// byte hour;
// byte minute;
// byte second;

// bool century = false;
// bool h12Flag;
// bool pmFlag;
// byte alarmDay, alarmHour, alarmMinute, alarmSecond, alarmBits;
// bool alarmDy, alarmH12Flag, alarmPmFlag;
// int counter = 10;

void setup() {
  /* Serial monitor for showing status and data */
  Serial.begin(115200);
  sensors.begin(); 
  while (!Serial) {}
  /* Initialize the I2C bus */
  Wire.begin();
  Wire.setClock(400000);
  /* Wire at the primary I2C s */
  bme.Config(&Wire, bfs::Bme280::I2C_ADDR_PRIM);// A8 and A7
  /* Initialize the BME-280 */
  if (!bme.Begin()) {
    Serial.println("Error initializing communication with BME-280");
    while (1) {}
  }

  pinMode(21, INPUT);
  pinMode(22, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
}

void loop() {
  //date setting function
  // char wantSet = Serial.read();
  // if (wantSet == 'x') {
  //   Serial.println("1 fish");
  //   setDateStuff(year, month, date, dOW, hour, minute, second);
  // }

  
  DangerDew(AtmoSensorDew(), LongTempSensorDew());
  

  SerialPrinter(AtmoSensorDew(), LongTempSensorDew());

  // digitalWrite(0, myRTC.checkIfAlarm(1, true));
  // digitalWrite(1, myRTC.checkIfAlarm(2, true));

  delay(500);
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

  boolean danger = AtmoDewPont + 10 > SenseDewPont;

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

  Serial.print("Temp:");
  Serial.print(bme.die_temp_c());
  // Serial.print(", FreedomTemp:");
  // Serial.print(FreedomUnits(bme.die_temp_c()));
  Serial.print(", Humid:");
  Serial.print(bme.humidity_rh());
  Serial.print(", DewPont:");
  Serial.print(AtmoDewPont);

  Serial.print(", LongSenseC:"); 
  Serial.print(sensors.getTempCByIndex(0));

  // Serial.print(", LongSenseF:"); 
  // Serial.print(sensors.getTempFByIndex(0)); 

  Serial.print(", LongSenseDewPoint:");
  Serial.print(SenseDewPont);

  Serial.print(", ethernet reading:");
  Serial.print(analogRead(21));

  // Serial.print("   Time(YYMMDDwHHMMSS): 2");
	// if (century) {			// Won't need this for 89 years.
	// 	Serial.print("1");
	// } else {
	// 	Serial.print("0");
	// }
	// Serial.print(myRTC.getYear(), DEC);
	// Serial.print(' ');
	
	// // then the month
	// Serial.print(myRTC.getMonth(century), DEC);
	// Serial.print(" ");
  
	// // then the date
	// Serial.print(myRTC.getDate(), DEC);
	// Serial.print(" ");
  
	// // and the day of the week
	// Serial.print(myRTC.getDoW(), DEC);
	// Serial.print(" ");
  
	// // Finally the hour, minute, and second
	// Serial.print(myRTC.getHour(h12Flag, pmFlag), DEC);
	// Serial.print(" ");
	// Serial.print(myRTC.getMinute(), DEC);
	// Serial.print(" ");
	// Serial.print(myRTC.getSecond(), DEC);

  // Serial.print("   ");

  // //chack alarms
  // Serial.print(myRTC.checkAlarmEnabled(1));
  // Serial.print(" ");
  // Serial.print(myRTC.checkAlarmEnabled(2));

  // Serial.print("   ");
  // Serial.print(myRTC.checkIfAlarm(1, false));
  // Serial.print(" ");
  // Serial.print(myRTC.checkIfAlarm(2, false));

  Serial.println();

}

//Clock Functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// void setDateStuff(byte& year, byte& month, byte& date, byte& dOW, byte& hour, byte& minute, byte& second) {
//   // Call this if you notice something coming in on
//   // the serial port. The stuff coming in should be in
//   // the order YYMMDDwHHMMSS, with an 'x' at the end.
//   boolean gotString = false;
//   char inChar;
//   byte temp1, temp2;
//   char inString[20];
  
//   Serial.println("2 fish");

//   //byte j=0;
//   // while (!gotString) {
//   //   if (Serial.available()) {
//   //     Serial.println("4 fish");
//   //     inChar = Serial.read();
//   //     inString[j] = inChar; //STUPID WHILE LOOP
//   //     j += 1;               //let this be a testament to while loop's inadequecy 
//   //     if (inChar == 'x') {
//   //       Serial.println("5 fish");
//   //       gotString = true;
//   //     }
//   //   } 
//   // }

//   for(int i = 0; i < 13; i++){
//     inChar = Serial.read();
//     inString[i] = inChar;
//   }

//   Serial.println(inString);
//   // Read year first
//   temp1 = (byte)inString[0] -48;
//   temp2 = (byte)inString[1] -48;
//   year = temp1*10 + temp2;
//   // now month
//   temp1 = (byte)inString[2] -48;
//   temp2 = (byte)inString[3] -48;
//   month = temp1*10 + temp2;
//   // now date
//   temp1 = (byte)inString[4] -48;
//   temp2 = (byte)inString[5] -48;
//   date = temp1*10 + temp2;
//   // now Day of Week
//   dOW = (byte)inString[6] - 48;
//   // now hour
//   temp1 = (byte)inString[7] -48;
//   temp2 = (byte)inString[8] -48;
//   hour = temp1*10 + temp2;
//   // now minute
//   temp1 = (byte)inString[9] -48;
//   temp2 = (byte)inString[10] -48;
//   minute = temp1*10 + temp2;
//   // now second
//   temp1 = (byte)inString[11] -48;
//   temp2 = (byte)inString[12] -48;
//   second = temp1*10 + temp2;

//   myRTC.setClockMode(false);  // set to 24h
//   //setClockMode(true); // set to 12h
      
//   myRTC.setYear(year);
//   myRTC.setMonth(month);
//   myRTC.setDate(date);
//   myRTC.setDoW(dOW);
//   myRTC.setHour(hour);
//   myRTC.setMinute(minute);
//   myRTC.setSecond(second);
  
//   // Test of alarm functions
//   // set A1 to one minute past the time we just set the clock
//   // on current day of week.


//   myRTC.setA1Time(dOW, hour, minute + 1, second, 00001000 /*sets alarm to match hour, minutes and seconds*/, true, false, false);
  
//   // set A2 to two minutes past, on current day of month.
//   myRTC.setA2Time(date, hour, minute + 3, 00001000 /*sets alarm to match hour, minutes and seconds*/, false, false, false);
//   // Turn on both alarms, with external interrupt
//   myRTC.turnOnAlarm(1);
//   myRTC.turnOnAlarm(2);
      
// }
