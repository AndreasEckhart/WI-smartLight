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

:: Workspace-Datei liegt IM geklonten Repo-Ordner
set "WORKSPACE_FILE_NAME=2025-WI-Schnuppern.code-workspace"

:: ==========================================================
:: ERMITTLUNG DES KORREKTEN DESKTOP-PFADES (Registry-Lookup)
:: ==========================================================
set "ACTUAL_DESKTOP_PATH="

:: Der Befehl liest den aktuellen, von der Shell verwendeten Desktop-Pfad aus der Registry
for /f "tokens=2*" %%a in ('reg query "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" /v Desktop 2^>nul') do (
    set "ACTUAL_DESKTOP_PATH=%%b"
)

:: Fallback, falls die Abfrage fehlschlaegt
if not defined ACTUAL_DESKTOP_PATH (
    set "ACTUAL_DESKTOP_PATH=%USERPROFILE%\Desktop"
)

:: Endgueltige Zielpfade definieren
set "TARGET_BASE_DIR=%ACTUAL_DESKTOP_PATH%\%TARGET_BASE_DIR_NAME%"
set "TARGET_PROJECT_DIR=%TARGET_BASE_DIR%\%REPO_FOLDER%"
:: Zielpfad zur Workspace-Datei
set "TARGET_WORKSPACE_PATH=%TARGET_PROJECT_DIR%\%WORKSPACE_FILE_NAME%"


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
:: SCHRITT 3: VS CODE MIT WORKSPACE STARTEN
:: ==========================================================
:SCHRITT_3
echo.
echo === 3/3 Starte VS Code mit Workspace-Datei ===

:: Pruefung auf Existenz der Workspace-Datei im geklonten Repo
if exist "%TARGET_WORKSPACE_PATH%" (

    if exist "%VSCODE_EXE_PATH%" (
        :: Starte Code.exe direkt und uebergebe den Pfad zur .code-workspace Datei
        start "" "%VSCODE_EXE_PATH%" "%TARGET_WORKSPACE_PATH%"
        
        echo VS Code wurde mit Workspace gestartet.
    ) else (
        echo FEHLER: VS Code wurde nicht am erwarteten Pfad gefunden: "%VSCODE_EXE_PATH%"
    )

) else (
    echo FEHLER: Workspace-Datei %WORKSPACE_FILE_NAME% nicht im Repo-Ordner gefunden: "%TARGET_PROJECT_DIR%"
)

:: ==========================================================
:: ABSCHLUSS
:: ==========================================================
:ENDE
echo.
echo Setup- und Startvorgang abgeschlossen.
pause
endlocal