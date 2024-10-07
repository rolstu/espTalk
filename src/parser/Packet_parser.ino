#include <HardwareSerial.h>

HardwareSerial mySerial(1);  // Using UART1, you can choose another one if needed

int slaveID = 0;
int brightness = 0;
String incomingData = "";
bool newDataAvailable = false;

void setup() {
  Serial.begin(115200);  // Serial Monitor
  //mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX pin = GPIO16, TX pin = GPIO17 (adjust if needed)

  Serial1.setPins(21, 5);
  Serial1.begin(57600, SERIAL_8N1, 21, 5);

  Serial.println("ESP32 Ready for parsing...");
}

void loop() {

  // Check if data is available on the serial port
  if (Serial.available()) {
    char receivedChar = Serial.read();

    // Build incoming data until CR (Carriage Return) or LF (Line Feed) is detected
    if (receivedChar == '\n' || receivedChar == '\r') {
      newDataAvailable = true;
    } else {
      incomingData += receivedChar;
    }
  }
  if (newDataAvailable) {
    Parse_Number();
    createAndSendPacket(slaveID, brightness);
  }
}
