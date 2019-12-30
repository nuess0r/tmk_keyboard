USB Controller for a IBM Model M Keyboard
=========================================

This is a port of the TMK firmware to the dulcimer project.

The dulcimer project is a USB replacement controller for the IBM Model M keyboard.

You can find the original documentation here:
http://www.schatenseite.de/2008/07/12/dulcimer/

This controller uses an ATmega32 and the VUSB library for USB support.

If you happen to have a Teensy 2.0++ lying around, you can use a different port
of TMK to make your beloved Model M future proof:
https://blog.lmorchard.com/2016/02/21/modelm-controller/ (Thanks to Les Orchard
for all the work to create the Model M keymap)

Keymaps
-------

By default this TMK firmware provides a classical QUERTY layout for 101 keys.

The project provides additional keymaps with multimedia keys and for 103 keys.
See Building Yourself how to build the firmware with this optional mappings.

In the variant keymap_101_modernized.c (and the same for 103 keys) some keys are
remapped to more modern use cases:
- Caps lock is now Windows Key or Command key in Mac
- F11 is now Media Play
- F12 is now Media Next
- Print Screen is now Volume Mute
- Scroll lock is now Volume up
- Break is now Volume down

Hardware
---------

You can use the original dulcimer hardware without modification with
this TMK firmware.

If you don't own one, you can build one yourself. In the circuit sub-folder
is a KiCAD project for the dulcimer.

It's a one layer PCB, so you can either etch your self a PCB or order a PCB
from yourfavorite manufacturer.

Firmware
--------

The firmware consists of two parts, the bootloader and the actual TMK firmware.






Building Firmware Yourself
--------------------------

You need to install avr-gcc, the avr glibc and avrdude.

### Bootloader

Enter the bootloader directory and execute

    make

Use avrdude with your programmer settings adjusted:
    avrdude -P /dev/ttyACM0 -c STK500 -p atmega32 -U flash:w:obj/main.hex -U hfuse:w:0xc8:m -U lfuse:w:0x8f:m

### TMK Firmware

After you flashed the bootloader you can make and flash the TMK firmware.

Before you build it, you have to decide which keymap you would like to use.

Rename the desired keymap file to `keymap.c` and build the firmware:

    make

If your bootloader is loaded and recognized by your OS you can flash it with a simple

    make program

or use again avrdude with your programmer settings adjusted:
    avrdude -P /dev/ttyACM0 -c STK500 -p atmega32 -U flash:w:dulcimer.hex

Debugging
---------

Debugging is done via the serial UART in the AVR. No debugging messages are transferred via USB.

The transmit pin (TX) is available on the connector JP4.

It is 5 V TTL, so you will need a RS232 level shifter or a 5 V tolerant USB
serial adapter (you can use two resistors to build a voltage divider to
protect your 3.3 V tolerant adapter)

You can enable the debug messages by pressing the magic command `LShift` + `RShift` + `d`
