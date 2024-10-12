#include <HardwareSerial.h>

// Define RS485 communication pins for the Master
#define RS485TransmitReceive 25
#define RXD2 16
#define TXD2 17

// Create an instance of HardwareSerial for RS485 communication
HardwareSerial RS485Serial(2);

// Variables for controlling the light on the slave
uint8_t slaveID = 1;
uint8_t brightness = 255;  // Max brightness
char command;

// Function to send command to Slave ESP32 via RS485
void sendCommand(uint8_t slaveID, uint8_t command, uint8_t brightness) {
  digitalWrite(RS485TransmitReceive, HIGH);  // Set RS485 to transmit mode
  delay(10);

  RS485Serial.write(0x3A);        // Start byte
  RS485Serial.write(slaveID);     // Slave ID
  RS485Serial.write(command);     // Command (1: ON, 0: OFF, 2: Set Brightness)
  RS485Serial.write(brightness);  // Brightness value (0-255)

  RS485Serial.write(0x0D);  // Carriage return
  RS485Serial.write(0x0A);  // Line feed

  delay(10);
  digitalWrite(RS485TransmitReceive, LOW);  // Set RS485 to receive mode
}

void setup() {
  // Initialize Serial for debugging and RS485 communication
  Serial.begin(115200);
  RS485Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(RS485TransmitReceive, OUTPUT);
  digitalWrite(RS485TransmitReceive, LOW);  // Set to receive mode initially

  Serial.println("Enter commands: [1] Turn ON, [0] Turn OFF, [2] Set Brightness");
}

void loop() {
  // Check if a new command is received from the serial monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input.length() > 0) {
      if (input[0] == '1' || input[0] == '0') {
        // Turn ON (1) or OFF (0) lights
        command = input[0] - '0';  // Convert character to int
        sendCommand(slaveID, command, brightness);
        Serial.println((command == 1) ? "Turn ON command sent" : "Turn OFF command sent");
      } else if (input[0] == '2') {
        // Set Brightness (Format: 2,128)
        int commaIndex = input.indexOf(',');
        if (commaIndex != -1) {
          brightness = input.substring(commaIndex + 1).toInt();
          sendCommand(slaveID, 2, brightness);
          Serial.println("Brightness set to " + String(brightness));
        }
      } else {
        Serial.println("Invalid command. Use [1], [0], or [2,brightness]");
      }
    }
  }
}
