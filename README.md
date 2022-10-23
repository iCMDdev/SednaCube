# SednaCube
<img src="/sednacube-banner.png" alt="SednaCube Banner Image">

SednaCube is Sedna PocketQube Team's data logging device, submitted for the [Qube2Space](https://students.2space.ro/qube2space.html) competition. Source code and schematics are available here.

# Whoami
[Sedna](https://en.wikipedia.org/wiki/90377_Sedna) is a tiny, little dwarf planet in our Solar System, situated at 1.26×10^10 km away from us. Likewise, SednaCube is a small [PocketQube](https://en.wikipedia.org/wiki/PocketQube) that hopes to be as small as Sedna is when compared to other planets, and to work independently for as far away as Sedna is situated from us.

Another interesting aspect of Sedna is its orbit. This dwarf's orbital period is 11,408 years ([JPL](https://ssd.jpl.nasa.gov/sbdb.cgi?sstr=Sedna)). That's a lot of years! Similarly, SednaCube can record sensor data for about 5000 hours (more than 200 days!) on a 32GB SD card.

# Repository structure
This repository has the following folders:
- <b>[Code](/Code)</b> - includes the source code for the Arduino (both flight and debugging), as well as software for 3D visualisation such as Processing sketches.
- <b>[Schematic](/Schematic)</b> - includes the circuit's schematic
- <b>[PCB](/PCB)</b> - includes the PCBs' Gerber files. The device has 5 stacked PCBs, connected using XH2.54 connectors.

# Description
SednaCube is a PocketQube designed for a rocket flight up to 9000m. Its mission is to record data such as pressure, temperature, altitude, humidity, orientation and location during a rocket flight that will take place later this year. The data will be then analysed, hoping that we will find interesting information about the flight and environment.

<i>Used hardware: Arduino Nano, GY-91 (BMP280 & MPU9250), DHT11, SIM800L, GY-NEO6MV2.</i>

Due to AVR's memory limits, and since we want the fastest record rate possible (contest limit is minimum 1Hz, we achieved speeds up to 20-25Hz), the flight version of the program is esentially only recording data to the SD card in some CSV files, and reporting it's location through SMS using a SIM module. It doesn't process or analyse any data, since this can be done after flight. Moreover, the flight version is stripped of debugging methods (with some exceptions: the LEDs and logging data saved into EEPROM) for the same reasons. However, versions specially made for debugging during the building process are available.

For data analysis, we will use spreadsheet editors with their built-in graph functions, frameworks such as [Matplotlib](https://matplotlib.org) or [Swift Charts](https://developer.apple.com/documentation/charts), and a groundbreaking Flight Replay feature, in which the SednaCube will read data from the SD card and act as if it was real data captured from the sensors. We will be using Processing 3 / 4 or another similar software to create 3D flight simulations, as well as realtime sensor data visualisations (helpful for debugging), using a USB connection to the device.

# Functional Diagram
<img src="/sednacube-diagram.png" alt="SednaCube Functional Diagram Image">

