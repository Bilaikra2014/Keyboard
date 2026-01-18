#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x3C, 0xDC, 0x75, 0x63, 0x62, 0xE0};

typedef struct {
  char caractere; // On utilise char pour être cohérent
  bool maj = false;
} struct_message;

struct_message message;
esp_now_peer_info_t peerInfo;

void send_ascii(){
  for(int i = 97; i <= 122; i++){ // de 'a' à 'z'
    message.caractere = (char)i;
    Serial.print((char)i);
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    delay(50); // Augmenté à 50ms pour la stabilité HID
  }

  // char* tosend = R"(&é"'(-è_çà))";
  char* tosend = "1234567890";
  for(int i = 0; tosend[i]!='\0'; i++){ // de 'a' à 'z'
    message.caractere = tosend[i];
    Serial.print(tosend[i]);
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    delay(50); // Augmenté à 50ms pour la stabilité HID
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) return;

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) return;

  delay(2000); // Laisse le temps au récepteur d'être prêt
  // send_ascii();
  pinMode(15,INPUT);
  pinMode(16,INPUT);
}

void loop() {
  static bool lastBtn15 = LOW;
  static bool lastBtn16 = LOW;

  bool btn15 = digitalRead(15);
  bool btn16 = digitalRead(16);

  if (btn15 == HIGH && lastBtn15 == LOW) {
    Serial.println("ok");
    send_ascii();
  }

  if (btn16 == HIGH && lastBtn16 == LOW) {
    message.maj = !message.maj; // On bascule entre vrai et faux
    Serial.print("Mode Majuscule : ");
    Serial.println(message.maj ? "ON" : "OFF");
    delay(50); // Anti-rebond
}

  lastBtn15 = btn15;
  lastBtn16 = btn16;

}
