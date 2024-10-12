# ESP32 RS485 Master-Slave Communication

This project demonstrates RS485 communication between two ESP32 devices, where one acts as a master and the other as a slave. The master sends commands to the slave to control a light’s brightness or turn it on/off. 

### Requirements:
- Two ESP32 boards (Master and Slave)
- RS485 transceivers
- TM1637 display (optional, for Slave)
- LED or light connected to the Slave ESP32

### Master Code Overview (`MasterEsp32.ino`):
- Sends commands over RS485 to control the slave device.
- Accepts user input via the Serial Monitor for controlling the slave's light:
  - `1` to turn the light ON.
  - `0` to turn the light OFF.
  - `2,brightness` to adjust the brightness (e.g., `2,128` for 50% brightness).
- Pin Configuration:
  - **RS485TransmitReceive (GPIO 25):** RS485 transmit/receive control.
  - **RXD2 (GPIO 16):** RS485 receive pin.
  - **TXD2 (GPIO 17):** RS485 transmit pin.

### Slave Code Overview (`SlaveEsp32.ino`):
- Receives commands from the master over RS485.
- Controls an LED or light based on the command received:
  - `1`: Turn ON.
  - `0`: Turn OFF.
  - `2`: Set brightness (0-255).
- TM1637 display can optionally show data like brightness (optional feature).
- Pin Configuration:
  - **RS485TransmitReceive (GPIO 25):** RS485 transmit/receive control.
  - **RXD2 (GPIO 16):** RS485 receive pin.
  - **TXD2 (GPIO 17):** RS485 transmit pin.
  - **lightPin (GPIO 32):** Control pin for the light.
  - **CLK (GPIO 22)** and **DIO (GPIO 23):** Pins for the TM1637 display.

### How It Works:
1. **Master:**
   - The user inputs commands through the Serial Monitor:
     - `1`: Turn ON the light.
     - `0`: Turn OFF the light.
     - `2,brightness`: Set the brightness level.
   - The master sends the command along with the slave ID and brightness value over RS485.
   
2. **Slave:**
   - The slave listens for incoming commands from the master.
   - Once a command is received, the slave checks if it's addressed to its own ID, and then processes the command to control the light.
   - The TM1637 display (if connected) can be used to display status or brightness values.

### Example Commands:
- **Turn ON light:** `1`
- **Turn OFF light:** `0`
- **Set brightness to 128 (50%):** `2,128`

### Wiring:
- Connect TX, RX, and RS485 transmit/receive control pins between the RS485 transceivers and both ESP32 boards.
- Connect the light or LED to the `lightPin` on the Slave ESP32.
- Optionally connect a TM1637 display to the Slave.

### Usage:
1. Upload the `MasterEsp32.ino` to the Master ESP32.
2. Upload the `SlaveEsp32.ino` to the Slave ESP32.
3. Open the Serial Monitor on the Master and input commands to control the Slave’s light.
