# MIDI to HID Keyboard

The object of this project is to take standard MIDI CC messages (value not important) and send a corresponding ASCII character over the USB to the host device.

From MIDI CC messages 1-20 it will output ascii codes "a" to "t" over the USB output

Easily add more CC messages to the loop

Uses a Seeed XIAO RP2040 board for its size, but I guess any RP2040 based board will work.



