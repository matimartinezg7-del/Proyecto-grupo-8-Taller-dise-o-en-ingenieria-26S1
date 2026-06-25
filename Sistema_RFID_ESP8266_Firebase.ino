#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>

// Firebase (instalar libreria "Firebase ESP Client" de Mobizt desde el Library Manager)
#include <FirebaseESP8266.h>

// -------- WIFI --------
const char* ssid = "iPhone de mati";
const char* password = "mati9952";

// -------- TELEGRAM --------
#define BOT_TOKEN "8689745546:AAF3F2nJPecoRRT1sVCOu-ghOBwJXIfDqPE"
#define CHAT_ID "8462815235"

// -------- FIREBASE --------
#define FIREBASE_HOST "proyecto-rfid-4d86f-default-rtdb.firebaseio.com"

#define FIREBASE_AUTH "J5awsCQTrZiGFMRRmmokmuQEfAd6iWyTg267rNHY"

FirebaseData fbdo;
FirebaseConfig fbConfig;
FirebaseAuth fbAuth;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Hora por internet
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -4 * 3600;
const int daylightOffset_sec = 0;

// -------- RFID (pines NodeMCU) --------
// SS  -> D2 (GPIO4)
// RST -> D1 (GPIO5)
// SCK -> D5, MISO -> D6, MOSI -> D7 (SPI por hardware)
#define SS_PIN  D2
#define RST_PIN D1
MFRC522 mfrc522(SS_PIN, RST_PIN);

// -------- ESTRUCTURA --------
struct Alimento {
  String nombre;
  int duracionHoras;
  unsigned long tiempoIngreso;
  bool activo;
  bool alertaEnviada;
};

Alimento alimentos[10];

struct AlimentoInfo {
  String uid;
  String nombre;
  int duracionDias;
};

// -------- UID DE TAGS --------
AlimentoInfo listaAlimentos[] = {
  {"04 43 88 4A BA 2A 81", "Leche", 3},
  {"04 39 88 4A BA 2A 81", "Lechuga", 5},
  {"04 45 88 4A BA 2A 81", "Yogurt", 7},
  {"04 44 88 4A BA 2A 81", "Palta", 1},
  {"04 42 88 4A BA 2A 81", "Manzana", 2}
};

int cantidadAlimentos = sizeof(listaAlimentos) / sizeof(listaAlimentos[0]);

// -------- CONVERTIR UID --------
String getUID() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(mfrc522.uid.uidByte[i], HEX);
    if (i != mfrc522.uid.size - 1) uid += " ";
  }
  uid.toUpperCase();
  return uid;
}

// -------- FORMATEAR FECHA --------
String formatearFecha(time_t tiempo) {
  struct tm* fecha = localtime(&tiempo);
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", fecha);
  return String(buffer);
}

// -------- ENVIAR A FIREBASE --------
void enviarAFirebase(String uid, String nombre, time_t ingreso, time_t expira, int dias) {
  // Cada lectura se guarda como un nodo nuevo bajo /alimentos con push (clave unica automatica)
  FirebaseJson json;
  json.set("uid", uid);
  json.set("nombre", nombre);
  json.set("fecha_ingreso", formatearFecha(ingreso));
  json.set("fecha_expiracion", formatearFecha(expira));
  json.set("duracion_dias", dias);
  json.set("timestamp", (int)ingreso); // epoch, util para ordenar en el dashboard

  if (Firebase.pushJSON(fbdo, "/alimentos", json)) {
    Serial.println("Firebase OK: " + fbdo.dataPath());
  } else {
    Serial.println("Firebase ERROR: " + fbdo.errorReason());
  }

  // Ademas, guardamos el "ultimo leido" en un nodo fijo para acceso rapido
  Firebase.setJSON(fbdo, "/ultimo", json);
}

// -------- REGISTRAR ALIMENTO --------
void registrarAlimento(String uid) {
  for (int i = 0; i < cantidadAlimentos; i++) {
    if (uid == listaAlimentos[i].uid) {

      time_t ahora;
      time(&ahora);

      time_t fechaExpiracion = ahora + ((time_t)listaAlimentos[i].duracionDias * 24 * 60 * 60);

      String mensaje = "📥 Alimento ingresado\n\n";
      mensaje += "Alimento: " + listaAlimentos[i].nombre + "\n";
      mensaje += "Fecha ingreso: " + formatearFecha(ahora) + "\n";
      mensaje += "Expira aprox: " + formatearFecha(fechaExpiracion) + "\n";
      mensaje += "Duración estimada: " + String(listaAlimentos[i].duracionDias) + " días";

      bool enviado = bot.sendMessage(CHAT_ID, mensaje, "");
      Serial.println(enviado ? "Mensaje enviado:" : "Fallo al enviar:");
      Serial.println(mensaje);

      // ----- Subir a Firebase -----
      enviarAFirebase(uid, listaAlimentos[i].nombre, ahora, fechaExpiracion, listaAlimentos[i].duracionDias);

      return;
    }
  }

  bot.sendMessage(CHAT_ID, "❓ Tag no reconocido\nUID: " + uid, "");
  Serial.println("Tag no reconocido: " + uid);
}

// -------- REVISAR EXPIRACION --------
void revisarAlimentos() {
  for (int i = 0; i < 10; i++) {
    if (alimentos[i].activo) {

      unsigned long tiempoPasado = millis() - alimentos[i].tiempoIngreso;
      int horasPasadas = tiempoPasado / 3600000UL;
      int horasRestantes = alimentos[i].duracionHoras - horasPasadas;

      if (horasRestantes <= 24 && horasRestantes > 0 && !alimentos[i].alertaEnviada) {
        bot.sendMessage(CHAT_ID,
          "⚠ " + alimentos[i].nombre + " expira en " + String(horasRestantes) + " horas", "");
        alimentos[i].alertaEnviada = true;
      }

      if (horasRestantes <= 0) {
        bot.sendMessage(CHAT_ID,
          "❌ " + alimentos[i].nombre + " EXPIRADO", "");
        alimentos[i].activo = false;
      }
    }
  }
}

// -------- WIFI CON TIMEOUT Y REINTENTO --------
void conectarWiFi() {
  Serial.print("Conectando WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado, IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nNo se pudo conectar WiFi, reintentando...");
    delay(2000);
    conectarWiFi();
  }
}

// -------- SETUP --------
void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  conectarWiFi();

  client.setInsecure(); // necesario para Telegram HTTPS

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sincronizando hora...");

  time_t ahora = time(nullptr);
  int intentosHora = 0;
  while (ahora < 100000 && intentosHora < 20) {
    delay(500);
    Serial.print(".");
    ahora = time(nullptr);
    intentosHora++;
  }
  Serial.println(ahora > 100000 ? "\nHora sincronizada" : "\nNo se pudo sincronizar la hora");

  // ----- Inicializar Firebase -----
  fbConfig.database_url = String("https://") + FIREBASE_HOST;
  fbConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase inicializado");

  bool enviado = bot.sendMessage(CHAT_ID, "🤖 Sistema RFID iniciado", "");
  Serial.println(enviado ? "Mensaje de inicio enviado" : "Fallo al enviar mensaje de inicio");
}

// -------- LOOP --------
void loop() {

  // Reconectar si se cae el WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado, reconectando...");
    conectarWiFi();
  }

  // Detectar tarjeta
  if (mfrc522.PICC_IsNewCardPresent() &&
      mfrc522.PICC_ReadCardSerial()) {

    String uid = getUID();
    Serial.println("UID: " + uid);

    registrarAlimento(uid);

    delay(1500);
  }

  revisarAlimentos();
}
