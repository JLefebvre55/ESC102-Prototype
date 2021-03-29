# ESC102 Prototype

An assistive object re-identification device for the visually impaired employing NFC ID and audio recording and playback.

# Development

## Libraries

- [WaveHC](https://github.com/adafruit/WaveHC) (see [setup](#setup) about pin number modifications)
- [Adafruit PN532](https://github.com/adafruit/Adafruit-PN532)
- [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)

## Setup

1. Purchase the parts and assemble the prototype as laid out in [`/hardware`](./hardware/README.md)
2. Download all [libraries](#libraries) and place them under your Arduino libraries folder, with **some modifications**:
   1. **WaveHC**
       - use the modified `WavePinDefs.h` from this repository
       - Set the **`DVOLUME`** macro (line 36 in `WaveHC.h`) to `1`


## Todo

- [ ] Test Code/Setup
  - [X] Check/fix pin conflicts
  - [X] Play sound file
  - [X] Build registry from TXT
  - [X] Read/Write IDs from NFC tags
  - [ ] Play registry on NFC read
  - [ ] Create/update/delete files on SD
    - [ ] Registry updates
    - [ ] Audio file allocation, deletion, overwriting

- [ ] Actual Code
  - [X] Setup
    - [X] POST (this will evolve, of course)
    - [X] Build registry from SD file
  - [ ] Main Loop
    - [ ] Reading/Writing Tags
      - [ ] Read a tag
      - [ ] Check registry for tag ID
      - [ ] If exists, play audio from file
      - [ ] If conflict, write new non-conflicting ID to tag, (or if not exists) then:
    - [ ] Record audio
      - [ ] Prompt user to record audio
      - [ ] Record audio signal to wave object
      - [ ] Playback and confirm
      - [ ] Write wave object to file
      - [ ] Add filename and NFC ID to registry
      - [ ] Update registry file

- [ ] UI
  - [ ] Prompt/startup audio

- [ ] Readme
  - [X] List libraries, modifications
  - [X] Timeline
  - [ ] Testing procedures

- [ ] Hardware BOM and assembly notes