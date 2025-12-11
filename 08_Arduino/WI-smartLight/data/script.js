let statusInterval;
let brightnessTimeout;
let wechselzeitTimeout;
let initialLoad = true;

document.addEventListener('DOMContentLoaded', function() {
    loadStatus();
    statusInterval = setInterval(loadStatus, 5000);
    
    // Event Listeners
    document.getElementById('wifiForm').addEventListener('submit', handleWiFiSubmit);
    document.getElementById('mqttForm').addEventListener('submit', handleMQTTSubmit);

    // Checkbox change listeners für Input-Felder aktivieren/deaktivieren
    document.getElementById('wifiEnabled').addEventListener('change', toggleWifiInputs);
    document.getElementById('mqttEnabled').addEventListener('change', toggleMqttInputs);

    // Passwort ändern
    document.getElementById('passwordForm').addEventListener('submit', handlePasswordSubmit);

    // Ensure toggle buttons reflect initial state
    document.querySelectorAll('.toggle-password').forEach(button => {
        const inputId = button.getAttribute('onclick').match(/'([^']+)'/)[1];
        const input = document.getElementById(inputId);
        if (input.type === 'text') {
            button.classList.add('active');
        }
    });
});


function togglePasswordVisibility(inputId) {
    const input = document.getElementById(inputId);
    const button = input.nextElementSibling;
    if (input.type === 'password') {
        input.type = 'text';
        button.classList.add('active');
    } else {
        input.type = 'password';
        button.classList.remove('active');
    }
}

function toggleWifiInputs() {
    const enabled = document.getElementById('wifiEnabled').checked;
    const inputs = ['ssid', 'password'];
    const scanButton = document.querySelector('button[onclick="scanWiFi()"]');
    
    inputs.forEach(id => {
        //document.getElementById(id).disabled = !enabled;
        document.getElementById(id).readOnly = !enabled;
    });
    scanButton.disabled = !enabled;
}

function toggleMqttInputs() {
    const enabled = document.getElementById('mqttEnabled').checked;
    const inputs = ['mqttServer', 'mqttPort', 'mqttUser', 'mqttPassword', 'mqttTopic'];
    
    inputs.forEach(id => {
        //document.getElementById(id).disabled = !enabled;
        document.getElementById(id).readOnly = !enabled;
    });
}

function toggleAccordion(id) {
    const content = document.getElementById(id);
    const header = content.previousElementSibling;
    const icon = header.querySelector('.accordion-icon');
    
    if (content.classList.contains('active')) {
        content.classList.remove('active');
        icon.textContent = '+';
        icon.style.transform = 'rotate(0deg)';
    } else {
        // Close all other accordions
        document.querySelectorAll('.accordion-content').forEach(item => {
            item.classList.remove('active');
        });
        document.querySelectorAll('.accordion-icon').forEach(icon => {
            icon.textContent = '+';
            icon.style.transform = 'rotate(0deg)';
        });
        
        // Open clicked accordion
        content.classList.add('active');
        icon.textContent = '−';
        icon.style.transform = 'rotate(180deg)';
    }
}

