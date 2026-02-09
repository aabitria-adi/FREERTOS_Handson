# FreeRTOS Hands-on Exercise Repo

This branch demonstrate the use of interrupts and capturing it into SystemView 
continuous trace.

To use this, simply clone the repository into your preferred IDE (Eclipse, 
VSCode, etc.).  Build your application by adding your tasks and other FreeRTOS 
utilities into main.c and add other sources if needed.

There is no need to modify other sources, as this may break the current working 
functionality.

This has been tested with gcc-arm-none-eabi toolchain.


# Hardware Devkit 

This repository is for use with Blue Pill board/devkit.  It contains the MCU 
STM32F103C8T6 which has 64KB Flash and 20KB RAM.

The green and red LEDs are connected to PC13 and PC14, respectively.  The GPIO 
pins act as SINK, i.e. it is connected to LED cathode side, determining by
inverted logic whether it turns the LED on (via 0) or off (via 1).  The LED 
anode is connected to current limiting resistor which is connected to Vcc on 
the other side.

The button GPIO is connected to PB1.  On one side it is directly connected to
VCC and on its other side it is pulled to GND via a pull-down resistor.  When 
idle it is pulled low, when pressed it is pulled high.

UART1 module is used for SystemView continuous trace capture, and the following 
pins are used.  SystemView must be configured for UART and baud rate of 576000
to enable capture.

       RX      A10
       TX      A9

Sample traces are attached with .SVdat extensions.  These can be opened and 
examined with SystemView.       

Schematic is also included as well as the FreeRTOS Guide.