#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0x3C, 0xDC, 0x75, 0x63, 0x62, 0xE0};

// Codes internes pour accents
#define E_ACUTE 1   // é
#define E_GRAVE 2   // è
#define A_GRAVE 3   // à
#define C_CEDILLE 4 // ç

typedef struct {
  uint8_t code;
} struct_message;

struct_message message;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Envoi OK" : "Envoi NOK");
}

void setup() {
  Serial.begin(115200);

  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erreur ajout peer");
    return;
  }
}

void loop() {
  static bool lastBtn15 = HIGH;
  static bool lastBtn16 = HIGH;

  bool btn15 = digitalRead(15);
  bool btn16 = digitalRead(16);

  if (btn15 == LOW && lastBtn15 == HIGH) {
    message.code = 'A';
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
  }

  if (btn16 == LOW && lastBtn16 == HIGH) {
    message.code = 'B'; // caractère normal
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
  }

  lastBtn15 = btn15;
  lastBtn16 = btn16;

}
