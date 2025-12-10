# smartLight Projekt-Setup

## Schnellstart für Windows

Dieses Verzeichnis enthält automatisierte Setup-Skripte für das smartLight-Projekt.

### Option 1: Automatisches Setup (empfohlen)

Laden Sie die Datei **`smartLight-setup.cmd`** herunter und führen Sie sie aus:

1. **Klick auf den Link unten** (Download startet automatisch)
2. **Datei speichern** in einem beliebigen Ordner
3. **Doppelklick** auf `smartLight-setup.cmd`
4. Das Skript installiert automatisch:
   - PlatformIO IDE in VS Code
   - Deutsche Sprachpacks
   - Git (falls noch nicht installiert)
   - Alle erforderlichen Abhängigkeiten

Das Projekt wird dann auf dem Desktop unter `Desktop\2025-WI-Schnuppern\WI-smartLight` erstellt und automatisch in VS Code geöffnet.

#### Download-Links:

- **[smartLight-setup.cmd herunterladen](https://github.com/AndreasEckhart/WI-smartLight/raw/main/10_Vorbereitung/new_project/smartLight-setup.cmd)** ⬇️

### Option 2: Manuelle Setup

Falls Sie das Skript lieber manuell durchgehen möchten, führen Sie folgende Schritte durch:

1. **Git installieren** (falls nicht vorhanden)
   ```powershell
   winget install Git.Git
   ```

2. **VS Code installieren** (falls nicht vorhanden)
   ```powershell
   winget install Microsoft.VisualStudioCode
   ```

3. **VS Code Extensions installieren**
   - PlatformIO IDE
   - German Language Pack

4. **Repository klonen**
   ```powershell
   git clone https://github.com/AndreasEckhart/WI-smartLight.git
   cd WI-smartLight
   code 2025-WI-Schnuppern.code-workspace
   ```

### Anforderungen

- **Windows 10/11**
- **Administrator-Rechte** (für Treiber-Installation)
- **Internet-Verbindung** (für Downloads)
- **~500 MB** freier Speicherplatz

### Unterstützte Hardware

- **ESP32** Mikrocontroller
- **WS2812B NeoPixel** LED-Ring (24 LEDs)
- **CP210x USB-Treiber** (wird vom Setup installiert)

### Häufige Probleme

**Skript wird nicht ausgeführt?**
- Rechtsklick → "Mit PowerShell ausführen"
- Oder im Administrator-Terminal: `.\smartLight-setup.cmd`

**Port COM ist nicht verfügbar?**
- CP210x Treiber prüfen: [Silicon Labs CP210x Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

**PlatformIO wird nicht erkannt?**
- VS Code neu starten
- Extensions aktualisieren

---

**Version:** 1.0  
**Erstellt:** 2025  
**Für:** HTL Anichstrasse Innsbruck - Wirtschaftsingenieure
