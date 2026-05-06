#include <Arduino.h>
#include <Keypad.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "MyEncoder.h"
#include <esp_now.h>
#include <WiFi.h>
// --- Configuration Matrice (5x15) ---
const byte ROWS = 5;
const byte COLS = 15;
byte rowPins[ROWS] = {41, 5, 7, 38, 45};
byte colPins[COLS] = {15, 17, 18, 8, 3, 9, 11, 13, 21, 40, 39, 47, 14, 12, 10};

char keys[ROWS][COLS] = {
  {'\0','1','2','3','4','5','6','7','8','9','0','\0','=',KEY_BACKSPACE,KEY_DELETE},
  {KEY_TAB,'q','w','e','r','t','y','u','i','o','p','\0','\0','\0',KEY_RETURN},
  {KEY_CAPS_LOCK,'a','s','d','f','g','h','j','k','l',';','\0','\0',KEY_ESC},
  {KEY_LEFT_SHIFT,'<','z','x','c','v','b','n','m',',','.','/',KEY_PAGE_DOWN,KEY_UP_ARROW,KEY_PAGE_UP},
  {KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT,'\0',' ','\0',KEY_RIGHT_ALT,'\0','\0','\0','\0','\0',KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW}, 
};        

// Gestion de ESP-NOW
uint8_t DongleMacAdress[] = {0xDC,0xB4,0xD9, 0x14,0x42,0x18};
esp_now_peer_info_t infosRecepteur;                             
// Objets
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
MyEncoder encoder = MyEncoder(4,6,42);

void EncoderAction(int cmd) {
  ConsumerControl.press(cmd);
  delay(10);
  ConsumerControl.release();
}

void sendData(uint16_t data){
  esp_now_send(DongleMacAdress, (uint8_t *) &data, sizeof(data));
}

void setup() {
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erreur d'initialisation ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW Ok");
  memcpy(infosRecepteur.peer_addr, DongleMacAdress, 6);
  infosRecepteur.channel = 0;  
  infosRecepteur.encrypt = false;
  if (esp_now_add_peer(&infosRecepteur) != ESP_OK){
    Serial.println("Échec de l'appairage");
    return;
  }
  USB.begin();
  encoder.init();
  encoder.onIncrement(EncoderAction, CONSUMER_CONTROL_VOLUME_INCREMENT);
  encoder.onDecrement(EncoderAction, CONSUMER_CONTROL_VOLUME_DECREMENT);
  encoder.onPress(EncoderAction, CONSUMER_CONTROL_MUTE);
  Keyboard.begin();
  delay(2000);
}

void loop() {
  encoder.loop();
  if (keypad.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if (keypad.key[i].kchar != '\0' && keypad.key[i].stateChanged) {        
        uint8_t keyToSend = (uint8_t)keypad.key[i].kchar;
        switch(keypad.key[i].kstate){
          case PRESSED:
            Serial.print("Appui: ");
            Serial.println(keypad.key[i].kchar);
            // On envoie le code normal (bit 15 à 0)
            // sendData(keypad.key[i].kchar);
            Keyboard.press(keypad.key[i].kchar);
            Keyboard.release(keypad.key[i].kchar);
            break;
          case RELEASED:
            // CRUCIAL : On prévient le récepteur du relâchement pour SHIFT et GUI (Windows)
            // if(keypad.key[i].kchar == KEY_LEFT_SHIFT || keypad.key[i].kchar == KEY_LEFT_GUI) {
            //   Serial.print("Relâchement spécial: ");
            //   Serial.println(keypad.key[i].kchar);
            //   // On envoie le code avec le flag de relâchement (bit 15 à 1)
            //   sendData(keypad.key[i].kchar | 0x8000);
            // }
            break;
            
          case HOLD: 
            break;
        }
      }
    }
  }
}
