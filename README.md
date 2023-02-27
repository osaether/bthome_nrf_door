# bthome_nrf_door
BTHome door sensor using the nRF52840

This project implements a [BTHome](https://bthome.io/) door sensor with nRF52840.

It assumes that a (NC) relay is connected between GPIO0.2 and GPIO0.26 of the nRF52.


# Building

You need the [nRF Connect SDK](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/index.html) to build this sample.

Clone this repo in a folder under the samples folder in the nRF Connect SDK.
To build for the nRF52840 DK run the west build command:

```shell
west build -b nrf52840dk_nrf52840
```

To build for other devices/boards replace nrf52840dk_nrf52840 with the correct one.
