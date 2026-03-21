#include "Communication.h"

bool shiftPresse = false;
bool globalCapsLock = false;
bool Ctrl = false;

struct_message message;
esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0x3C, 0xDC, 0x75, 0x63, 0x62, 0xE0};



void EspNow_init(){
  if (esp_now_init() != ESP_OK) return;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) return;gpio_wakeup_enable((gpio_num_t)4,  GPIO_INTR_LOW_LEVEL);  
}

void ChangeVolume(char c){
  message.cmd = c;
  message.caractere = '\0';
  message.ctrl = Ctrl;
  message.maj = '\0'; 
  esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
}

void Mute(){
    message.cmd = 'm';
    message.caractere = '\0';
    message.ctrl = Ctrl;
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
}

void TypeCtrl(bool c){
    Ctrl = c;

}

void TypeCarac(char c) {
    message.cmd = '\0';
    message.caractere = c;
    message.maj = (shiftPresse || globalCapsLock); 
    message.ctrl = Ctrl;
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
}

void TypeMaj(bool c) {
  message.cmd = '\0';
  message.caractere = '\0';
  message.maj = c;    
  message.ctrl = Ctrl;
  esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
}
