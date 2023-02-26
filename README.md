# bthome_nrf_door
BTHome door sensor using the nRF52

This project implements a [BTHome](https://bthome.io/) door sensor with nRF52.

It assumes that a digital signal is connected to GPIO0.2 that is 1 when the door is
closed and 0 when the door is open.

# Building

You need the [nRF Connect SDK](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/index.html) to build this sample.

Clone this repo in a folder under the samples folder in the nRF Connect SDK.
To build for the nRF52840 DK run the west build command:

```shell
west build -b nrf52840dk_nrf52840
```

To build for other devices/boards replace nrf52840dk_nrf52840 with the correct one.
