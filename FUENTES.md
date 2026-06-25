# Declaración de Fuentes e Integridad Académica


## 1. Librerías utilizadas

| Librería | Versión | Uso en el proyecto | Fuente |
|------------------------|---------|--------------------------------------------|--------|
| ESP8266WiFi | (incluida en el core ESP8266) | Conexión WiFi de la placa | https://github.com/esp8266/Arduino |
| WiFiClientSecure | (incluida en el core ESP8266) | Conexión HTTPS para Telegram | https://github.com/esp8266/Arduino |
| UniversalTelegramBot | 1.3.x | Envío de notificaciones por Telegram | https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot |
| ArduinoJson | 6.x | Dependencia de UniversalTelegramBot (parseo JSON) | https://github.com/bblanchon/ArduinoJson |
| MFRC522 | 1.4.x | Lectura de etiquetas RFID con el módulo RC522 | https://github.com/miguelbalboa/rfid |
| SPI | (incluida en el core Arduino) | Comunicación con el lector RC522 | https://www.arduino.cc/reference/en/language/functions/communication/spi/ |
| Firebase ESP Client (FirebaseESP8266) | 4.x | Envío de los datos a Firebase Realtime Database | https://github.com/mobizt/Firebase-ESP-Client |
| time.h (NTP) | (estándar) | Obtención de la fecha/hora actual vía NTP | — |


### Dashboard web

| Librería / SDK | Versión | Uso en el proyecto | Fuente |
|----------------|---------|--------------------|--------|
| Firebase JavaScript SDK | 9.x (modular) | Conexión del dashboard a la Realtime Database y lectura en tiempo real con `onValue` | https://firebase.google.com/docs/web/setup |

---

## 2. Código externo adaptado
 Codigo hecho por Claude


## 3. Uso de Inteligencia Artificial

### Integración de Firebase en el firmware del ESP8266
- **Herramienta:** Claude (Anthropic), junio 2026
- **Uso:** ayuda para integrar la librería Firebase ESP Client en el sketch
  existente (includes, objetos `FirebaseData`/`FirebaseConfig`/`FirebaseAuth`,
  inicialización en el `setup()` y función para subir cada lectura a la
  Realtime Database).
- **Adaptación:** <!-- COMPLETAR: ej. "se ajustó la ruta /alimentos y el nodo
  /ultimo, los nombres de los campos (nombre, uid, fecha_ingreso, fecha_expiracion,
  duracion_dias, timestamp) y se conectó con la lógica de lectura RFID propia."
- **Comprensión:** el equipo entiende que la función arma un objeto JSON con los
  datos del alimento y lo envía con `Firebase.pushJSON()` a la ruta `/alimentos`,
  generando una clave única por registro, y sobrescribe `/ultimo` con la última
  lectura.

### Dashboard web (estructura y lógica de tiempo real)
- **Herramienta:** Claude (Anthropic) / Claude Code, junio 2026
- **Uso:** generación de la página HTML/JS que se conecta a Firebase, escucha
  cambios con `onValue`, calcula la cuenta regresiva al vencimiento y clasifica
  por color de urgencia.
- **Adaptación:** <!-- COMPLETAR: ej. "se ajustaron los umbrales de color, el
  parseo de fecha DD/MM/YYYY y el orden por urgencia." -->


---

### Estructura del README
- Herramienta: Claude (junio 2026)
- Uso: Generación de una base del README que luego fue editado por el equipo; los detalles tecnicos fueron escritos por los integrantes del grupo.
