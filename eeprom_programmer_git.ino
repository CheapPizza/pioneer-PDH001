

#define CE 8
#define SK 9
#define DI 10
#define DO 11

void setup() {
  // Chip enable
  pinMode(CE, OUTPUT);
  digitalWrite(CE, HIGH);

  // Serial clock
  pinMode(SK, OUTPUT);
  digitalWrite(SK, LOW);

  // Data in (to EEPROM)
  pinMode(DI, OUTPUT);
  digitalWrite(DI, LOW);

  // Data out (from EEPROM)
  pinMode(DO, INPUT);

  Serial.begin(9600);
  Serial.println("Started");

  int address = 0b0000;
  // int data = 0b1000100010001000; // This should theoretically set the pin to 0000 = disabled, but does not work
  int data = 0b0001000100010001; // Sets the pin to 7777 (writes 1111). See decoding table below.

  // Decoding table for pin
  // https://www.turbododge.com/threads/resetting-code-on-pioneer-dex-77.137295/page-2
  // Memory	Pin
  // 8	    0
  // 4	    1
  // C	    2
  // 2	    3
  // A	    4
  // 6	    5
  // E	    6
  // 1	    7
  // 9	    8
  // 5	    9

  sendRCL();
  sendWREN();
  // Wiping memory
  for (int i = 0; i <= 15; i++) {
    writeWord(i, 0);
  }
  // Writing known PIN (7777)
  writeWord(address, data);
  sendSTO();
}

void loop() {}

// Send a single bit, with clock pulse
void sendBit(uint8_t bit) {
  delay(1);
  digitalWrite(DI, bit ?  HIGH : LOW);
  delay(1);
  digitalWrite(SK, HIGH);
  delay(1);
  digitalWrite(SK, LOW);
  delay(1);
}

// Receive a single bit from DO, with clock pulse
uint8_t receiveBit() {
  digitalWrite(SK, HIGH);
  delay(1);
  uint8_t bit = digitalRead(DO);
  digitalWrite(SK, LOW);
  delay(1);
  return bit;
}

// Send recall instruction (1XXXX101)
void sendRCL() {
  digitalWrite(CE, HIGH);
  delay(1);
  
  // Send instruction: 10000101
  sendBit(1);  // Start bit
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(1);  // Opcode bit
  sendBit(0);  // Opcode bit
  sendBit(1);  // Opcode bit (RCL = 101)
  
  digitalWrite(CE, LOW);
  delay(1);
}

// Send write enable instruction (1XXXX100)
void sendWREN() {
  digitalWrite(CE, HIGH);
  delay(1);
  
  // Send instruction:  10000100
  sendBit(1);  // Start bit
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(1);  // Opcode bit
  sendBit(0);  // Opcode bit
  sendBit(0);  // Opcode bit (WREN = 100)
  
  digitalWrite(CE, LOW);  // Reset instruction register
  delay(1);
}

// Send write disable instruction (1XXXX000)
void sendWRDS() {
  digitalWrite(CE, HIGH);
  delay(1);
  
  // Send instruction: 10000000
  sendBit(1);  // Start bit
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Opcode bit
  sendBit(0);  // Opcode bit
  sendBit(0);  // Opcode bit (WRDS = 000)
  
  digitalWrite(CE, LOW);
  delay(1);
}

// Send store instruction (1XXXX001)
void sendSTO() {
  digitalWrite(CE, HIGH);
  delay(1);
  
  // Send instruction: 10000001
  sendBit(1);  // Start bit
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Don't care
  sendBit(0);  // Opcode bit
  sendBit(0);  // Opcode bit
  sendBit(1);  // Opcode bit (STO = 001)
  
  digitalWrite(CE, LOW);
  delay(1);
}

// Write 16-bit word to address
// Instruction format: 1AAAA011 followed by 16 data bits
void writeWord(uint8_t address, uint16_t data) {
  digitalWrite(CE, HIGH);
  
  // Send instruction byte:  1AAAA011
  sendBit(1);  // Start bit
  
  // Send 4-bit address (MSB first)
  sendBit((address >> 3) & 0x01);
  sendBit((address >> 2) & 0x01);
  sendBit((address >> 1) & 0x01);
  sendBit(address & 0x01);
  
  // Send opcode 011 (WRITE)
  sendBit(0);
  sendBit(1);
  sendBit(1);
  
  delay(1);
  
  // Send 16-bit data (LSB first: D0 to D15)
  Serial.println("Writing data");
  for (int i = 0; i <= 15; i++) {
    int bit = (data >> i) & 0x01;
    sendBit(bit);
    Serial.print(bit);
  }
  Serial.println();
  Serial.println("Write done");

  digitalWrite(CE, LOW);  // Complete write operation
  delayMicroseconds(1);
  
  // Wait for internal write cycle to complete (up to 5ms typical)
  delay(10);
}

// Note: This is not working as I would excpect and the output is wrong
// // Read 16-bit word from address
// // Instruction format: 1AAAA11X followed by reading 16 data bits
// uint16_t readWord(uint8_t address) {
//   uint16_t data = 0;
  
//   digitalWrite(CE, HIGH);
//   delayMicroseconds(1);
  
//   // Send instruction byte: 1AAAA110
//   sendBit(1);  // Start bit
  
//   // Send 4-bit address (MSB first)
//   sendBit((address >> 3) & 0x01);
//   sendBit((address >> 2) & 0x01);
//   sendBit((address >> 1) & 0x01);
//   sendBit(address & 0x01);
  
//   // Send opcode 110 (READ)
//   sendBit(1);
//   sendBit(1);
//   sendBit(0);
  
//   // Generate the "don't care" bit clock (SK #8)
//   delay(1);
//   digitalWrite(SK, HIGH);
//   delay(1);
//   digitalWrite(SK, LOW);
//   delay(1);
  
//   // D₀ is now valid on DO line (clocked by falling edge of SK #8)
//   // Read it BEFORE generating the next clock
//   uint8_t bit0 = digitalRead(DO);
//   data |= (bit0 << 0);
//   Serial.println("Reading");
//   Serial.print(bit0);
  
//   // Read remaining 15 bits (D1 to D15) with clock pulses
//   for (int i = 1; i <= 15; i++) {
//     uint8_t bit = receiveBit();
//     data |= (bit << i);
//     Serial.print(bit);
//   }
//   Serial.println();
//   Serial.println("Done reading");
  
//   digitalWrite(CE, LOW);
//   delayMicroseconds(1);
  
//   return data;
// }