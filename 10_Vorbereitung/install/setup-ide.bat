@echo off
setlocal

:: ==========================================================
:: ANPASSUNGEN VOR DER AUSFÜHRUNG
:: ==========================================================

:: Pfad zur Master-Konfigurationsdatei anpassen!
:: Die Master_settings.json MUSS im selben Ordner wie dieses Skript liegen
set SETTINGS_SOURCE=".\vscode-settings.json"

:: Setze den erwarteten Pfad zur VS Code ausführbaren Datei (User Scope Installation)
set VSCODE_PATH="%LOCALAPPDATA%\Programs\Microsoft VS Code\bin\code"

:: ==========================================================
:: VORPRÜFUNG: WINGET VERFÜGBARKEIT
:: ==========================================================
echo.
echo === Prüfe Verfügbarkeit von winget ===
winget --version >nul 2>&1
if %errorlevel% neq 0 (
    echo FEHLER: winget ist nicht verfügbar. Bitte manuell im Microsoft Store installieren oder OS aktualisieren.
    pause
    exit /b 1
)
echo winget verfügbar.

:: ==========================================================
:: SCHRITT 1: GIT SILENT INSTALLATION (USER)
:: ==========================================================
echo.
echo === 1/5 Starte Git Installation (User Scope) ===
winget install --id Git.Git --scope user --silent --accept-package-agreements --accept-source-agreements
if errorlevel 1 (
    echo Fehler: Git Installation fehlgeschlagen.
) else (
    echo Git erfolgreich installiert.
)

:: ==========================================================
:: SCHRITT 2: VS CODE SILENT INSTALLATION (USER)
:: ==========================================================
echo.
echo === 2/5 Starte VS Code Installation (User Scope) ===
winget install --id Microsoft.VSCode --scope user --silent --accept-package-agreements --accept-source-agreements
if errorlevel 1 (
    echo Fehler: VS Code Installation fehlgeschlagen.
) else (
    echo VS Code erfolgreich installiert.
)

:: Wartezeit, um sicherzustellen, dass die Prozesse abgeschlossen sind
timeout /t 10 /nobreak >nul

:: ==========================================================
:: SCHRITT 3: KONFIGURATION ANWENDEN & COPILOT DEAKTIVIEREN (USER)
:: ==========================================================
echo.
echo === 3/5 Kopiere Master-Settings.json und deaktiviere Copilot ===
:: Zielpfad für die Benutzerkonfiguration
set SETTINGS_DEST="%APPDATA%\Code\User\settings.json"

:: Erstelle den Zielordner, falls nicht vorhanden
if not exist "%APPDATA%\Code\User" mkdir "%APPDATA%\Code\User"

:: Kopiere die Master-Datei
copy /Y %SETTINGS_SOURCE% %SETTINGS_DEST%
if exist %SETTINGS_DEST% (
    echo Konfigurationsdatei erfolgreich kopiert und Copilot/Chat deaktiviert.
) else (
    echo Fehler beim Kopieren der settings.json!
)

:: ==========================================================
:: SCHRITT 4: VS CODE STARTEN
:: ==========================================================
echo.
echo === 4/5 Starte VS Code ===
if exist %VSCODE_PATH%.exe (
    start "" %VSCODE_PATH%
    echo VS Code wurde gestartet.
    echo WICHTIG: PlatformIO Core sollte nun mit dem Download starten.
) else (
    echo FEHLER: VS Code konnte nicht gestartet werden, code.exe nicht am erwarteten Pfad.
)

:: Wartezeit, um die Installation der Erweiterung abzuschließen (PlatformIO Core Download startet beim Start)
timeout /t 5 /nobreak >nul

:: ==========================================================
:: SCHRITT 5: VS CODE ERWEITERUNGEN INSTALLATION (USER)
:: ==========================================================
echo.
echo === 5/5 Installiere Sprachpaket und PlatformIO ===

if exist %VSCODE_PATH%.exe (
    echo.
    echo === Installiere German Language Pack ===
    %VSCODE_PATH% --install-extension MS-CEINTL.vscode-language-pack-de
    if errorlevel 1 (
        echo Fehler beim Installieren des deutschen Sprachpakets.
    ) else (
        echo Deutsches Sprachpaket installiert.
    )

    echo.
    echo === Installiere PlatformIO Extension ===
    %VSCODE_PATH% --install-extension platformio.platformio-ide
    if errorlevel 1 (
        echo Fehler beim Installieren der PlatformIO-Erweiterung.
    ) else (
        echo PlatformIO-Erweiterung installiert.
    )
) else (
    echo FEHLER: code.exe Pfad nicht gefunden. Konnte VS Code Erweiterungen nicht installieren.
)


:: ==========================================================
:: ABSCHLUSS
:: ==========================================================
echo.
echo Setup unter Benutzerrechten abgeschlossen.
pause

endlocal