#include <HardwareSerial.h>
#include <TM1637Display.h>

// Define pins for TM1637 display and RS485 communication
#define CLK 22
#define DIO 23
#define RS485TransmitReceive 25
#define RXD2 16
#define TXD2 17

// Create display and serial instances
TM1637Display display(CLK, DIO);
HardwareSerial MySerial(2);  

// Define pins for input and buffer sizes
const int pins[] = {26, 27, 32, 33, 39, 36, 34, 35};
const int numPins = 8; 
const int bufferSize = 64;

// Arrays for storing incoming and outgoing data
uint8_t Receiveddata[bufferSize];
uint8_t TotalDriverdata[bufferSize];
uint8_t DriverReceiveddata[bufferSize];
uint8_t DataforMaster[bufferSize];

// Variables for tracking data and states
uint8_t switchState = 0;
uint8_t bytesRead = 0;
uint8_t DriverbytesRead = 0;
uint8_t TotalbytesRead = 0;
uint8_t DataformasterBytes =0;

volatile bool dataComplete = false; 
volatile bool isValid = false; 
volatile bool AddressOK = false; 
volatile bool finalData = false; 
volatile bool DriverdataComplete = false; 

void setup() {
// Initialize Serial communication for debugging
  Serial.begin(115200);

  // Initialize and clear the TM1637 display
  display.setBrightness(0x0f); // Set brightness level
  display.clear(); // Clear the display

   // Set pin modes for input pins
  for (int i = 0; i < numPins ; i++)
  {
    pinMode(pins[i], INPUT);
  }
 
  // Initialize serial ports for communication
  Serial1.setPins(21,5);
  Serial1.begin(57600, SERIAL_8N1, 21, 5);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  // Set RS485 pin mode and ensure it's in receive mode
  pinMode(RS485TransmitReceive, OUTPUT);
  digitalWrite(RS485TransmitReceive, LOW); 

  // Wait for serial connection to be established
  while (!Serial) {
    delay(10);
  }
}

void loop() {
  
  //display.clear();
 // Display the current address on the TM1637 display
 uint8_t currentAddress = DisplayAddress();

 // Check if data is available from Serial1
   if (Serial1.available() && !dataComplete)  
   {
       ReceiveData();
    }


 // Process data if complete
  if(dataComplete)
        {
        bool isValid = validchecksum(Receiveddata,bytesRead);
        //Serial.println(isValid);
         if(isValid == 1) 
          {
           bool AddressOK =  AddressCheck(Receiveddata,currentAddress);
           //Serial.println(AddressOK);
            if(AddressOK == 1)
              {
               // Serial.println("removingaddress");
                removeSecondElement(Receiveddata,bytesRead);
                addNewCheckSum(Receiveddata,bytesRead);
                 finalData = CheckFinaldata(Receiveddata,bytesRead);              
              }
              if (finalData == 1)
              {
                SendDataToLEDDrivers(Receiveddata,bytesRead);
                delay(2000); //According to Data Sheet
                //If the dimming command or set max current command are used It can take up to 2 seconds 
                //for the current and voltage readings to move to their final state.

               // Check if data is available from Serial2
               if (Serial2.available() && !DriverdataComplete) 
                {
                  ReceiveFromDriver();
                 if(DriverdataComplete == 1)
                  {
                  comparefromdriverdata();
                  AddMasteraddress(DataforMaster,DataformasterBytes);
                  SenddatatoMaster(DataforMaster,DataformasterBytes);
                  }
                }
              }
          }     
        }
// Reset buffers and states for the next loop iteration
resetBuffers();
delay(1000);  // Delay to avoid overwhelming the serial communication
}

uint8_t DisplayAddress()
{
  switchState = 0;
  for (int i = 0; i < numPins; i++)
    {
    // Read each pin and shift its value to the correct bit position
    switchState |= digitalRead(pins[i]) << i; // Read each pin and shift its value to the correct bit position
    }
   // Invert switchState and display it on the TM1637
  switchState = ~switchState;
display.showNumberDecEx(switchState, 0 , false , 3 , 0);
return switchState;
}

