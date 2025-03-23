# FreeRTOS Hands-on Exercise Repo

This repository contains the working source files to facilitate FreeRTOS learning through hands-on coding exercises on board a Blue Pill devkit.

To use this, simply clone the repository into your preferred IDE (Eclipse, VSCode, etc.).  Build your application by adding your tasks and other FreeRTOS utilities into main.c and add other sources if needed.

There is no need to modify other sources, as this may break the current working functionality.

This has been tested with gcc-arm-none-eabi toolchain.


# Hardware Devkit 

This repository is for use with Blue Pill board/devkit.  It contains the MCU STM32F103C8T6 which has 64KB Flash and 20KB RAM.

The exercise covers the seven-segment display whose pins are mapped to the Blue Pill as follows:

      SEVEN   BLUE
     SEGMENT  PILL
        a      A0
        b      A1
        c      A2
        d      A3
        e      A4
        f      A5
        g      A6


Printing characters via seria port is also supported.  UART1 module is used and the following pins are used.

       RX      A10
       TX      A9
       
Send the character strings serially by using printf.  Take note that a separate UART-to-USB converter must be provided.       