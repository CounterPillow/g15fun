g15fun
======

Example program that demonstrates how to talk to the LCD inside the Logitech G15 (and similar devices) on Linux without needing g15daemon, by using raw HID devices.


Usage
-----

Compile with `make`, run with `./g15fun`. If you don't have udev rules set up for it (only Fedora does by default, to my knowledge), you may need to run as root.


Acknowledgements
----------------

This wouldn't have been possible without Hans de Goede's work on lcdproc, which resulted in this newer and cleaner approach of talking to the device. I also use his `hidraw_lib` code, albeit slightly modified to get rid of the logging code, to find the right hidraw device node to open.

`miku.h` is a cropped and dithered version of [an image drawn by @04119__snail on Twitter](https://twitter.com/04119__snail/status/1392368943885537282).
