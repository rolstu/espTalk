// Function to create and send the packet
void createAndSendPacket(uint8_t slaveID, uint8_t brightness) {
  // Initialize packet array with fixed values
  uint8_t packet[9] = {
    0x3A,        // Byte 1: Header
    slaveID,     // Byte 2: Slave ID (argument)
    0x3C,        // Byte 3: Command (fixed)
    0x00,        // Byte 4: Offset (fixed)
    0x01,        // Byte 5: Length (fixed)
    brightness,  // Byte 6: Brightness data (argument)
    0x00,        // Byte 7: Sum (to be calculated)
    0x0D,        // Byte 8: End byte 1
    0x0A         // Byte 9: End byte 2
  };

  // Calculate the sum of bytes 2 to 6
  uint8_t sum = 0;
  for (int i = 1; i <= 5; i++) {  // Sum bytes 2 to 6
    sum += packet[i];
  }

  // Set the sum in Byte 7
  packet[6] = sum;

  // Send the packet over serial (for debugging)
  Serial.print("Sending packet: ");
  for (int i = 0; i < 9; i++) {
    Serial.print(packet[i], HEX);
    Serial.print(" ");
    Serial1.print(packet[i], HEX);
    Serial1.print(" ");
  }
  Serial.println();
  Serial1.println();

  // Send the packet over mySerial (UART1)
  // Serial1.print(packet, 9);  // Send the 9-byte packet
}