async function loadStatus() {
    try {
        const response = await fetch('/api/status');
        const data = await response.json();
        
        // Benutzername im Header anzeigen
        if (data.user_name) {
            document.getElementById('userName').textContent = data.user_name;
        }
        
        document.getElementById('chipId').textContent = data.chip_id;
        document.getElementById('uptime').textContent = formatUptime(data.uptime) || '-';
        document.getElementById('mode').textContent = data.mode;
        document.getElementById('ip').textContent = data.ip;
        document.getElementById('wifiStatus').textContent = data.wifi_status;
        
        // MQTT Status mit Link, wenn verbunden
        const mqttStatusElement = document.getElementById('mqttStatus');
        if (data.mqtt_status && data.mqtt_status.toLowerCase().includes('verbunden')) {
            const chipId = data.chip_id;
            mqttStatusElement.innerHTML = `${data.mqtt_status} - <a href="https://os-beyond.at/schnuppern/index.html?client_id=${chipId}" target="_blank">Test</a>`;
        } else {
            mqttStatusElement.textContent = data.mqtt_status;
        }
        
        document.getElementById('currentEffect').textContent = data.current_effect;
        document.getElementById('currentBrightness').textContent = data.brightness || '-';
        document.getElementById('currentWechselzeit').textContent = formatTime(data.auto_timer) || '-';
        document.getElementById('version').textContent = data.version || '-'; // Replace with actual version if available

        // Show/hide wechselzeit in status based on current effect
        const wechselzeitStatus = document.getElementById('currentWechselzeitStatus');
        const currentWechselzeit = document.getElementById('currentWechselzeit');
        
        if (parseInt(data.effect || 0) === 0) {
            wechselzeitStatus.classList.add('show');
            if (data.auto_timer !== undefined) {
                currentWechselzeit.textContent = formatTime(data.auto_timer);
            }
        } else {
            wechselzeitStatus.classList.remove('show');
        }

        // Update WiFi and MQTT settings only on initial load
        if (initialLoad) {
            document.getElementById('wifiEnabled').checked = data.wifi_enabled;
            document.getElementById('mqttEnabled').checked = data.mqtt_enabled;
            document.getElementById('ssid').value = data.wifi_ssid || '';
            document.getElementById('mqttServer').value = data.mqtt_server || 'ae6eb0d482a24683b08611859374ec88.s1.eu.hivemq.cloud';
            document.getElementById('mqttPort').value = data.mqtt_port || 8883;
            document.getElementById('mqttUser').value = data.mqtt_user || 'testHTL';
            document.getElementById('mqttTopic').value = data.mqtt_topic || 'schnuppern';
            document.getElementById('statusLedEnabled').checked = data.led_enabled;

            // Activate/deactivate input fields based on checkbox state
            toggleWifiInputs();
            toggleMqttInputs();
            initialLoad = false;
        }
        
        // Update effect selection normally
        document.getElementById('effectSelect').value = data.effect || 0;
        
        // Show/hide wechselzeit container based on current effect
    toggleWechselzeitVisibility(parseInt(data.effect || 0) === 0);

        // Update brightness slider if value changed externally
        if (data.brightness !== undefined) {
            const slider = document.getElementById('brightnessSlider');
            if (Math.abs(slider.value - data.brightness) > 5) { // Only update if significantly different
                slider.value = data.brightness;
                updateBrightness(data.brightness, false);
            }
        }

        // Update wechselzeit slider if value changed externally
        if (data.auto_timer !== undefined) {
            const slider = document.getElementById('wechselzeitSlider');
            if (Math.abs(slider.value - data.auto_timer) > 5) { // Only update if significantly different
                slider.value = data.auto_timer;
                updateWechselzeit(data.auto_timer, false);
            }
        }

    } catch (error) {
        console.error('Error loading status:', error);
    }
}

function toggleWechselzeitVisibility(show) {
    const container = document.getElementById('wechselzeitContainer');
    if (show) {
        container.classList.add('show');
    } else {
        container.classList.remove('show');
    }
}

function formatTime(seconds) {
    if (seconds < 60) {
        return seconds === 1 ? '1 Sekunde' : `${seconds} Sekunden`;
    } else {
        const minutes = Math.floor(seconds / 60);
        const remainingSeconds = seconds % 60;
        if (remainingSeconds === 0) {
            return minutes === 1 ? '1 Minute' : `${minutes} Minuten`;
        } else {
            return `${minutes} Min ${remainingSeconds}s`;
        }
    }
}

function updateBrightness(value, updateServer = true) {
    const brightnessValue = document.getElementById('brightnessValue');
    brightnessValue.textContent = value;
    
    if (updateServer) {
        // Debounce API calls while sliding
        clearTimeout(brightnessTimeout);
        brightnessTimeout = setTimeout(() => {
            setBrightness(value);
        }, 300);
    }
}

