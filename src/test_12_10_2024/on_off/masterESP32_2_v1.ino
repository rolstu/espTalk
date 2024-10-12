#include <HardwareSerial.h>

// Define pins for serial communication (replace with your actual pins)
#define MASTER_TX_PIN 21
#define MASTER_RX_PIN 5

// Define pins for RS485 communication (optional, replace with your actual pins)
#define RS485_TRANSMIT_RECEIVE 25

// Define buffer size for data transmission
const int bufferSize = 64;

// Define data structure for sending (modify according to your data format)
struct MyData {
  uint8_t address;
  uint8_t command;
  uint8_t checksum; // Add checksum calculation if needed
};

// Create HardwareSerial objects for communication
HardwareSerial masterSerial(1); // Replace 1 with the appropriate port number

// Function to get user input for slave ID and command
void getUserInput(MyData& data) {
  Serial.println("Enter slave ID (0-255):");
  while (!Serial.available()) {
    delay(10);
  }
  data.address = Serial.read() - '0'; // Convert ASCII to integer

  Serial.println("Enter command (1: Turn on lights, 0: Turn off lights):");
  while (!Serial.available()) {
    delay(10);
  }
  data.command = Serial.read() - '0';
}

void setup() {
  Serial.begin(115200); // Debug serial

  // Initialize master serial for communication with slave
  masterSerial.begin(57600, SERIAL_8N1, MASTER_TX_PIN, MASTER_RX_PIN);

  // Optional: Initialize RS485 pin (ensure LOW for receive mode)
  pinMode(RS485_TRANSMIT_RECEIVE, OUTPUT);
  digitalWrite(RS485_TRANSMIT_RECEIVE, LOW);
}

void loop() {
  MyData dataToSend;
  getUserInput(dataToSend);

  // Calculate checksum (optional)
  // ... (implement checksum calculation based on your data format)

  // Send data to the slave
  sendData(dataToSend);

  // Wait for and receive data from the slave
  MyData receivedData;
  if (receiveData(&receivedData)) {
    // Validate received data (modify validation logic as needed)
    if (validateData(receivedData)) {
      Serial.println("Received valid data:");
      Serial.print("Address: 0x");
      Serial.println(receivedData.address, HEX);
      Serial.print("Command: ");
      Serial.println(receivedData.command);
    } else {
      Serial.println("Received data is invalid.");
    }
  } else {
    Serial.println("Failed to receive data from slave.");
  }

  delay(2000); // Adjust delay as needed
}

// ... (other functions)
