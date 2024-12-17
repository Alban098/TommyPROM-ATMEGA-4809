# TommyPROM - An Arduino-based EEPROM programmer - Nano Every Port

## Port of the original code to Nano Every with ATMEGA 4809 chip, (only for 28C family of Chips !)

Control pinout change from original (This is made to preserve internal masks applied when doing port manipulation, Changed from **Nano v3 PORTC** to **Nano Every PORTD**) :
* **WE** from **A0** to **A3**
* **CE** from **A1** to **A2**
* **OE** from **A2** to **A1**
* **CLK_HI** from **A3** to **A0**
* **CLK_LO** from **A4** to **A6**
* **AD-BIT** from **A5** to **A7**

Data pinout changed from original (Port mapping has been updated) :
* **D2** to **D2**
* **D3** to **D7**
* **D4** to **A4**
* **D5** to **A5**
* **D6** to **D9**
* **D7** to **D10**
* **D8** to **D5**
* **D9** to **D3**

This weird non-consecutive pinout is to use the least possible amount of ports when doing direct port manipulation, DATA is now from MSB to LSB :
* Port F's 5th bit
* Port B's 3 least significant bits
* Port A's 4 least significant bits 

! Support for more than 16 bits address has not been ported !

This fork also include an extension to program [this project](https://github.com/Alban098/Breadboard-CPU-Emulator), by interfacing itself with the Manual Programming switches
(More detail in the dedicated repo)

[Original Project](https://github.com/TomNisbet/TommyPROM)
