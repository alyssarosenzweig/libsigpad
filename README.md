libsigpad
==========

libsigpad implements a userspace driver for Topaz signature pads, at the moment capable of display bitmaps on certain HID USB models. The sample `clock` shows an example of how to use this library.

Dependencies
==========

libsigpad uses the [RawHID library](http://www.pjrc.com/teensy/rawhid.html), which it expects to coexist with. It additionally builds with the RawHID makefile (change the source name as needed).

Privileges
=========

In certain configurations, root permissions may be necessary to use USB devices. If this is the case, it may be mitigated through, e.g. udev rules. See the [RawHID documentation](http://www.pjrc.com/teensy/rawhid.html) for more information.

About
==========

I found one of these devices at a surplus store missing the pen and documentation but otherwise functional. That being said, there were no free driver available, so I fuzzed the device and later reverse-engineered enough of the protocol to get images. See `libsigpad.c` for the implementation of the protocol itself.
