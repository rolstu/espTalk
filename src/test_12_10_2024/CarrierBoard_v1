#include <HardwareSerial.h>

// Define pins for RS485 communication
#define RS485TransmitReceive 25
#define RXD2 16
#define TXD2 17

// RS485 communication setup
HardwareSerial MySerial(2);

// Buffers and flags
const int bufferSize = 64;
uint8_t DataToSend[bufferSize];
uint8_t DataReceived[bufferSize];
uint8_t bytesToSend = 0;
uint8_t bytesRead = 0;
volatile bool dataComplete = false;

// User-defined variables for slave ID and brightness
uint8_t slaveID = 0x01;
uint8_t brightness = 0xFF;  // Default full brightness (0xFF)

void setup() {
  // Initialize Serial Monitor for debugging and user input
  Serial.begin(115200);

  // Initialize RS485 communication
  MySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Set pin mode for RS485 transmit/receive control
  pinMode(RS485TransmitReceive, OUTPUT);

  // Set RS485 to receive mode by default
  digitalWrite(RS485TransmitReceive, LOW);

  // Print instruction for the user
  Serial.println("Enter commands to set slaveID and brightness:");
  Serial.println("Format: ID,Brightness (e.g., 2,128 sets slaveID to 2 and brightness to 128)");
}

void loop() {
  // Check for user input via the Serial Monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');  // Read input until newline
    processInput(input);  // Process the input from user
  }

  // Send data to the slave
  sendDataToSlave();

  // Wait for a response from the slave
  if (MySerial.available() && !dataComplete) {
    receiveData();
  }

  // Process the received data once it's complete
  if (dataComplete) {
    processReceivedData();
    dataComplete = false;
  }

  delay(2000);  // Wait before the next communication cycle
}

void processInput(String input) {
  // Parse the input for slave ID and brightness
  int commaIndex = input.indexOf(',');
  
  if (commaIndex != -1) {
    // Extract slave ID and brightness values from the input
    slaveID = input.substring(0, commaIndex).toInt();
    brightness = input.substring(commaIndex + 1).toInt();

    // Print confirmation
    Serial.print("Slave ID set to: ");
    Serial.println(slaveID);
    Serial.print("Brightness set to: ");
    Serial.println(brightness);
  } else {
    Serial.println("Invalid format! Use format: ID,Brightness");
  }
}

void prepareData() {
  // Prepare a data packet to send based on user input
  DataToSend[0] = 0x3A;      // Start byte
  DataToSend[1] = slaveID;   // Slave ID set by user
  DataToSend[2] = 0x10;      // Command or function byte
  DataToSend[3] = brightness;  // Brightness value set by user
  DataToSend[4] = 0xFF;      // Example data (can be changed as needed)
  DataToSend[5] = 0x0D;      // Carriage return
  DataToSend[6] = 0x0A;      // Line feed

  bytesToSend = 7;  // Total number of bytes to send
}

void sendDataToSlave() {
  // Set RS485 to transmit mode
  digitalWrite(RS485TransmitReceive, HIGH);

  // Prepare the data to send
  prepareData();

  // Send the data to the slave
  MySerial.write(DataToSend, bytesToSend);
  Serial.println("Data sent to slave:");

  for (int i = 0; i < bytesToSend; i++) {
    Serial.print(DataToSend[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Set RS485 to receive mode after sending
  digitalWrite(RS485TransmitReceive, LOW);
}

void receiveData() {
  // Read the incoming data from the slave
  while (MySerial.available()) {
    uint8_t ch = MySerial.read();
    DataReceived[bytesRead] = ch;
    bytesRead++;

    // Check for end of message (0x0D 0x0A)
    if (DataReceived[bytesRead - 1] == 0x0A && DataReceived[bytesRead - 2] == 0x0D) {
      dataComplete = true;
      break;
    }

    // Reset buffer if overflow occurs
    if (bytesRead >= bufferSize) {
      bytesRead = 0;
    }
  }
}

void processReceivedData() {
  // Print the received data
  Serial.println("Data received from slave:");

  for (int i = 0; i < bytesRead; i++) {
    Serial.print(DataReceived[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Reset buffer and byte count
  bytesRead = 0;
}
