// XK-Scanner (IS221017 & IS221043)
// WiFi & BLE Scanner auf E-Ink Display (LilyGo ESP32-S3)
// ----------------------------------------------------------
// Das Programm scannt für WLAN und Bluetooth-Geräte in der Umgebung und wechselt per Knopfdruck zwischen den Scans.
// Ergebnisse werden auf dem 4.7" E-Ink Display angezeigt.
// Die Modi können per Doppelklick gewechselt und der aktuelle Scan durch Langklick (neu-)gestartet werden.

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

uint8_t* framebuffer = NULL;
char buf[128];
#define MAX_LINES 9

BLEScan* pBLEScan;
int bleScanTime = 5;

// Buttons initialisieren
// 1 = WiFi-Scan, 2 = BLE-Scan
Button2 btn1(BUTTON_1);
int scan_mode = 1;

// Langklick: wiederholt den aktuellen Scan
void rescan(Button2& b) {
  displayInfo();
}

// Button Doppelklick: Scanmodus wechseln
void handler(Button2& b) {
  switch (btn1.getType()) {
    case double_click:
      if (scan_mode == 1) {
        scan_mode++;
      } else if (scan_mode == 2) {
        scan_mode--;
      }
      displayInfo();
    default:
      break;
  }
}

void displayInfo() {
  switch (scan_mode) {
    case 1:
      wifiScan();
      break;
    case 2:
      bluetoothScan();
      break;
    default:
      break;
  }
  epd_poweroff();
}

// Durchführung und Ausgabe des WiFi-Scanners
void wifiScan() {
  epd_poweron();
  epd_clear();

  float start = millis();

  int32_t cursor_x = 20;
  int32_t cursor_y = 40;

  float duration = 0;

  writelnWithSpacing("[/] Scanning for WiFi networks in your area...", &cursor_x, &cursor_y);

  int n = WiFi.scanNetworks();
  if (n == 0) {
    writelnWithSpacing("[-] No networks found", &cursor_x, &cursor_y);
  } else {
    sprintf(buf, "[+] Found %d networks:", n);

    writelnWithSpacing(buf, &cursor_x, &cursor_y);
    writelnWithSpacing("Nr       SSID                                            RSSI                          Encryption", &cursor_x, &cursor_y);

    for (int i = 0; i < n; i++) {
      const char* encType = "Unknown";
      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN: encType = "Open"; break;
        case WIFI_AUTH_WEP: encType = "WEP"; break;
        case WIFI_AUTH_WPA_PSK: encType = "WPA"; break;
        case WIFI_AUTH_WPA2_PSK: encType = "WPA2"; break;
        case WIFI_AUTH_WPA_WPA2_PSK: encType = "WPA/WPA2"; break;
        case WIFI_AUTH_WPA3_PSK: encType = "WPA3"; break;
      }

      if (WiFi.RSSI(i) >= -60) {
        sprintf(buf, "%2d:   %-20s                        %2d (good)                         %-8s",
                i + 1,
                WiFi.SSID(i).substring(0, 12).c_str(),
                WiFi.RSSI(i),
                encType);
      } else if (WiFi.RSSI(i) >= -80 && WiFi.RSSI(i) < -60) {
        sprintf(buf, "%2d:   %-20s                        %2d (mid)                       %-8s",
                i + 1,
                WiFi.SSID(i).substring(0, 12).c_str(),
                WiFi.RSSI(i),
                encType);
      } else {
        sprintf(buf, "%2d:   %-20s                        %2d (bad)                          %-8s",
                i + 1,
                WiFi.SSID(i).substring(0, 12).c_str(),
                WiFi.RSSI(i),
                encType);
      }

      if (cursor_y > EPD_HEIGHT - 30) {
        epd_clear();
        cursor_x = 20;
        cursor_y = 40;
        writelnWithSpacing("Nr       SSID                                            RSSI                          Encryption", &cursor_x, &cursor_y);
      }

      writelnWithSpacing(buf, &cursor_x, &cursor_y);
    }

    cursor_y = EPD_HEIGHT - 10;
    cursor_x = 20;

    float end = millis();
    duration = end - start;

    String scan_done = "[+] WiFi-Scan completed in " + String(duration / 1000) + " seconds.";
    writelnWithSpacing(scan_done.c_str(), &cursor_x, &cursor_y);

    epd_poweroff_all();
  }
}

