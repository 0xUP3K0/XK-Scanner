# XK Scanner (LilyGo e-paper WiFi & BLE Scanner)

Dies ist ein Netzwerk-Scanner für das **LilyGo EPD-47 e-Ink Display**, der sowohl **WLAN-** als auch **Bluetooth-Geräte** in der Umgebung erkennen und anzeigen kann. Die Bedienung erfolgt über einen Button, mit dem Scans durchgeführt werden können und zwischen den Scan-Modi gewechselt werden kann.

## Hardware-Voraussetzungen

- LilyGo EPD-47 Board mit e-Ink Display
- ESP32 Mikrocontroller
- Physischer Button (`BUTTON_1`) (mit dem Display integriert)

## Software Libraries

- `epd_driver.h` – für das e-Ink Display
- `WiFi.h` – für WLAN-Funktionen
- `BLEDevice.h`, `BLEUtils.h`, `BLEScan.h` – für Bluetooth Low Energy
- `Button2.h` – für Button-Handling
- **FiraSans** Schriftarten

## Funktionsweise

### Initialisierung

#### Display Setup:

- Initialisierung des e-Ink Displays
- Allokierung eines Framebuffers
- Initialisierung der Scanner
- Anzeige einer Begrüßungsnachricht

#### Scanner Initialisierung:

- WLAN wird im **Station-Modus** initialisiert  
- BLE-Scanner wird **aktivem Scan-Modus** mitkonfiguriert

## Hauptfunktionen

### 1. WLAN-Scan (`wifiScan()`)

- Scannt nach verfügbaren WLAN-Netzwerken  
- Zeigt folgende Informationen an:
  - SSID
  - Signalstärke (RSSI) mit Qualitätsbewertung (*good/medium/bad*)
  - Verschlüsselungstyp (WPA2, WPA3, etc.)
- **Automatischer Seitenumbruch** bei vollem Display  

### 2. Bluetooth-Scan (`bluetoothScan()`)

- Scannt nach BLE-Geräten
- Zeigt folgende Informationen an:
  - Gerätename oder MAC-Adresse
  - Signalstärke (RSSI) mit Qualitätsbewertung (*excellent/average/poor*)
  - Ungefähre Distanz bis zum Gerät, basierend auf dem RSSI und dem TXPower
- **Automatischer Seitenumbruch** bei vollem Display

## Button-Steuerung
Linker Button (SENSOP_VN)
- **Doppelklick:** Wechselt zwischen WLAN- und Bluetooth-Scan-Modus  
- **Langer Klick:** Führt einen neuen Scan im aktuellen Modus durch  

## Display-Ausgabe

- Verwendung der Schriftarten **FiraSans** für Überschriften und **FiraSansBook12pt** für Scan-Ergebnisse
- Automatische Zeilenabstände und Cursor-Positionierung
- Display wird nach jedem Scan ausgeschaltet zur Stromersparnis (`epd_poweroff`)

## Bedienung

1. Display am PC einstecken und im Arduino IDE "ESP32S3 Dev Module" auswählen
2. Datei laden und dann auf "Upload" klicken
3. Mit **Doppelklick** den Scan-Modus wechseln
4. Mit **langem Klick** einen Scan starten (default: WiFi-Scan)
5. Ergebnisse werden auf dem Display angezeigt

---

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
