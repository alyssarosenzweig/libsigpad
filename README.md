libsigpad
==========

libsigpad implements a userspace driver for Topaz signature pads, at the moment capable of display bitmaps on certain HID USB models. The sample `clock` shows an example of how to use this library (text rendering is implemented in library, although pull requests to switch to a library like FreeType would be welcome.)

About
==========

I found one of these devices at a surplus store missing the pen and documentation but otherwise functional. That being said, there were no free driver available, so I fuzzed the device and later reverse-engineered enough of the protocol to get images. See `libsigpad.c` for the implementation of the protocol itself.
