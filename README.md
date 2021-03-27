# ESC102 Prototype

An assistive object re-identification device for the visually impaired employing NFC ID and audio recording and playback.

# Development

## Libraries

- [WaveHC](https://github.com/adafruit/WaveHC) (see [setup](#setup) about pin number modifications)

## Setup

1. Purchase the parts and assemble the prototype as laid out in [`/hardware`](./hardware/README.md)
2. Download all [libraries](#libraries) and place them under your Arduino libraries folder, with **some modifications**:
   1. **WaveHC**
       - use the modified `WavePinDefs.h` from this repository
       - Set the **`DVOLUME`** macro (line 36 in `WaveHC.h`) to `1`

## Todo

- [ ] Test Code V1
  - [X] Check/fix pin conflicts

- [ ] Readme
  - [X] List libraries, modifications

- [ ] Hardware BOM and assembly notes