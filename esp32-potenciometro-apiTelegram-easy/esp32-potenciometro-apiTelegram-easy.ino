#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Configuración de Wi-Fi
const char* ssid = "LarryBot Wi-Fi";       // Reemplaza con tu red Wi-Fi
const char* password = "suscribete"; // Reemplaza con tu contraseña

// Configuración de Telegram
const char* botToken = "7905386912:AAHOpdp_e8ZS7IMIW6pgxMWVIOtFtCDxSKY";  // Token del bot de Telegram
const String chatID = "1249279984";        // ID del chat de Telegram

// Configuración del potenciómetro
const int pinPotenciometro = 32;  // Pin analógico del potenciómetro
int valorAnterior = -1;           // Para evitar mensajes repetidos

// Cliente seguro y bot de Telegram
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

void setup() {
  Serial.begin(115200);
  pinMode(pinPotenciometro, INPUT);

  // Conectar a la red Wi-Fi
  Serial.println("Conectando a la red Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado a Wi-Fi.");
  Serial.println("Dirección IP: " + WiFi.localIP().toString());

  // Configurar cliente seguro para Telegram
  client.setInsecure();

  // Mensaje inicial en Telegram
  bot.sendMessage(chatID, "🤖 ESP32 conectado correctamente. Dirección IP: " + WiFi.localIP().toString(), "");
}

void loop() {
  // Leer valor del potenciómetro (0 a 4095) y convertir a porcentaje
  int valorPot = analogRead(pinPotenciometro);
  int porcentaje = map(valorPot, 0, 4095, 0, 100);

  Serial.printf("Valor: %d | Porcentaje: %d%%\n", valorPot, porcentaje);

  // Verificar si el valor cambió y enviar alerta si es necesario
  if (porcentaje != valorAnterior) {
    if (porcentaje >= 100) {
      bot.sendMessage(chatID, "🚨 El potenciómetro está al 100%.", "");
    } 
    else if (porcentaje <= 10) {
      bot.sendMessage(chatID, "⚠️ El potenciómetro está por debajo del 10%.", "");
    }
    valorAnterior = porcentaje;  // Actualizar valor anterior
  }

  delay(5000);  // Verificar cada 5 segundos
}