void ReceiveData()
{
  while(Serial1.available()){

   uint8_t ch= Serial1.read();
    if(ch == 0x3A || bytesRead > 0)
    {
      Receiveddata[bytesRead] = ch;
      bytesRead++;
      ch = 0;
      if(bytesRead >= bufferSize)
      {
        // Buffer overflow handling
       // Serial.println("buffer overflow,resetting buffer");
        bytesRead = 0;
       }
      else if (Receiveddata[bytesRead-1] == 0x0A)
          {
           if (Receiveddata[bytesRead-2] == 0x0D)
            {
              //Serial.println("Received data:");
              //for (size_t i = 0; i < bytesRead; i++)
               // {
                 // Serial.println(Receiveddata[i], HEX);
                 // Serial.println(" "); 
               // }
               // End of message detected
                dataComplete = true;
                //Serial.println(bytesRead);
            }
        }
}
}
}



bool validchecksum(uint8_t array[], uint8_t bytesRead )
{  
   uint16_t sum = 0;
  // Calculate checksum by summing bytes from index 1 to (bytesRead - 4)
   for (size_t i = 1; i <= (bytesRead - 4) ; i++)
    {
    sum += array[i];
    //Serial.println(sum, HEX);
    }
  //Serial.print("Calculated Checksum: 0x");
  //Serial.println(sum, HEX);
  uint8_t expectedChecksum = array[bytesRead - 3];
 // Serial.print("Expected Checksum: 0x");
 // Serial.println(expectedChecksum, HEX);
  dataComplete = false;
  return (sum == expectedChecksum);
      
}


bool AddressCheck(uint8_t array[],uint8_t address)
{
  uint8_t DeviceAddress = 0; 
  int i = 0;
  if(array[i] == 0x3A)
    {
    DeviceAddress = array[i+1];
    }
  isValid = false;
  //Serial.println(DeviceAddress,HEX);
  //Serial.println(address, HEX);
  return (DeviceAddress == address);
}

void removeSecondElement(uint8_t array[], size_t length) 
    {
     // Serial.println(length);
    
    for (int i = 1; i <= length - 1 ; i++)
      {
      array[i] = array[i + 1];
     // Serial.println(array[i], HEX);
     // Serial.println(" "); 
     }
     length--;
     //Serial.println("Array after removal:");
    //for (size_t i = 0; i < length; i++) {
   // Serial.print(array[i], HEX);
    //Serial.print(" ");

   // }
    bytesRead = length;
    }

void addNewCheckSum(uint8_t array[], uint8_t bytesRead )
{
    
    uint16_t sum = 0;
    for (size_t i = 1; i <= (bytesRead - 4) ; i++)
    {
    sum += array[i];
    //Serial.println(sum, HEX);
    }
     array[bytesRead-3] = sum;
    // Serial.println("Final Data to LED DRIVER:");
    //for (size_t i = 0; i < bytesRead; i++) {
    //Serial.print(array[i], HEX);
    //Serial.print(" ");
    //}
}


bool validDataLength(uint8_t array[], uint8_t length) 
{
  if (length < 7) return false;  // Ensure minimum length

  uint8_t dataLength = array[3];
  // Calculate actual data length (total length - header - checksum - end markers)
  uint8_t actualDataLength = length - 7; // 3 header bytes + 1 data length byte + 1 checksum byte + 2 end marker bytes

  if (dataLength == actualDataLength) 
  {
    return true;
  } 
  else 
  {
    //Serial.print("Invalid data length: ");
   // Serial.print("Expected ");
    //Serial.print(dataLength);
    //Serial.print(", got ");
    //Serial.println(actualDataLength);
    return false;
  }
}

bool CheckFinaldata(uint8_t array[], uint8_t length)
{
  bool dataok = validDataLength(array,length);
  bool checksum = validchecksum(array,length);
  return (dataok && checksum);
}



