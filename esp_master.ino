#include <esp_now.h>
#include <WiFi.h>

// Define the MAC addresses of the slaves (these should be unique for each slave)
uint8_t slave1Address[] = {0x24, 0x6F, 0x28, 0x12, 0x34, 0x56};  // Replace with actual MAC
uint8_t slave2Address[] = {0x24, 0x6F, 0x28, 0x12, 0x34, 0x57};  // Replace with actual MAC

// Data structure to send to the slaves
typedef struct struct_message {
  char message[32];
} struct_message;

struct_message myData;

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(onSent);

  // Add Slave 1
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, slave1Address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add slave 1");
    return;
  }

  // Add Slave 2
  memcpy(peerInfo.peer_addr, slave2Address, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add slave 2");
    return;
  }
}

void loop() {
  // Prepare message
  strcpy(myData.message, "Hello from Master!");

  // Send message to Slave 1
  esp_now_send(slave1Address, (uint8_t *) &myData, sizeof(myData));
  delay(1000);  // 1 second delay before sending next

  // Send message to Slave 2
  esp_now_send(slave2Address, (uint8_t *) &myData, sizeof(myData));
  delay(1000);
}
