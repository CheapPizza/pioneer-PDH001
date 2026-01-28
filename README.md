# pioneer-PDH001
Program a Pioneer PDH001 EEPROM to unlock a code locked KEX-900/KEX-500/DEX-77 head unit. This will also clear the 3 hour lockout activated by entering an incorrect pin 3 times.

## Requirements
- Any PC capable of programming an Arduino
- An Arduino board (Uno, Nano, etc.)
- Arduino IDE
- A Pioneer PDH001 or a Xicor X24C44 EEPROM chip
- A way to connect the EEPROM to the Arduino (e.g., breadboard, jumper wires, etc.)

## Instructions

### Before you start:
- I higly recommend soldering a DIP-8 socket to the head unit so you do not have to resolder the EEPROM each time you want to program it.
- I am not responsible for you bricking the EEPROM by connecting it incorrectly.
- This has been tested to work on a KEX-900 cassette deck. It should work on other Pioneer head units using the same PDH001 EEPROM as well.
- I have not tested programming with a programming clip. Use at your own risk.
- Look up the X24C44 datasheet to help with pinout.
- If you want to be safe buy X24C44 chips from AliexPress/eBay to test. They are identical to the PDH001 and much cheaper.

### How to program
1. Connect the EEPROM to the Arduino according to the pinout below. PDH001 and X24C44 have the same pinout.
2. Clone the repository or download the .ino file.
3. Open the .ino file in the Arduino IDE.
4. Plug in your Arduino board to your PC.
5. Open the Serial Monitor in the Arduino IDE (set baud rate to 9600).
6. Upload the sketch to the Arduino board.
7. Verify you can see the output so you know the program executed correctly.
8. Power off the Arduino.
9. Done, after reinstalling the EEPROM the code is now 7777.
10. (Optional) The pin can now be set by holding volume + and - after powering on the unit. 0000 should remove the code entirely.

Note: If you did not see the output you can restart the Arduino by pressing the reset button or unplugging and plugging it back in.

## Pinout and connections

| EEPROM Pin | Function | Arduino Pin | Notes |
|------------|----------|-------------|-------|
| 1 | CE (Chip Enable) | Pin 8 | |
| 2 | SK (Serial Clock) | Pin 9 | |
| 3 | DI (Data In) | Pin 10 | |
| 4 | DO (Data Out) | Pin 11 | |
| 5 | Vss (GND) | GND | |
| 6 | RECALL (active low) | - | Pull up to Vcc with 10kΩ resistor |
| 7 | STORE (active low) | - | Pull up to Vcc with 10kΩ resistor |
| 8 | Vcc (+5V) | +5V | |

**Note:** Pin 6 (RECALL) and Pin 7 (STORE) should be pulled up to Vcc (+5V) by placing a 10k ohm resistor between each pin and Vcc. This is because the code does the recall and store operations. Leaving these inputs disconnected may lead to unpredictable behavior.

# Development
The program works for writing but reading is not implemented yet.  
The read data seems bit shifted and I haven't figured out why.  
As the main purpose of this program is to unlock the head unit I feel no need to spend more time on this additional feature.  
I do not own the unit (KEX-900) this was tested on and will have to send it back to it's owner so I am unable to test further.  
If you do figure it out feel free to create a pull request!

# Credits
The code system uses a simple mapping for the pin. The decoding logic and other related useful information can be found here:
[Turbo Dodge forum](https://www.turbododge.com/threads/resetting-code-on-pioneer-dex-77.137295/page-2)