void SendDataToLEDDrivers(uint8_t array[], uint8_t length)
{
 // Serial.write(array,length);
  //Serial.println("Final Data to LED DRIVER:");
 // for (size_t i = 0; i < length; i++) {
 // Serial.println(array[i], HEX);
  Serial2.write(array,length);
//}
}

void ReceiveFromDriver()
{
    //Serial.print("Received: ");
    while(Serial2.available()) 
    {
      // Read incoming data
      uint8_t ch = Serial2.read();
      // Print incoming data to the Serial monitor
      TotalDriverdata[TotalbytesRead] = ch;
      TotalbytesRead++;
      //Serial.println(ch,HEX);
      ch = 0;
    }
         // Serial.println("Received data:");
             // for (size_t i = 0; i < TotalbytesRead; i++)
              //  {
               //   Serial.println(TotalDriverdata[i], HEX);
               // Serial.println(" "); 
             //   }
        
      //Serial.print(incomingByte, HEX);
      //Serial.print(" ");
  //Serial.print("totalbytes are:");
  //Serial.println(TotalbytesRead); 
 DriverdataComplete = true;
}

void comparefromdriverdata() {
  //Serial.println("Enter in comparefolder");
    for(int i = 0; i < TotalbytesRead; i++)
    {
     // Serial.print("i= ");
      //Serial.println(i);
      uint8_t ch = TotalDriverdata[i];
        if (ch == 0x3A || DriverbytesRead > 0) 
        {
         DriverReceiveddata[DriverbytesRead] = ch;
          DriverbytesRead++;
          if (DriverReceiveddata[DriverbytesRead - 1] == 0x0A) {
          if (DriverReceiveddata[DriverbytesRead - 2] == 0x0D)
          {
          //Serial.println("End of message detected.");
            if (!isSameAsLastSent(Receiveddata, bytesRead, DriverReceiveddata,DriverbytesRead))
            {
            Serial.println("New data received:");
            for (size_t j = 0; j < DriverbytesRead; j++) 
            {
              DataforMaster[j] = DriverReceiveddata[j];
              //Serial.print(DriverReceiveddata[j], HEX);
              Serial.print(DataforMaster[j], HEX);   
              Serial.print(" ");
            }
            Serial.println();
          } 
          else 
          {
            Serial.println("Data matches sent data, discarding.");
          }
          DataformasterBytes = DriverbytesRead;
          Serial.println("DataformasterBytes");
          Serial.println(DataformasterBytes);

         DriverbytesRead = 0; // Reset after processing
        }
      }
    }
  }
  TotalbytesRead = 0;
}

bool isSameAsLastSent(uint8_t array1[],uint8_t length1,uint8_t array2[], uint8_t length2) 
{
  if (length1 != length2 ) {
    return false;
  }
  for (size_t i = 0; i < length1; i++) {
    if (array1[i] != array2[i]) {
      return false;
    }
  }
  return true;
}

void AddMasteraddress(uint8_t array[], size_t length) 
    {
     // Shift elements to the right
  for (int i = length; i > 1; i--) {
    array[i] = array[i - 1];
  }
  
  // Insert the new byte at the second position
  array[1] = 0xFF;
  
  // Increase the length of the array
  length++;
  DataformasterBytes = length;
}

void SenddatatoMaster(uint8_t array[], size_t length)
{
  digitalWrite(RS485TransmitReceive, HIGH);
  Serial1.write(array,length);
  Serial.println("Final Data to Master:");
  for (size_t i = 0; i < length; i++) {
  Serial.println(array[i], HEX);
  }
}

void resetBuffers() {
  memset(Receiveddata, 0, sizeof(Receiveddata));
  memset(TotalDriverdata, 0, sizeof(TotalDriverdata));
  memset(DriverReceiveddata, 0, sizeof(DriverReceiveddata));
  bytesRead = 0;
  DriverbytesRead = 0;
  TotalbytesRead = 0;
  DataformasterBytes = 0;
  dataComplete = false;
  DriverdataComplete = false;
  isValid = false;
  AddressOK = false;
  finalData = false;
  digitalWrite(RS485TransmitReceive, LOW);
}

