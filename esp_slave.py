#include <esp_now.h>
#include <WiFi.h>

// Data structure to receive the message
typedef struct struct_message {
  char message[32];
} struct_message;

struct_message incomingData;

void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingData, incomingData, sizeof(incomingData));
  Serial.print("Received message: ");
  Serial.println(incomingData->message);
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

  // Register receive callback
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Do nothing here, waiting for messages
}
