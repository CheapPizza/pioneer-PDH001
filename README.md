# pioneer-PDH001
Program a Pioneer PDH001 EEPROM to unlock a code locked KEX-900/KEX-500/DEX-77 head unit.

## Requirements
- Any PC capable of programming an Arduino
- An Arduino board (Uno, Nano, etc.)
- Arduino IDE
- A Pioneer PDH001 or a Xicor X24C44 EEPROM chip
- A way to connect the EEPROM to the Arduino (e.g., breadboard, jumper wires, etc.)

## Instructions
1. Connect the EEPROM to the Arduino according to the pinout below. PDH001 and X24C44 have the same pinout.
2. Clone the repository or download the .ino file.
3. Open the .ino file in the Arduino IDE.
4. Plug in your Arduino board to your PC.
5. Open the Serial Monitor in the Arduino IDE (set baud rate to 9600).
6. Upload the sketch to the Arduino board.
7. Verify you can see the output so you know the program executed correctly.
8. Power off the Arduino.
9. Done!

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
