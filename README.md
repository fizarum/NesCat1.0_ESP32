# NesCat1.0_ESP32 modification

Modified version of NesCat1.0_ESP32 system(original: <https://github.com/nathalis/NesCat1.0_ESP32>).

!!! **Its in progress and most features not working at this moment.** !!!

I want to make separate releases as soon as I get stable version of my mod.
Currently I'm working on software changes, but PCB reworking is planned as well

Planned changes:

- ~~move from SD7789 to ILI9341 display~~ done
- ~~add PCF8574N for keyboard (keys)~~ added for ABCD buttons
- ~~move to 16 channel gpio expander~~ implemented by MCP23017
- add charger module
- change to Max98357A I2S module

Required libraries:

- Adafruit_BusIO <https://github.com/adafruit/Adafruit_BusIO.git>
- Adafruit_GFX <https://github.com/adafruit/Adafruit-GFX-Library>
- Adafruit_ILI9341 <https://github.com/adafruit/Adafruit_ILI9341>
- ~~SdFat <https://github.com/greiman/SdFat>~~
- ~~arduinoFFT <https://github.com/kosme/arduinoFFT>~~
- ESP32-audioI2S <https://github.com/schreibfaul1/ESP32-audioI2S>

## Pinout

### Display

- TFT_CLK 18
- TFT_COPI 23
- TFT_CIPO 19
- TFT_CS 5
- TFT_DC 16
- TFT_RST 17

### SD

- SD_CS 34
- SD_CLK 18
- SD_COPI 23
- SD_CIPO 19

### Keyboard

- KB_CLK 22
- KB_SDA 21

### Sound

- I2S_DOUT 25
- I2S_BCLK 27
- I2S_LRC 26
