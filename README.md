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

- [ ] Code
  - [X] Check/fix pin conflicts
  - [X] Build Registry
    - [X] Read all files in root directory, filter for WAVs, append to linked list (global length int)
  - [ ] Save/Restore Registry to file on SD (might deprecate that first step eh?)
  - [ ] Read IDs from NFTs
  - [ ] etc.

- [ ] Readme
  - [X] List libraries, modifications

- [ ] Hardware BOM and assembly notes