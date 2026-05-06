#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <USBHIDConsumerControl.h> // Ajouter ceci
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

unsigned long winPressTime = 0;
bool winActive = false;
bool comboUsed = false;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataRaw, int len) {
  uint16_t received;
  memcpy(&received, incomingDataRaw, sizeof(received));

  bool isReleased = (received & 0x8000);
  uint16_t keyCode = (received & 0x7FFF);

  // --- CAS DE LA TOUCHE WINDOWS ---
  if (keyCode == KEY_LEFT_GUI) {
    if (!isReleased) {
      winActive = true;
      comboUsed = false;
      winPressTime = millis();
      Keyboard.press(KEY_LEFT_GUI); // On prépare le modificateur
      Serial.println("Win pressé (en attente...)");
    } 
    else {
      winActive = false;
      Keyboard.release(KEY_LEFT_GUI); // On relâche le modificateur
      
      // Si on relâche Win sans avoir utilisé de raccourci (ex: Win+R)
      // et que l'appui était court (optionnel, ici on simplifie)
      if (!comboUsed) {
        Serial.println("Simple Tap détecté -> Envoi Ctrl+Esc");
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_ESC);
        delay(50);
        Keyboard.releaseAll();
      }
    }
    return;
  }

  // --- CAS DES AUTRES TOUCHES ---
  if (isReleased) {
    Keyboard.release(keyCode);
  } 
  else {
    if (winActive) {
      comboUsed = true; // On marque qu'un raccourci est en cours (ex: Win + R)
      Serial.println("Combinaison Windows + Touche détectée");
    }
    Keyboard.press(keyCode);
    
    // Si ce n'est pas une touche de modification (comme Shift), 
    // on peut relâcher immédiatement pour simuler une frappe
    if (keyCode != KEY_LEFT_SHIFT && keyCode != KEY_LEFT_CTRL && keyCode != KEY_LEFT_ALT) {
       delay(20);
       Keyboard.release(keyCode);
    }
  }
}
void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  delay(2000);
  ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
  ConsumerControl.release();
} 
void loop() {
}
