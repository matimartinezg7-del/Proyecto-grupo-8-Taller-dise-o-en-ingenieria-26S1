# Heladera Inteligente IoT — Control de Vencimientos por RFID

Sistema IoT que registra alimentos mediante etiquetas RFID y avisa antes de que
se venzan, para reducir el desperdicio de comida en el hogar. Cada vez que se
escanea un alimento, el sistema lo guarda en la nube, envía una notificación por
Telegram y lo muestra en un dashboard web en tiempo real con cuenta regresiva
hasta el vencimiento.

**Curso:** TEI201 — Taller de Diseño en Ingeniería · Design Engineering Center · UAI

## Integrantes del equipo

<!-- COMPLETAR: reemplazar con los nombres reales del equipo -->
- Nombre Apellido — rol
- Nombre Apellido — rol
- Nombre Apellido — rol

## Problema que resuelve (referencia al Avance 1)

<!-- COMPLETAR/AJUSTAR con el problema exacto que plantearon en el Avance 1 -->
En el Avance 1 identificamos que en los hogares se desperdicia una cantidad
significativa de alimentos porque las personas no tienen visibilidad de qué
productos están próximos a vencer dentro del refrigerador. Los alimentos quedan
"escondidos" al fondo y se descubren cuando ya no sirven.

Nuestro sistema captura el ingreso de cada alimento (vía RFID), calcula su fecha
de vencimiento y genera información accionable —una lista priorizada por urgencia
y alertas anticipadas— que permite al usuario decidir qué consumir primero.

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

Ver la lista completa con costos en [`hardware/BOM.csv`](hardware/BOM.csv).
En resumen: una placa NodeMCU ESP8266, un lector RFID-RC522, etiquetas RFID,
cables y fuente de alimentación USB.

## Estructura del repositorio

```
heladera-iot-tei201/
├── README.md                  ← este archivo
├── FUENTES.md                 ← declaración de librerías, código externo e IA (obligatorio)
├── firmware/                  ← código del ESP8266
│   ├── heladera_rfid.ino
│   └── libraries/             ← librerías necesarias (ver lista abajo)
├── hardware/                  ← esquemático y lista de materiales
│   ├── esquematico.png
│   └── BOM.csv
├── diseno-3d/                 ← modelo del encapsulado
│   ├── encapsulado.f3d        ← (subir export desde Fusion 360)
│   ├── planos.pdf
│   └── renders/
├── testing/                   ← protocolo de pruebas y evidencia
│   ├── protocolo_pruebas.md
│   └── datos/
├── docs/                      ← documentación y reporte
│   └── reporte_final.pdf
└── dashboard/                 ← (si el dashboard está en este repo) o ver link abajo
```

## Instrucciones para cargar el firmware

1. Instalar el **Arduino IDE** y agregar el soporte para placas ESP8266
   (Gestor de placas → URL `http://arduino.esp8266.com/stable/package_esp8266com_index.json`).
2. Instalar las librerías listadas en `FUENTES.md` desde el Library Manager.
3. Abrir `firmware/heladera_rfid.ino`.
4. Completar las credenciales en la sección de configuración:
   - WiFi (`ssid`, `password`)
   - Telegram (`BOT_TOKEN`, `CHAT_ID`)
   - Firebase (`FIREBASE_HOST`, `FIREBASE_AUTH`)
   > Estas credenciales NO deben subirse al repositorio. Ver `firmware/config.example.h`.
5. Seleccionar la placa **NodeMCU 1.0 (ESP-12E Module)** y el puerto correcto.
6. Cargar el sketch y abrir el Monitor Serial a 115200 baudios para verificar.

## Dashboard

<!-- COMPLETAR con el link real una vez desplegado en Firebase Hosting -->
- **Link en vivo:** https://TU-PROYECTO.web.app
- **Tecnología:** HTML + JavaScript (SDK de Firebase v9 modular), lectura en
  tiempo real con `onValue`.
- **Funciones:** lista de alimentos en tiempo real, cuenta regresiva hasta el
  vencimiento, semáforo por urgencia (verde >3 días, amarillo 1–3 días, rojo
  <1 día o vencido) y panel de recomendaciones por frecuencia de ingreso.

Para ejecutarlo localmente: abrir `dashboard/index.html` en el navegador
(no requiere servidor ni build).

## Estado del proyecto

Prototipo funcional — Avance #3 (evaluación final del ramo).
