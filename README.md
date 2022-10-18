# SAMD21-RTC-Clock
Use the Real Time Clock (RTC) peripheral on a SAMD21 microcontroller as a date-and-time clock

## Introduction
There can be advantages to having an RTC right there, on the silicon, rather than having to communicate with an external one such as the DS3231.

It is much faster to access date and time data directly from the device memory.

Direct access to the RTC is possible by means of register names defined in the Arduino Core for SAMD21.

Those pre-defined names allow programs to avoid importing a third-party library.

## Contents

* [The SAMD21 Clock System](#the-samd21-clock-system)
* [Clock Source](#clock-source)
* [Generic Clock Generator](#generic-clock-generator)
* [RTC Peripheral](#rtc-peripheral)
* [Synchronizing and Enabling](#synchronizing-and-enabling)
* [References](#references)

## The SAMD21 Clock System
Useful articles available online include:

* [*Clock System Configuration*](https://microchipdeveloper.com/32arm:samd21-clock-system-configuration), by Microchip
* [*Understanding the SAM D21 clocks*](https://blog.thea.codes/understanding-the-sam-d21-clocks/), by Stargirl Flowers

Three, different resources inside the SAMD21 must be linked together in the code:

1. *clock source*, an oscillator running at 32,768 Hz
2. *generic clock generator*
3. *RTC peripheral*

Linking them together is done by writing certain values into certain registers during the setup process.

See the code examples for details.

### Clock source
Most of the SAMD21 Arduinos include an external crystal oscillator running at 32,768 Hz. The Arduino Core names this source, XOSC32K. It's the most accurate source.

At the time of writing, the author knew of only one that does not: the Arduino Nano 33 IoT. It provides a slightly less accurate, internal, 32,768 Hz oscillator named OSC32K.

The Arduino library automatically enables the most accurate 32,768 Hz clock source. It means you code does not need to do anything about getting the source ready.

### Generic Clock Generator
SAMD provides eight of these, numbered 0 through 7. Arduino Core uses the first three or four, leaving the rest for user code. The examples make use of GCLK number 4.

GLCK4 can be configured to:

* receive the 32,768 Hz signal from the XOSC32K source,
* scale the signal down to 1,024 Hz, and
* send that scaled-down signal on to the RTC peripheral.

### RTC Peripheral
Three different operating modes are available for this device. The code examples elect Mode 2, the Clock/Calendar mode.

The RTC can be configured to scale the incoming 1,024 Hz signal down to 1 Hz, i.e., one "tick" each second.

In Mode 2, the date and time values are stored as subsections of a single, 32-bit bitfield register, named CLOCK. 

Each incoming tick increments the Seconds value. Rollover into Minutes, Hours, Days, Months and Years is handled automatically. The data sheet claims that leap years are handled correctly.

The CLOCK register can be read or written as a single, 32-bit integer. The individual subsections can be read individually, also.

### Synchronizing and Enabling
The GCLK and RTC registers serve in a sort of intermodal capacity. They are transfer points between your code and the hardware. 

This arrangement is necessary because the speed at which your code runs may differ from that of the peripheral. 

Writing a value to one of these registers initiates a necessary "synchronization" procedure that transfers the value on to the target device. 

It happens automatically. However, the code needs to wait after writing, for the sync to complete. A certain "Sync" bit goes high when the process begins and goes low to indicate it has completed.

Likewise, synchronization may be needed before reading a value from one of the registers. Writing a certain "read request" bit starts the sync procedure. Code then waits for the Sync bit to go low, after which the requested value can be read.

Think of it as catching a subway train. First you walk to the platform. Then you wait for the train. When it arrives and the doors open, the train and you become "synchronized" and you can step aboard. 

The data sheet identifies the registers that require synchronization for writing or for reading.

The GCLK and RTC need to be enabled, by setting each one's ENABLE bit.

Importantly, the RTC peripheral also needs to be un-masked in the Power Manager. Doing this allows power to the peripheral. The data sheet states that attempting to access a peripheral while it is masked (powered-down) may lock up the CPU.

## Examples
The example programs are designed to set a date and time, for example, 7:30 a.m. on 16 October 2022.  Then, at five-second intervals, read and display the updated date and time values. For example:

> Serial started<br>
> Initial:<br>
> 10/16/22 7:30:0<br>
> Update:<br>
> 10/16/22 7:30:5<br>
> Update:<br>
> 10/16/22 7:30:10<br>
> Update:<br>
> 10/16/22 7:30:15<br>

An example Arduino program is provided for use with the Seeed Studio SAMD21 XIAO module. 

If the SerialUSB output does not appear in the Serial Monitor after uploading the code, it could mean that the Arduino IDE closed the serial port of the device. Try unplugging the USB cable then plugging it back in to re-open the serial port.

The XIAO example may be readily adaptable to other Arduino models that include an external 32,768 Hz crystal oscillator. Replace the statements, ```SerialUSB.print()``` and ```SerialUSB.println()```, with their Arduino hardware counterparts, ```Serial.print()``` and ```Serial.println()```.

## References

* [The SAMD21 Family Data Sheet from Microchip](https://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_DataSheet_DS40001882F.pdf)
* Arduino Core Library Files
    * [GCLK instance header](https://github.com/arduino/ArduinoModule-CMSIS-Atmel/blob/master/CMSIS-Atmel/CMSIS/Device/ATMEL/samd21/include/instance/gclk.h)
    * [GCLK component header](https://github.com/arduino/ArduinoModule-CMSIS-Atmel/blob/master/CMSIS-Atmel/CMSIS/Device/ATMEL/samd21/include/component/gclk.h)
    * [RTC instance header](https://github.com/arduino/ArduinoModule-CMSIS-Atmel/blob/master/CMSIS-Atmel/CMSIS/Device/ATMEL/samd21/include/instance/rtc.h)
    * [RTC component header](https://github.com/arduino/ArduinoModule-CMSIS-Atmel/blob/master/CMSIS-Atmel/CMSIS/Device/ATMEL/samd21/include/component/rtc.h)
    * [Power Manager instance header](https://github.com/arduino/ArduinoModule-CMSIS-Atmel/blob/master/CMSIS-Atmel/CMSIS/Device/ATMEL/samd21/include/instance/pm.h)
    * [Power Manager component header](https://github.com/arduino/ArduinoModule-CMSIS-Atmel/blob/master/CMSIS-Atmel/CMSIS/Device/ATMEL/samd21/include/component/pm.h)


