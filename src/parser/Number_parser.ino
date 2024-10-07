void Parse_Number(){
  //   // Check if data is available on the serial port
  // if (Serial.available()) {
  //   char receivedChar = Serial.read();

  //   // Build incoming data until CR (Carriage Return) or LF (Line Feed) is detected
  //   if (receivedChar == '\n' || receivedChar == '\r') {
  //     newDataAvailable = true;
  //   } else {
  //     incomingData += receivedChar;
  //   }
  // }

  // Process the incoming packet when new data is available
  if (newDataAvailable) {
    newDataAvailable = false;
    
    // Remove the colon (':') and parse the slave ID and brightness
    if (incomingData.startsWith(":")) {
      incomingData = incomingData.substring(1); // Remove the colon
      
      int commaIndex = incomingData.indexOf(',');
      if (commaIndex != -1) {
        slaveID = incomingData.substring(0, commaIndex).toInt();           // Get the slave ID
        brightness = incomingData.substring(commaIndex + 1).toInt();       // Get the brightness value
        
        // Display the parsed values
        Serial.print("Slave ID: ");
        Serial.println(slaveID);
        Serial.print("Brightness: ");
        Serial.println(brightness);
      } else {
        Serial.println("Invalid packet format, missing comma.");
      }
    } else {
      Serial.println("Invalid packet format, missing colon.");
    }

    // Clear the incoming data buffer for the next packet
    incomingData = "";
  }
}
