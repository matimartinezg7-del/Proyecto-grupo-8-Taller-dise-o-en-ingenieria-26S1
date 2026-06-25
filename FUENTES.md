# Declaración de Fuentes e Integridad Académica

Este archivo declara las librerías de terceros, el código externo adaptado y el
uso de herramientas de inteligencia artificial en el desarrollo del proyecto,
según el código de honor de la UAI y los requisitos del Avance #3 de TEI201.

---

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

<!-- VERIFICAR las versiones exactas que tienen instaladas en su Library Manager
     y ajustar la tabla. Si usan alguna librería extra, agregarla. -->

### Dashboard web

| Librería / SDK | Versión | Uso en el proyecto | Fuente |
|----------------|---------|--------------------|--------|
| Firebase JavaScript SDK | 9.x (modular) | Conexión del dashboard a la Realtime Database y lectura en tiempo real con `onValue` | https://firebase.google.com/docs/web/setup |

---

## 2. Código externo adaptado

Declarar acá cualquier fragmento tomado de tutoriales, foros, documentación o
repositorios. Ejemplos de cómo completarlo (ajustar a lo que realmente usaron):

### Ejemplo de lectura RFID con MFRC522
- **Fuente:** ejemplo `DumpInfo` / `ReadNUID` de la librería MFRC522
  (https://github.com/miguelbalboa/rfid/tree/master/examples)
- **Adaptación:** <!-- COMPLETAR: ej. "se tomó la rutina de lectura del UID y se
  adaptó para asociar cada UID a un nombre de alimento mediante un switch/tabla,
  descartando la impresión completa del dump." -->

### Configuración del bot de Telegram
- **Fuente:** ejemplos de la librería Universal-Arduino-Telegram-Bot
  (https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/tree/master/examples)
- **Adaptación:** <!-- COMPLETAR: ej. "se usó la estructura de envío de mensajes
  (bot.sendMessage) y se construyó el texto de la alerta con los datos del
  alimento leído." -->

### Sincronización de hora por NTP
- **Fuente:** <!-- COMPLETAR si tomaron el patrón de configTime() de algún tutorial -->
- **Adaptación:** <!-- COMPLETAR -->

<!-- AGREGAR aquí cualquier otro fragmento copiado/adaptado, con archivo y líneas.
     Si NO copiaron nada de un sitio externo más allá de las librerías, dejarlo
     explícito: "El resto del código fue escrito por el equipo." -->

---

## 3. Uso de Inteligencia Artificial

Se permite usar IA; lo exigido es declarar qué generó, qué hace y cómo se adaptó.

### Integración de Firebase en el firmware del ESP8266
- **Herramienta:** Claude (Anthropic), junio 2026
- **Uso:** ayuda para integrar la librería Firebase ESP Client en el sketch
  existente (includes, objetos `FirebaseData`/`FirebaseConfig`/`FirebaseAuth`,
  inicialización en el `setup()` y función para subir cada lectura a la
  Realtime Database).
- **Adaptación:** <!-- COMPLETAR: ej. "se ajustó la ruta /alimentos y el nodo
  /ultimo, los nombres de los campos (nombre, uid, fecha_ingreso, fecha_expiracion,
  duracion_dias, timestamp) y se conectó con la lógica de lectura RFID propia." -->
- **Comprensión:** el equipo entiende que la función arma un objeto JSON con los
  datos del alimento y lo envía con `Firebase.pushJSON()` a la ruta `/alimentos`,
  generando una clave única por registro, y sobrescribe `/ultimo` con la última
  lectura. <!-- AJUSTAR a su implementación real -->

### Dashboard web (estructura y lógica de tiempo real)
- **Herramienta:** Claude (Anthropic) / Claude Code, junio 2026
- **Uso:** generación de la página HTML/JS que se conecta a Firebase, escucha
  cambios con `onValue`, calcula la cuenta regresiva al vencimiento y clasifica
  por color de urgencia.
- **Adaptación:** <!-- COMPLETAR: ej. "se ajustaron los umbrales de color, el
  parseo de fecha DD/MM/YYYY y el orden por urgencia." -->
- **Comprensión:** el equipo entiende cómo el listener `onValue` actualiza la
  interfaz sin recargar, y cómo se calcula `fecha_expiracion - ahora`.

### Estructura del README y de este repositorio
- **Herramienta:** Claude (Anthropic), junio 2026
- **Uso:** generación del template base del README y la organización de carpetas;
  el contenido técnico específico del proyecto fue escrito/verificado por el equipo.

<!-- AGREGAR cualquier otro uso de IA: depuración de errores de compilación,
     ayuda con el modelado 3D, redacción del reporte, etc. Sean específicos. -->

---

> **Nota:** durante la presentación, cualquier integrante debe poder explicar qué
> hace cada función del código, haya sido escrita por el equipo o generada con IA.
> Declarar el uso de IA no exime de comprender el código.
