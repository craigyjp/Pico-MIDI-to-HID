# MIDI to HID Keyboard

The object of this project is to take standard MIDI CC messages (value not important) and send a corresponding ASCII character over the USB to the host device.

From MIDI CC messages 1-26 it will output ascii codes "A" to "Z", CC 27-36 are numbers 1-9 & 0
Special commands such as ENTER, are 100, ESCAPE 101, UP ARROW 102, DOWN ARROW 103, LEFT ARROW 104, RIGHT ARROW 105.

Easily add more CC messages to the loop

Uses a Seeed XIAO RP2040 board for its size, but I guess any RP2040 based board will work.



