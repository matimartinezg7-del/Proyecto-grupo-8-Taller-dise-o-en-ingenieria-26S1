# IoT — Control de Vencimientos por RFID

Sistema IoT que registra alimentos mediante etiquetas RFID y permite monitorear cuanto tiempo queda para su vencimiento, para reducir el desperdicio de comida en el hogar. Cada vez que se
escanea un alimento, el sistema lo guarda en la nube y lo muestra en un dashboard web en tiempo real con cuenta regresiva
hasta el vencimiento.

**Curso:** TEI201 — Taller de Diseño en Ingeniería · UAI

## Integrantes 
Colomba Prado
Santiago Seguro
Matias Martinez
## Problema que resuelve (referencia al Avance 1)

En el Avance 1 identificamos que en los hogares se desperdicia una cantidad
significativa de alimentos especialmente en el caso de personas que viven solas.

Nuestro sistema captura el ingreso de cada alimento (vía RFID), calcula su fecha
de vencimiento y genera información accionable —una lista priorizada por urgencia que permite al usuario decidir qué consumir primero, ademas de presentar estadisticas y recomendaciones de consuma en base a los datos almacenados de consumo historico y los alimentos que estan almacenados en ese momento

## Cómo funciona (ciclo de datos)

```
Etiqueta RFID  →  ESP8266 + RC522  →  Firebase Realtime DB  →  Dashboard web
 (captura)         (lectura)           (almacenamiento)          (información)
                        │
                        └──→  Notificación Telegram (alerta)
```

1. **Captura:** se acerca un alimento con su etiqueta RFID al lector RC522.
2. **Procesa:** el ESP8266 identifica el alimento, registra fecha de ingreso y
   calcula la fecha de vencimiento según su duración estimada. 
3. **Almacena:** sube el registro a Firebase Realtime Database (histórico persistente).
4. **Informa:** el dashboard web muestra la lista en tiempo real, con cuenta
   regresiva y semáforo de urgencia (verde / amarillo / rojo), y envía un aviso
   por Telegram en cada ingreso.

## Componentes necesarios para replicar el hardware
Placa NodeMCU ESP8266
Lector RFID-RC522
Tags RFID
Cables
Protoboard
Fuente de alimentacion micro-USB
En resumen: una placa NodeMCU ESP8266, un lector RFID-RC522, etiquetas RFID,
cables y fuente de alimentación USB.

## Instrucciones para cargar el firmware

1. Instalar el programa **Arduino IDE** y agregar el soporte para placas ESP8266
   (Gestor de placas → URL `http://arduino.esp8266.com/stable/package_esp8266com_index.json`).
2. Instalar las librerías listadas en `FUENTES.md` desde el Library Manager.
3. Abrir `firmware/heladera_rfid.ino`.
4. Completar las credenciales en la sección de configuración:
   - WiFi (`ssid`, `password`)
   - Telegram (`BOT_TOKEN`, `CHAT_ID`)
   - Firebase (`FIREBASE_HOST`, `FIREBASE_AUTH`)
5. Seleccionar la placa **NodeMCU 1.0 (ESP-12E Module)** y el puerto correcto.
6. Cargar el sketch y abrir el Monitor Serial a 115200 baudios para verificar.

## Dashboard
## Dashboard

El dashboard es una página web (HTML + CSS + JavaScript) que se conecta a la
Firebase Realtime Database y muestra los alimentos registrados en tiempo real,
con cuenta regresiva hasta el vencimiento y semáforo de urgencia.

### Cómo ejecutarlo

El dashboard se sirve localmente con un servidor estático. Necesitas tener
**Python** instalado y **conexión a internet** (los datos vienen de Firebase
en la nube).

1. Abre una terminal dentro de la carpeta del dashboard (`rfid-dashboard`,
   donde está `index.html`).
2. Levanta el servidor local: python -m http.server 5500
3. Abre el navegador en: http://localhost:5500
4. Deja la terminal abierta mientras usás el dashboard. Si la cierras, el
   servidor se apaga y la página deja de cargar.

> **Nota:** aunque el dashboard corre en local, los datos se leen desde Firebase,
> por lo que se requiere internet para ver la información en tiempo real.