function updateWechselzeit(value, updateServer = true) {
    const wechselzeitValue = document.getElementById('wechselzeitValue');
    
    wechselzeitValue.textContent = formatTime(value);
    
    if (value < 60) {
        wechselzeitDisplay.textContent = `${value}s`;
    } else {
        const minutes = Math.floor(value / 60);
        const seconds = value % 60;
        if (seconds === 0) {
            wechselzeitDisplay.textContent = `${minutes}m`;
        } else {
            wechselzeitDisplay.textContent = `${minutes}m ${seconds}s`;
        }
    }
    
    if (updateServer) {
        // Debounce API calls while sliding
        clearTimeout(wechselzeitTimeout);
        wechselzeitTimeout = setTimeout(() => {
            setWechselzeit(value);
        }, 300);
    }
}

async function setBrightness(value) {
    try {
        const response = await fetch('/api/brightness', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ brightness: parseInt(value) })
        });
        
        const result = await response.json();
        
        if (!result.success) {
            showMessage('Fehler beim Ändern der Helligkeit', 'error');
        }
    } catch (error) {
        console.error('Error setting brightness:', error);
        showMessage('Verbindungsfehler beim Ändern der Helligkeit', 'error');
    }
}

async function setWechselzeit(value) {
    try {
        const response = await fetch('/api/auto-timer', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ auto_timer: parseInt(value) })
        });
        
        const result = await response.json();

        if (result.success) {
            showMessage('Wechselzeit geändert', 'success');
            loadStatus();
        } else {
            showMessage('Fehler beim Ändern der Wechselzeit', 'error');
        }

    } catch (error) {
        console.error('Error setting wechselzeit:', error);
        showMessage('Verbindungsfehler beim Ändern der Wechselzeit', 'error');
    }
}

async function scanWiFi() {
    const button = event.target;
    const originalText = button.textContent;
    button.textContent = 'Scanne...';
    button.disabled = true;
    
    try {
        const response = await fetch('/api/scan');
        const networks = await response.json();
        
        const wifiList = document.getElementById('wifiList');
        wifiList.innerHTML = '';
        
        networks.forEach(network => {
            const item = document.createElement('div');
            item.className = 'wifi-item';
            item.innerHTML = `
                <span>${network.ssid}</span>
                <span class="wifi-signal">${getSignalStrength(network.rssi)}</span>
            `;
            item.onclick = () => {
                document.getElementById('ssid').value = network.ssid;
            };
            wifiList.appendChild(item);
        });
        
    } catch (error) {
        showMessage('Fehler beim Scannen der Netzwerke', 'error');
    } finally {
        button.textContent = originalText;
        button.disabled = false;
    }
}

function getSignalStrength(rssi) {
    if (rssi > -50) return '●●●●';
    if (rssi > -60) return '●●●○';
    if (rssi > -70) return '●●○○';
    if (rssi > -80) return '●○○○';
    return '○○○○';
}

