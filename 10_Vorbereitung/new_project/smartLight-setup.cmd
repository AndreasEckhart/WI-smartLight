@echo off
chcp 65001
setlocal

:: ==========================================
:: Einstellungen
:: ==========================================
:: Download Pfad
set "REPO_URL=https://github.com/AndreasEckhart/WI-smartLight/archive/refs/heads/main.zip"
:: Workspace-Datei liegt Projekt-Ordner
set "WORKSPACE_FILE_NAME=2025-WI-Schnuppern.code-workspace"

echo ===============================================
echo   WI-smartLight Projekt wird heruntergeladen
echo ===============================================

:: Datei- und Ordnernamen
set "ZIP_FILE=WI-smartLight-download.zip"
set "EXTRACT_FOLDER=WI-smartLight

:: Falls alte Dateien existieren → löschen
if exist "%ZIP_FILE%" del "%ZIP_FILE%"
if exist "%EXTRACT_FOLDER%" rmdir /s /q "%EXTRACT_FOLDER%"

echo Lade Projekt von GitHub herunter...
curl -L "%REPO_URL%" --output "%ZIP_FILE%"

if not exist "%ZIP_FILE%" (
    echo Fehler: ZIP konnte nicht heruntergeladen werden.
    pause
    exit /b
)

echo Entpacke Dateien...
powershell -command "Expand-Archive -Force '%ZIP_FILE%' '%EXTRACT_FOLDER%'"

echo Lösche ZIP...
del "%ZIP_FILE%"

:: GitHub ZIPs enthalten einen Unterordner → automatisch erkennen
for /d %%i in ("%EXTRACT_FOLDER%\*") do set "PROJECT_DIR=%%i"

:: Zielpfad zur Workspace-Datei
set "TARGET_WORKSPACE_PATH=%PROJECT_DIR%\%WORKSPACE_FILE_NAME%"

echo Starte VS Code...
code "%TARGET_WORKSPACE_PATH%" --no-wait

echo Fertig! Projekt wurde in VS Code geöffnet.

