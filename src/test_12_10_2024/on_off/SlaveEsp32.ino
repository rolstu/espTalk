#include <HardwareSerial.h>
#include <TM1637Display.h>

// Define RS485 communication pins for the Slave
#define RS485TransmitReceive 25
#define RXD2 16
#define TXD2 17

// Define pin for controlling the light
#define lightPin 32  // Replace with the correct GPIO for the light

// TM1637 display pins (if you want to display something on the slave)
#define CLK 22
#define DIO 23

// Create an instance of HardwareSerial for RS485 communication
HardwareSerial RS485Serial(2);
TM1637Display display(CLK, DIO);

// Slave ID and received data
uint8_t slaveID = 1;
uint8_t receivedData[4];
uint8_t bytesRead = 0;
bool dataComplete = false;

// Function to control the light
void controlLight(uint8_t command, uint8_t brightness) {
  if (command == 1) {
    analogWrite(lightPin, brightness);  // Turn on light with brightness
    Serial.println("Light ON");
  } else if (command == 0) {
    analogWrite(lightPin, 0);  // Turn off light
    Serial.println("Light OFF");
  }
}

void setup() {
  // Initialize Serial and RS485
  Serial.begin(115200);
  RS485Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(lightPin, OUTPUT);
  pinMode(RS485TransmitReceive, OUTPUT);
  digitalWrite(RS485TransmitReceive, LOW);  // Set to receive mode

  // Initialize the TM1637 display
  display.setBrightness(0x0f);
  display.clear();
}

void loop() {
  // Receive data from Master ESP32
  if (RS485Serial.available() && !dataComplete) {
    receivedData[bytesRead++] = RS485Serial.read();

    if (bytesRead >= 4) {  // We expect 4 bytes
      if (receivedData[0] == 0x3A && receivedData[3] == 0x0A) {
        dataComplete = true;
      }
    }
  }

  // If data reception is complete, process it
  if (dataComplete) {
    if (receivedData[1] == slaveID) {  // Check if the command is for this slave
      uint8_t command = receivedData[2];
      uint8_t brightness = receivedData[3];
      controlLight(command, brightness);  // Control the light based on the command
    }

    // Reset buffer and variables for the next communication
    bytesRead = 0;
    dataComplete = false;
  }
}