async function handleWiFiSubmit(e) {
    e.preventDefault();
    
    const formData = new FormData(e.target);
    const data = {
        enabled: formData.get('enabled') === 'on',
        ssid: formData.get('ssid'),
        password: formData.get('password')
    };
    
    try {
        const response = await fetch('/api/wifi', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data)
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('WLAN-Konfiguration gespeichert. smartLight wird neu gestartet...', 'success');
            setTimeout(() => {
                if (data.ssid) {
                    //window.location.href = 'http://smartlight.local';
                    window.location.href = window.location.pathname;
                }
            }, 3000);
        } else {
            showMessage('Fehler beim Speichern der WLAN-Konfiguration', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler', 'error');
    }
}

async function handleMQTTSubmit(e) {
    e.preventDefault();
    
    const formData = new FormData(e.target);
    const data = {
        enabled: formData.get('enabled') === 'on',
        server: formData.get('server'),
        port: parseInt(formData.get('port')),
        user: formData.get('user'),
        password: formData.get('password'),
        topic: formData.get('topic')
    };
    
    try {
        const response = await fetch('/api/mqtt', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data)
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('MQTT-Konfiguration gespeichert', 'success');
            loadStatus();
        } else {
            showMessage('Fehler beim Speichern der MQTT-Konfiguration', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler', 'error');
    }
}

async function changeEffect() {
    const effect = document.getElementById('effectSelect').value;
    
    try {
        const response = await fetch('/api/effect', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ effect: parseInt(effect) })
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('Effekt geändert', 'success');
            loadStatus();
        } else {
            showMessage('Fehler beim Ändern des Effekts', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler', 'error');
    }
}

async function toggleStatusLed() {
    const enabled = document.getElementById('statusLedEnabled').checked;
    
    try {
        const response = await fetch('/api/status-led', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ enabled: enabled })
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('Status-LED geändert', 'success');
        } else {
            showMessage('Fehler beim Ändern der Status-LED', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler', 'error');
    }
}

async function resetConfig() {
    if (!confirm('Sind Sie sicher, dass Sie die gesamte Konfiguration löschen möchten? smartLight wird neu gestartet.')) {
        return;
    }
    
    try {
        const response = await fetch('/api/reset', {
            method: 'POST'
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('Konfiguration gelöscht. smartLight wird neu gestartet...', 'success');
            setTimeout(() => {
                window.location.reload();
            }, 3000);
        } else {
            showMessage('Fehler beim Löschen der Konfiguration', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler', 'error');
    }
}

function showMessage(text, type) {
    const messageDiv = document.getElementById('message');
    const overlay = document.getElementById('overlay');
    const mainContent = document.getElementById('mainContent');
    
    messageDiv.textContent = text;
    messageDiv.className = `message ${type} show`;
    overlay.classList.add('active');
    mainContent.classList.add('disabled');
    
    setTimeout(() => {
        messageDiv.classList.remove('show');
        overlay.classList.remove('active');
        mainContent.classList.remove('disabled');
    }, 3000);
}

function formatUptime(seconds) {
    if (!seconds || seconds < 0) return 'Unbekannt';
    
    const days = Math.floor(seconds / (24 * 3600));
    seconds %= (24 * 3600);
    const hours = Math.floor(seconds / 3600);
    seconds %= 3600;
    const minutes = Math.floor(seconds / 60);
    
    let result = [];
    if (days > 0) result.push(`${days} Tag${days !== 1 ? 'e' : ''}`);
    if (hours > 0) result.push(`${hours} Stunde${hours !== 1 ? 'n' : ''}`);
    if (minutes > 0 || result.length === 0) result.push(`${minutes} Minute${minutes !== 1 ? 'n' : ''}`);
    
    return result.join(', ');
}

async function handlePasswordSubmit(e) {
    e.preventDefault();
    
    const currentPassword = document.getElementById('currentPassword').value;
    const newPassword = document.getElementById('newPassword').value;
    const confirmPassword = document.getElementById('confirmPassword').value;
    
    // Passwort-Validierung
    if (newPassword !== confirmPassword) {
        showMessage('Die neuen Passwörter stimmen nicht überein', 'error');
        return;
    }
    
    if (newPassword.length < 6) {
        showMessage('Das neue Passwort muss mindestens 6 Zeichen lang sein', 'error');
        return;
    }
    
    const data = {
        currentPassword: currentPassword,
        newPassword: newPassword
    };
    
    try {
        const response = await fetch('/api/change-password', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data)
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('Passwort erfolgreich geändert', 'success');
            document.getElementById('passwordForm').reset();
        } else {
            showMessage(result.message || 'Fehler beim Ändern des Passworts', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler beim Ändern des Passworts', 'error');
    }
}

// Neue Funktion: Abmelden
async function logout() {
    if (!confirm('Möchten Sie sich wirklich abmelden?')) {
        return;
    }
    
    try {
        const response = await fetch('/api/logout', {
            method: 'POST'
        });
        
        const result = await response.json();
        
        if (result.success) {
            showMessage('Erfolgreich abgemeldet', 'success');
            setTimeout(() => {
                window.location.href = '/'; // Redirect to home page
            }, 1500);
        } else {
            showMessage('Fehler beim Abmelden', 'error');
        }
    } catch (error) {
        showMessage('Verbindungsfehler beim Abmelden', 'error');
    }
}