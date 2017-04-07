% Reverse engineered protocol for Topaz signature pads

## Overview

The Topaz signature pads used a packet-based protocol, sent over various
interfaces depending on the model. Possible interfaces include raw USB HID
messages, RS-232, serial-over-USB, and other wireless interfaces. `libsigpad`
and the accompanying kernel driver implement only USB HID for the time being. 

Each packet begins with a command identifier and is followed by a set of
big-endian parameters.

## Packet reference

Backlight on:

Field      | Type      | Notes
-----------|-----------|--------------
Command    | `uint16_t`| Always `0xFF02`

Backlight off:

Field      | Type      | Notes
-----------|-----------|--------------
Command    | `uint16_t`| Always `0xFF03`

Bitmap blit:

Field      | Type      | Notes
-----------|-----------|--------------
Command    | `uint16_t`| Always `0xF207`
Mode       | `uint8_t` | `0x02` to overwrite the area; `0x03` for transparency
X position | `uint16_t`|
Y position | `uint16_t`|
Width      | `uint16_t`| Should probably be a multiple of 8
Height     | `uint16_t`| 
Bitmap     | `pixel[]` | Packed monochrome bitmap; (width * height / 8) bytes large

Rectangle clear:

Field      | Type      | Notes
-----------|-----------|--------------
Command    | `uint16_t`| Always `0xFF12`
Mode       | `uint8_t` | Set to 2 to clear, set to 1 to invert (?) region
X position | `uint16_t`| Measured from top-left corner
Y position | `uint16_t`|
Width      | `uint16_t`|
Height     | `uint16_t`|


## Notes on usage

Due to limits on HID packet length (24 bytes in this case), bitmaps in
particular need to be split across several packets, each with a separate
header. Delays between packets may be necessary to avoid data corruption,
requiring a display restart.

A screen clear can be achieved efficiently by filling a rectangle with mode set
to two, from (0, 0) to (width, height).

## Disclaimer

Information was obtained predominantly through fuzzing, trial-and-error, and
guesswork. Basic information is from the manufacturer website. Mode names were
from the public reference for the official driver API. It is likely the
protocol is similar for other models, although YMMV. I am not affiliated with
Topaz Systems.

Written by Alyssa Rosenzweig on 2017-02-17.
