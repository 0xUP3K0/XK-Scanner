# XK-Scanner

A WiFi and Bluetooth Low Energy (BLE) scanner designed for the LilyGo 4.7 Inch ESP32-S3 Development Board with E-Ink display.

## Overview

The XK-Scanner scans for nearby WiFi networks and Bluetooth devices, displaying the results on a 4.7" E-Ink display. Users can switch between scanning modes or perform a re-scan using button interactions.

## Features

- **Dual Scanning Modes:**
  - WiFi network scanning with encryption type detection
  - Bluetooth Low Energy (BLE) device scanning with distance estimation
- **E-Ink Display Output:** Clear, readable results on 4.7" display
- **Interactive Controls:** Button-based mode switching and rescanning
- **Signal Quality Assessment:** RSSI-based quality indicators
- **Distance Estimation:** BLE device distance calculation using path loss

## Hardware Requirements

- LilyGo 4.7 Inch ESP32-S3 Development Driver Board
- Button connected to `BUTTON_1` pin

## Dependencies

### Libraries Required
```cpp
#include <Arduino.h>
#include "epd_driver.h"
#include "FiraSans.h"
#include "fonts/FiraSansBook12pt.h"
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "Button2.h"
```

### Installation
- For the detailed installation, please refer to the official LilyGo Guide and the delivered Guide.

## Usage

### Controls
- **Double-Click Button:** Switch between WiFi and BLE scanning modes
- **Long-Press Button:** Perform rescan in current mode

### Scanning Modes

#### 1. WiFi Scanner (Default Mode)
- Scans for available WiFi networks
- Displays:
  - Network numbering
  - SSID
  - Signal strength (RSSI) with quality indicator
  - Encryption type

**Signal Quality Indicators:**
- **Good:** ≥ -60 dBm
- **Mid:** -60 to -80 dBm  
- **Bad:** < -80 dBm

#### 2. BLE Scanner
- Scans for Bluetooth devices
- Displays:
  - Device numbering
  - Device name or MAC address
  - Signal strength (RSSI) with quality indicator
  - Estimated distance in meters

**Signal Quality Indicators:**
- **Excellent:** ≥ -60 dBm
- **Average:** -60 to -80 dBm
- **Poor:** < -80 dBm

## Configuration

### Adjustable Parameters

```cpp
// BLE scan duration (seconds)
int bleScanTime = 5;

// Path Loss Exponent for distance calculation
// Indoor office environment: 4.0-10.0
float path_loss = 10;
```

### Distance Calculation
BLE device distance is estimated using the formula:
```
distance = 10^((TX_Power - RSSI) / (10 * path_loss_exponent))
```

## Display Features

- **Automatic Pagination:** Results span multiple screens if needed
- **Custom Fonts:** Uses FiraSans fonts for clear readability
- **Status Messages:** Real-time scanning progress and completion indicators
- **Scan Duration:** Shows time taken for each scan operation

## Installation & Setup

1. **Hardware Setup:**
   - Connect the LilyGo ESP32-S3 board to your computer
   - Ensure the E-Ink display is properly connected

2. **Software Setup:**
   - Open Arduino IDE
   - Install required libraries (see Dependencies section)
   - Load the XK-Scanner code
   - Select the correct board and port
   - Upload the code

3. **First Run:**
   - The device will show a welcome screen with usage instructions
   - Press the button to start scanning (default: Wi-Fi Scan)

## Code Structure

### Main Functions
- `setup()`: Initialize display, WiFi, BLE, and button handlers
- `loop()`: Handle button interactions
- `wifiScan()`: Perform WiFi network scanning and display results
- `bluetoothScan()`: Perform BLE device scanning and display results
- `displayInfo()`: Route to appropriate scanning function based on mode

### Button Handlers
- `handler()`: Process double-click events for mode switching
- `rescan()`: Process long-press events for rescanning

### Display Functions
- `writelnWithSpacing()`: Display text with proper formatting
- `writelnWithSpacingStart()`: Display startup text with different formatting

## Troubleshooting

### Common Issues
1. **Display not working:** Check E-Ink display connections and driver installation
2. **Button not responding:** Verify button wiring to `BUTTON_1` pin
3. **BLE scan fails:** Ensure BLE is properly initialized and device supports BLE
4. **Memory allocation errors:** Check available heap memory

### Serial Monitor
Enable Serial Monitor (115200 baud) for debugging information, including:
- BLE device details
- Memory allocation status
- Scanning progress

## Project Information

- **Creators:** IS221017 & IS221043
- **Target Platform:** ESP32-S3
- **Display:** LilyGo 4.7" E-Ink Display
- **Development Environment:** Arduino IDE

## License

This project is provided as-is for educational and development purposes.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve the XK-Scanner functionality.