// Durchführung und Ausgabe des Bluetooth-Scanners
void bluetoothScan() {
  epd_poweron();
  epd_clear();

  float start = millis();
  int32_t cursor_x = 20;
  int32_t cursor_y = 40;
  float duration = 0;


  writelnWithSpacing("[/] Scanning for Bluetooth devices in your area...", &cursor_x, &cursor_y);

  BLEScanResults foundDevices = pBLEScan->start(bleScanTime, false);
  int deviceCount = foundDevices.getCount();

  if (deviceCount == 0) {
    writelnWithSpacing("[-] No devices found", &cursor_x, &cursor_y);
  } else {
    sprintf(buf, "[+] Found %d devices:", deviceCount);

    writelnWithSpacing(buf, &cursor_x, &cursor_y);
    writelnWithSpacing("Nr    Name/Address                                                             RSSI", &cursor_x, &cursor_y);

    for (int i = 0; i < deviceCount; i++) {
      BLEAdvertisedDevice device = foundDevices.getDevice(i);
      const char* deviceName = device.getName().c_str();
      const char* deviceAddress = device.getAddress().toString().c_str();
      int rssi = device.getRSSI();

      if (strlen(deviceName) > 0) {
        if (rssi >= -60) {
          sprintf(buf, "%2d:   %-20s                                                %3d dBm (excellent)",
                  i + 1,
                  deviceName,
                  rssi);
        } else if (rssi >= -80 && rssi < -60) {
          sprintf(buf, "%2d:   %-20s                                                %3d dBm (average)",
                  i + 1,
                  deviceName,
                  rssi);
        } else if (rssi < -80) {
          sprintf(buf, "%2d:   %-20s                                                %3d dBm (poor)",
                  i + 1,
                  deviceName,
                  rssi);
        }
      } else {
        if (rssi >= -60) {
          sprintf(buf, "%2d:   %-20s                                                %3d dBm (excellent)",
                  i + 1,
                  deviceAddress,
                  rssi);
        } else if (rssi >= -80 && rssi < -60) {
          sprintf(buf, "%2d:   %-20s                                                %3d dBm (average)",
                  i + 1,
                  deviceAddress,
                  rssi);
        } else if (rssi < -80) {
          sprintf(buf, "%2d:   %-20s                                                %3d dBm (poor)",
                  i + 1,
                  deviceAddress,
                  rssi);
        }
      }

      if (cursor_y > EPD_HEIGHT - 30) {
        epd_clear();
        cursor_x = 20;
        cursor_y = 40;
        writelnWithSpacing("Nr    Name/Address                                                             RSSI", &cursor_x, &cursor_y);
      }
      writelnWithSpacing(buf, &cursor_x, &cursor_y);
    }

    cursor_y = EPD_HEIGHT - 10;
    cursor_x = 20;

    duration = millis() - start;

    String scan_done = "[+] BLE-Scan completed in " + String(duration / 1000) + " seconds.";
    writelnWithSpacing(scan_done.c_str(), &cursor_x, &cursor_y);

    pBLEScan->clearResults();
    epd_poweroff_all();
  }
}

// Intialisierung des Displays & Scan-Komponenten
void setup() {
  Serial.begin(115200);
  epd_init();

  framebuffer = (uint8_t*)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) {
    Serial.println("alloc memory failed !!!");
    while (1)
      ;
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

  btn1.setLongClickDetectedHandler(rescan);
  btn1.setDoubleClickHandler(handler);

  epd_poweron();
  epd_clear();
  int32_t cursor_x = 20;
  int32_t cursor_y = 40;

  writelnWithSpacingStart("➸ Welcome To The XK Scanner!", &cursor_x, &cursor_y);
  writelnWithSpacingStart("[-] Initializing scanners...", &cursor_x, &cursor_y);

  // Initializing the WiFi Scanner
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  writelnWithSpacingStart("[+] WiFi Scanner succesfully initialized!", &cursor_x, &cursor_y);

  // Initializing the BLE Scanner
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  writelnWithSpacingStart("[+] BLE Scanner successfully initialized!", &cursor_x, &cursor_y);
  writelnWithSpacing("", &cursor_x, &cursor_y);
  writelnWithSpacingStart("[-] Usage:", &cursor_x, &cursor_y);
  writelnWithSpacingStart("[+] Double-Press: Switch scanning modes.", &cursor_x, &cursor_y);
  writelnWithSpacingStart("[+] Long-Press: Restart current scan.", &cursor_x, &cursor_y);
  writelnWithSpacing("", &cursor_x, &cursor_y);
  writelnWithSpacingStart("➸ Press the button to start scanning...", &cursor_x, &cursor_y);
}

// Veränderung der Schriftart und Zeilenabstand vom Startbildschirm
void writelnWithSpacingStart(const char* text, int32_t* x, int32_t* y) {
  writeln((GFXfont*)&FiraSans, text, x, y, NULL);
  *y += ((GFXfont*)&FiraSans)->advance_y + 5;
  *x = 20;
}

// Veränderung der Schriftart und Zeilenabstand von der Ausgabe
void writelnWithSpacing(const char* text, int32_t* x, int32_t* y) {
  writeln((GFXfont*)&FiraSansBook12pt, text, x, y, NULL);
  *y += ((GFXfont*)&FiraSansBook12pt)->advance_y + 5;
  *x = 20;
}

void loop() {
  btn1.loop();
}