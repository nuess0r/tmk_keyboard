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

By default this TMK firmware provides a classical QUERTY layout for 101/103 keys.

The project provides an additional modernized keymap with multimedia keys.
See Building Yourself how to build the firmware with this optional mappings.

In the variant keymap_modernized.c some keys are
remapped to more modern use cases:
- Caps lock is now Windows Key or Command key in Mac
- F11 is now Media Play
- F12 is now Media Next
- Print Screen is now Volume Mute
- Scroll lock is now Volume up
- Break is now Volume down

Boot Magic Configuration - Virtual DIP Switch
---------------------------------------------
Boot Magic are executed during boot up time. Press Magic key below then plug in keyboard cable.
Note that you must use keys of **Layer 0** as Magic keys. These settings are stored in EEPROM so that retain your configure over power cycles.

To avoid configuring accidentally additive salt key `KC_INS` also needs to be pressed along with the following configuration keys. The salt key on the Model M is `insert` instead of the TMK default `space`!

#### General
- Skip reading EEPROM to start with default configuration(`ESC`)
- Clear configuration stored in EEPROM to reset configuration(`Backspace`)

#### Bootloader
- Kick up Bootloader(`B`)

#### Debug
- Debug enable(`D`)
- Debug matrix enable(`D`+`X`)
- Debug keyboard enable(`D`+`K`)
- Debug mouse enable(`D`+`M`)

#### Keymap
- Swap Control and CapsLock(`Left Control`)
- Change CapsLock to Control(`Caps Lock`)
- Swap LeftAlt and Gui(`Left Alt`)
- Swap RightAlt and Gui(`Right Alt`)
- Disable Gui(`Left Gui`)
- Swap Grave and Escape(`Grave`)
- Swap BackSlash and BackSpace(`Back Slash`)
- Enable NKRO on boot(`N`)

#### Default Layer
- Set Default Layer to 0(`0`)
- Set Default Layer to 1(`1`)
- Set Default Layer to 2(`2`)
- Set Default Layer to 3(`3`)
- Set Default Layer to 4(`4`)
- Set Default Layer to 5(`5`)
- Set Default Layer to 6(`6`)
- Set Default Layer to 7(`7`)


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

    avrdude -c dragon_isp -B 5 -p atmega32 -U flash:w:obj/main.hex -U hfuse:w:0xc8:m -U lfuse:w:0x8f:m


### TMK Firmware

After you flashed the bootloader you can make and flash the TMK firmware.

Before you build it, you have to decide which keymap you would like to use.

Rename the desired keymap file to `keymap.c` and build the firmware:

    make

The bootloader is loaded only for a few seconds before the TMK firmware gets loaded.
Flash the firwmare during this short period with a simple:

    make program

or use again avrdude with the settings adjusted for the usbasp bootloader:

    avrdude -p atmega32 -P /dev/ttyACM0 -c usbasp -U flash:w:dulcimer.hex -U lfuse:w:0x8F:m -U hfuse:w:0xC8:m

Debugging
---------

Debugging is done via the serial UART in the AVR. No debugging messages are transferred via USB.

The transmit pin (TX) is available on the connector JP4.

It is 5 V TTL, so you will need a RS232 level shifter or a 5 V tolerant USB
serial adapter (you can use two resistors to build a voltage divider to
protect your 3.3 V tolerant adapter)

You have to set `NO_UART = no` in the Makefile and after flashing you can enable
the debug messages by pressing the magic command `LShift` + `RShift` + `d`
