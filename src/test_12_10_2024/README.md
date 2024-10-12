# Carrier Board to LEDUART

### Instructions for CarrierBoard_vi.ino

#### Key Features:

- **User Input via Serial Monitor:**
  - You can enter the slave ID and brightness by typing in the format: `ID,Brightness` (e.g., `2,128`).
  - The slave ID can range from `1` to `255`, and brightness from `0` to `255`.

- **Data Packet Preparation:**
  - The `prepareData()` function assembles the data packet with the start byte (`0x3A`), the slave ID, the brightness, and other fixed bytes (`0x0D 0x0A` for carriage return and line feed).

- **RS485 Communication:**
  - The master sends data to the slave over RS485 and waits for the slave to respond.
  - It toggles between transmit and receive modes by controlling the `RS485TransmitReceive` pin (GPIO 25).

- **Receive and Process Data:**
  - When the master receives a message from the slave, it checks for the end-of-message markers (`0x0D 0x0A`) and prints the received data to the Serial Monitor.

#### How It Works:

1. After uploading the code, open the Serial Monitor at a baud rate of `115200`.
2. Type commands in the format `ID,Brightness` (e.g., `2,128` sets slave ID to `2` and brightness to `128`).
3. The master sends the command to the slave and waits for a response.
4. This code allows you to dynamically adjust the slave ID and brightness level based on user input, making the master more flexible in controlling the slave devices.

#### Example of Brightness Values:

- `0`: Lights are completely off.
- `64`: Lights are at 25% brightness.
- `128`: Lights are at 50% brightness (medium brightness).
- `192`: Lights are at 75% brightness.
- `255`: Lights are at full brightness (maximum intensity).

### Error Handling:

If you come across this error:

```bash
Connecting....Traceback (most recent call last): 
  File "esptool.py", line 37, in <module>
  File "esptool\__init__.py", line 1064, in _main
  File "esptool\__init__.py", line 859, in main
  File "esptool\cmds.py", line 466, in write_flash
  File "esptool\util.py", line 37, in flash_size_bytes
TypeError: argument of type 'NoneType' is not iterable
[21384] Faile
Chip is ESP32 in Secure Download Mode
WARNING: Stub loader is not supported in Secure Download Mode, setting --no-stub
Changing baud rate to 921600
Changed.
Enabling default SPI flash mode...
Configuring flash size...
d to execute script 'esptool' due to unhandled exception!
```

The error you are encountering suggests that the ESP32 is in Secure Download Mode, which might prevent the ESP from being programmed normally. Here's how you can troubleshoot and resolve this:

#### Steps to Resolve:

1. **Check Boot Mode Pins:**
   - Ensure that GPIO0 (BOOT pin) is not held low during normal operation. It's typically used to enter programming mode.
   - If it's in secure boot mode by accident, try resetting the board by removing and reconnecting power or pressing the reset button.

2. **Try Manual Flash Mode:**
   - Hold the BOOT button on the ESP32, and while holding it, initiate the flash process.
   - Release the button once the flashing starts.

3. **Use the `--no-stub` Option:**
   - Since the warning mentions "Stub loader is not supported in Secure Download Mode," you can try passing the `--no-stub` option to the esptool command to disable the stub loader. The command should look something like:
     ```bash
     esptool.py --port COMx --baud 921600 write_flash --no-stub 0x1000 firmware.bin
     ```
     Replace `COMx` with the correct port and `firmware.bin` with the actual firmware file.

4. **Check Flash Size:**
   - The error also mentions NoneType for flash size, which suggests the flash size might not be detected. Ensure you're specifying the correct flash size for the ESP32 by using the `--flash_size` argument, like this:
     ```bash
     esptool.py --port COMx --baud 921600 write_flash --flash_size 4MB 0x1000 firmware.bin
     ```
     The typical flash size for an ESP32 is 4MB, but check the specifications for your specific board.

5. **Ensure Secure Boot is Disabled:**
   - If Secure Boot has been enabled unintentionally, it will restrict normal flashing. Secure boot can be managed via the ESP32's efuse system, but this requires additional steps to disable it.

6. **Power-Cycle the Board:**
   - Sometimes, the ESP32 might get stuck in a weird state. Unplugging and re-plugging the power supply or using the reset button might help.
