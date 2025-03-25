# espTalk
## Esp32 contacting each other
**1. Install ESP32 Board Support:**
    Ensure you have the ESP32 board support installed in the Arduino IDE.
    
**2. Locate the link:**
    Go to File > Preferences and add the following URL to the “Additional Board Manager URLs” field:
```bash
https://github.com/rolstu/espTalk/releases
```
3. Then go to Tools > Board > Board Manager and search for “ESP32,” then install it.
4. Then run <code>https://github.com/rolstu/espTalk/releases</code> for each and every ESP32 and retrieve the MAC addresses.
5. Fill up the MAC addreses of the slave ESP32 in the <code>https://github.com/rolstu/espTalk/releases</code>
6. Set up the Slave ESP's as well with <code>https://github.com/rolstu/espTalk/releases</code>
