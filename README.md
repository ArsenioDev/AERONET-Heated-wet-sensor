<img align="right" width="138" height="138" src="https://www.x20.org/wp-content/uploads/2015/01/NASA-logo.png">
<img align="right" width="108" height="108" src="https://upload.wikimedia.org/wikipedia/commons/a/a0/United_States_Naval_Academy.png">

# AERONET-Heated-wet-sensor

## Heated Wet Sensor for the NASA AERONET program

![Ray tracing render](/IMG/DewDeleterPCB.PNG)
Designed as part of USNA-to-NASA internship by Victor Zimmerman. 

### Description
 

A PCB designed to cure NASA AERONET robots suffering from dew induced crepuscular narcolepsy (instruments remaining in standby due to dew on wet sensor). The PCB is connected to temperature sensors, CIMEL control box and a heated wet sensor, in order to determine if the wet sensor is at risk of dew condensation. If so, turns on heater to prevent dew condensation. Determines dew risk by calculating dew point at BME280(atmospheric DP) and compares it to dew point calculated by temperature sensor underneath heated wet sensor(Sensor DP). Heats wet sensor until sensor DP is higher than atmospheric DP. Formula used to calculate dew point: 

![𝐷𝑃=𝑇−(100 −𝐻)/5](https://github.com/ArsenioDev/AERONET-Heated-wet-sensor/assets/134075255/2027e12e-ac90-4998-baf8-d848c2ede493)

Is mostly a "plug-and-play" design; does not need any programming or input from operator beyond initial code load. Runs on 12V and minimum 1.5A AC power supply (1.2A of which is for the heater, I_Lim set by load driver).


#### Initial code load prerequisites/software dependencies
* Arduino IDE with [STM32Duino](https://github.com/stm32duino/Arduino_Core_STM32) BSP core added
* Include libraries: [BolderFlight BME280](https://github.com/bolderflight/bme280), [OneWire](https://github.com/PaulStoffregen/OneWire), and [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library)
* SWD (Serial Wire Debug) upload mechanism from list of uploads. 
* Core selected **Generic STM32F103C6**

### Progress
- [x] Attempted to use RX8130
- [x] failed to use RX8130 :(
- [x] Succesfully pivoted to DS3121
- [x] realized RTC was unneccesary 
- [x] finished code
- [x] finished spahgetti point to point (with italian falg colored wires)
- [ ] ~~failed test in temperature chamber~~
- [x] ~~give up creating dew condensation after a PHD told us it would not work~~
- [x] Meanwhile, forced dew condensation in temperature chamber
- [x] Defined schematic of PCB with teensy stand in
- [x] replaced teensy with STM32F103
- [x] Finished schematic
- [x] routed PCB
- [x] Fine Tuned
- [x] Swagged out silkscreen
- [x] issued production order
- [x] finished progress list
- [ ] finished README and documentat...zzz

![Ray tracing render](/IMG/DewDeleterSponsors.PNG)

