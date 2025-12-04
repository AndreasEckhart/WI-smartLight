# WI-smartLight<br><br>Entwicklungsumgebung einrichten (Windows 11 & Linux)

Diese Anleitung beschreibt, was für die Entwicklung an diesem Projekt benötigt wird und wie du die Umgebung einrichtest. Für Windows 11 gibt es ein Setup‑Skript, das die wichtigsten Schritte automatisiert.

---

## Schnellstart

- Windows 11: Empfohlen ist das Skript `10_Vorbereitung/install/setup-ide.cmd` (User‑Installation über winget). Es installiert VS Code, setzt VS‑Code‑Einstellungen und installiert die PlatformIO‑Erweiterung.
- Linux: Installiere VS Code, füge die PlatformIO‑Erweiterung hinzu und richte udev‑Regeln/Serienport‑Rechte ein.
- Projekt immer über den Workspace `2025-WI-Schnuppern.code-workspace` öffnen, damit alle Teilordner geladen werden.

---

## Windows 11

### Option A: Automatisches Setup (empfohlen)
1) Öffne `10_Vorbereitung/install/setup-ide.cmd` per Doppelklick oder aus PowerShell:

```powershell
# Aus dem Projektordner heraus
.\\10_Vorbereitung\\install\\setup-ide.cmd
```

Das Skript führt aus:
- Installiert Visual Studio Code (User‑Scope) via `winget`.
- Startet VS Code und installiert die Erweiterungen German Language Pack und PlatformIO. Beim ersten Start lädt PlatformIO Core automatisch nach (dauert initial ein wenig).

2) Treiber: CP210x USB‑to‑UART für ESP32 installieren (Pflicht)
- Ohne diesen Treiber erscheint der ESP32 nicht als COM‑Port in Windows.
- Offizielle Silicon‑Labs‑Treiberseite: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- Nach der Installation USB‑Kabel erneut verbinden. Den COM‑Port findest du im Geräte‑Manager unter „Anschlüsse (COM & LPT)“.

### Option B: Manuelles Setup
Falls du das Skript nicht nutzen möchtest, führe folgende Schritte in PowerShell (pwsh) aus:

```powershell

# 1) Visual Studio Code installieren
winget install --id Microsoft.VSCode --scope user --silent --accept-package-agreements --accept-source-agreements

# 2) PlatformIO‑Erweiterung installieren
"$env:LOCALAPPDATA\\Programs\\Microsoft VS Code\\bin\\code" --install-extension platformio.platformio-ide

# 3) Deutsches Sprachpaket für VS Code (optional, empfohlen)
"$env:LOCALAPPDATA\\Programs\\Microsoft VS Code\\bin\\code" --install-extension MS-CEINTL.vscode-language-pack-de

# 5) VS Code starten
Start-Process "$env:LOCALAPPDATA\\Programs\\Microsoft VS Code\\bin\\code"
```

Installiere danach unbedingt den CP210x‑Treiber (siehe oben).

---

## Linux

Hinweis: Der CP210x‑Treiber ist im Linux‑Kernel enthalten; es ist kein zusätzlicher Treiber nötig. Für den Zugriff auf die serielle Schnittstelle sind jedoch udev‑Regeln und Gruppenrechte erforderlich.

### Installation (Beispiele für gängige Distributionen)
- Debian/Ubuntu:

```bash
# VS Code (MS Repo verwenden)
sudo apt update
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor | sudo tee /usr/share/keyrings/packages.microsoft.gpg > /dev/null
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" | sudo tee /etc/apt/sources.list.d/vscode.list
sudo apt update
sudo apt install -y code
```

- Fedora:
```bash
sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
sudo sh -c 'echo -e "[code]\nname=Visual Studio Code\nbaseurl=https://packages.microsoft.com/yumrepos/vscode\nenabled=1\ngpgcheck=1\ngpgkey=https://packages.microsoft.com/keys/microsoft.asc" > /etc/yum.repos.d/vscode.repo'
sudo dnf check-update || true
sudo dnf install -y code
```

### PlatformIO in VS Code
- Starte VS Code und installiere die Erweiterung „PlatformIO IDE“ (ID: `platformio.platformio-ide`).


### Deutsches Sprachpaket für VS Code (optional, empfohlen)
```bash
code --install-extension MS-CEINTL.vscode-language-pack-de
```
Nach Installation ggf. VS Code neu starten. Die Anzeigesprache kann über „Configure Display Language“ (Befehlspalette) auf Deutsch umgestellt werden.

### udev‑Regeln und Port‑Rechte
```bash
# PlatformIO udev‑Regeln installieren
wget -O - https://raw.githubusercontent.com/platformio/platformio-core-installer/master/scripts/99-platformio-udev.rules \
  | sudo tee /etc/udev/rules.d/99-platformio-udev.rules > /dev/null
sudo udevadm control --reload-rules
sudo udevadm trigger

# Benutzer zur passenden Gruppe hinzufügen (häufig 'dialout' oder 'uucp' je nach Distro)
sudo usermod -aG dialout $USER 2>/dev/null || true
sudo usermod -aG uucp $USER 2>/dev/null || true

# Neue Gruppenzugehörigkeit aktivieren (neue Shell) oder einmal ab‑/anmelden
newgrp dialout || true
```

Geräteprüfung: Nach dem Einstecken des Boards sollte z. B. `/dev/ttyUSB0` oder `/dev/ttyACM0` erscheinen:

```bash
ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null || echo "Kein serielles Gerät gefunden"
```

---

## Projekt öffnen, bauen, flashen

### Projekt herunterladen
Das aktuelle Repo lokal herunterladen:
- Öffne `10_Vorbereitung/new_project/smartLight-setup.cmd` per Doppelklick oder aus PowerShell:

### In VS Code mit PlatformIO
- Das Skript lädt die aktuelle Version als zip-Datei von GitHub herunter und entpackt das Projekt im selben Verzeichnis
- Anschließend öffnet automatisch VS Code den entsprechenden Workspace
- Falls der Workspace nicht automatisch öffnet, öffne den Workspace `2025-WI-Schnuppern.code-workspace` in VS Code (nicht nur einen Unterordner).
- Öffne die gewünschte Umgebung (z. B. `01_Grundlagen` oder `02_WI-smartLight`).
- In der PlatformIO Seitenleiste:
  - Build: „Build“ ausführen
  - Flashen: „Upload“ ausführen
  - Seriell: „Monitor“ öffnen (Baudrate typ. 115200)

Beispiel (Windows PowerShell):
```powershell
code .\2025-WI-Schnuppern.code-workspace
```

Beispiel (Linux/macOS):
```bash
code 2025-WI-Schnuppern.code-workspace
```

---

## Troubleshooting
- Windows: Kein COM‑Port sichtbar? CP210x‑Treiber installieren (Link oben), anderes USB‑Kabel/Port testen, Geräte‑Manager prüfen.
- Linux: Benutzer in Gruppe `dialout`/`uucp`? udev‑Regeln geladen? Board erscheint als `/dev/ttyUSB*`? Kabel/Port testen.
- VS Code/PlatformIO: Nach Erstinstallation ggf. VS Code neu starten, damit PlatformIO Core sauber fertig installiert wird.
- Board‑Konfiguration: Die `platformio.ini` der jeweiligen Unterprojekte prüfen (Board/Port/Baudrate).

---

## Hinweise
- Das Windows‑Setup orientiert sich am Skript `10_Vorbereitung/install/setup-ide.cmd` in diesem Repo.