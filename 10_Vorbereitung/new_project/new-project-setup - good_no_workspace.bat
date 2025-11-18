@echo off
setlocal

:: ==========================================================
:: KONFIGURATION
:: ==========================================================
set "REPO_URL=https://github.com/AndreasEckhart/WI-smartLight.git"
set "REPO_FOLDER=WI-smartLight"

set "TARGET_BASE_DIR_NAME=2025-WI-Schnuppern"
set "TARGET_PROJECT_DIR_NAME=%TARGET_BASE_DIR_NAME%\%REPO_FOLDER%"

set "VSCODE_EXE_PATH=%LOCALAPPDATA%\Programs\Microsoft VS Code\Code.exe"

:: ==========================================================
:: ERMITTLUNG DES KORREKTEN DESKTOP-PFADES (Registry-Lookup KORRIGIERT)
:: ==========================================================
set "ACTUAL_DESKTOP_PATH="

:: KORREKTUR: Der Umleitungsoperator (>) wird mit einem Caret (^) geschuetzt.
for /f "tokens=2*" %%a in ('reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" /v Desktop 2^>nul') do (
    set "ACTUAL_DESKTOP_PATH=%%b"
)

:: Fallback, falls die Abfrage fehlschlaegt
if not defined ACTUAL_DESKTOP_PATH (
    echo ACHTUNG: Registry-Abfrage fehlgeschlagen. Verwende Standard-Desktop-Pfad.
    set "ACTUAL_DESKTOP_PATH=%USERPROFILE%\Desktop"
)

:: Endgueltige Zielpfade definieren
set "TARGET_BASE_DIR=%ACTUAL_DESKTOP_PATH%\%TARGET_BASE_DIR_NAME%"
set "TARGET_PROJECT_DIR=%ACTUAL_DESKTOP_PATH%\%TARGET_PROJECT_DIR_NAME%"


:: ==========================================================
:: SCHRITT 1: ZIELORDNER BEREINIGEN UND ERSTELLEN
:: ==========================================================
echo.
echo === 1/3 Zielordner bereinigen und erstellen ===
echo Zielordner (Aufgeloest): "%TARGET_BASE_DIR%"

:: 1. Loeschen des Ordners (falls er existiert)
if exist "%TARGET_BASE_DIR%" (
    echo Vorhandenen Ordner wird geloescht...
    rmdir /s /q "%TARGET_BASE_DIR%"
    if errorlevel 1 goto Loeschen_FEHLER
    
    :Loeschen_OK
    echo Loeschvorgang erfolgreich.
)

:: 2. Erstellen des neuen Ordners
echo Erstelle neuen Ordner...
mkdir "%TARGET_BASE_DIR%"

if errorlevel 1 goto Erstellen_FEHLER

echo Ordner "%TARGET_BASE_DIR_NAME%" erfolgreich erstellt.
goto SCHRITT_2


:Loeschen_FEHLER
echo FEHLER: Konnte den Ordner nicht loeschen. (Berechtigungen oder Pfadfehler.)
goto ENDE

:Erstellen_FEHLER
echo FEHLER: Konnte den Ordner nicht erstellen. (Pruefen Sie: %TARGET_BASE_DIR%)
goto ENDE


:: ==========================================================
:: SCHRITT 2: GIT REPOSITORY KLONEN
:: ==========================================================
:SCHRITT_2
echo.
echo === 2/3 Klone Git Repository ===

cd "%TARGET_BASE_DIR%"

echo Klone %REPO_URL% ...
git clone %REPO_URL%

if errorlevel 1 (
    echo FEHLER: Klonen fehlgeschlagen. Bitte Git-Installation/Verbindung pruefen.
    goto ENDE
)
echo Klonen erfolgreich. Repository liegt unter "%TARGET_PROJECT_DIR%".


:: ==========================================================
:: SCHRITT 3: VS CODE MIT PROJEKT STARTEN
:: ==========================================================
:SCHRITT_3
echo.
echo === 3/3 Starte VS Code mit PlatformIO Projekt ===

if exist "%VSCODE_EXE_PATH%" (
    start "" "%VSCODE_EXE_PATH%" "%TARGET_PROJECT_DIR%"
    
    echo VS Code wurde gestartet und oeffnet das Projekt.
) else (
    echo FEHLER: VS Code wurde nicht am erwarteten Pfad gefunden: "%VSCODE_EXE_PATH%"
)

:: ==========================================================
:: ABSCHLUSS
:: ==========================================================
:ENDE
echo.
echo Setup- und Startvorgang abgeschlossen.
pause
endlocal