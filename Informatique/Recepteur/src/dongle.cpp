#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

USBHIDKeyboard Keyboard;

// Structure pour recevoir les données (doit être identique à l'émetteur)
typedef struct struct_message {
    char texte[32]; // Pour recevoir un mot ou une phrase
} struct_message;

struct_message incomingData;

// Fonction de rappel (Callback) : exécutée quand on reçoit quelque chose
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataRaw, int len) {
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));
  Serial.print("Caractère(s) reçu(s) : ");
  Serial.println(incomingData.texte[0]);

  // Envoie les caractères reçus comme s'ils venaient d'un clavier USB
  Keyboard.print(incomingData.texte[0]);
}

void setup() {
  Serial.begin(115200);  
  Keyboard.begin();
  USB.begin();
  WiFi.mode(WIFI_STA);
  Serial.print("Adresse MAC : ");
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur d'initialisation ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));  
  Serial.println("Prêt à recevoir des données...");
}

void loop() {
}