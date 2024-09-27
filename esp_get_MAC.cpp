#include <WiFi.h>
#include <esp_wifi.h>

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);  // Read the MAC address in Station mode
  if (ret == ESP_OK) {
    // Print MAC address in standard format
    Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup(){
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate

  WiFi.mode(WIFI_STA);  // Set ESP32 to station mode
  WiFi.disconnect(true);  // Disconnect from any previous WiFi connections

  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();  // Read and print the MAC address
}

void loop(){
  // Optional: If you want to continuously print the MAC address
  delay(5000);  // Wait for 5 seconds before repeating (if needed)
  Serial.print("[LOOP] ESP32 Board MAC Address: ");
  readMacAddress();
}
