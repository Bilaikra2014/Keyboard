#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

USBHIDKeyboard Keyboard;

typedef struct {
  char caractere;
  bool maj; 
} struct_message;

struct_message incomingData;

char _convert(uint8_t c){
  switch(c){
    case 'a': return 'q'; // Pour afficher 'a', on doit "taper" sur la touche 'q'
    case 'q': return 'a'; // Pour afficher 'q', on doit "taper" sur la touche 'a'
    case 'z': return 'w'; // Pour afficher 'z', on doit "taper" sur la touche 'w'
    case 'w': return 'z'; // Pour afficher 'w', on doit "taper" sur la touche 'z'
    case 'm': return ';'; // Pour afficher 'm', on doit "taper" sur la touche ','
    case ',': return 'm'; // Inversement
    case ':': return 'M'; // Souvent nécessaire pour la ponctuation
    default:  return c;
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataRaw, int len) {
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));
  Keyboard.write(_convert(incomingData.caractere)); 
  // if (incomingData.maj) {
  //   Keyboard.press(KEY_LEFT_SHIFT);
  // } else {
  //   Keyboard.release(KEY_LEFT_SHIFT);
  // }
  // Keyboard.write(incomingData.caractere); 
}

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  USB.begin();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  delay(2000);

  // Keyboard.press(KEY_LEFT_CTRL);
  // Keyboard.press('x');
  // Keyboard.releaseAll();
  // Keyboard.press(KEY_LEFT_CTRL);
  // Keyboard.press('v');
  // Keyboard.releaseAll();  
}

void loop() {
  if(incomingData.maj == true){
    Keyboard.press(KEY_LEFT_SHIFT);
  }
  else {
    Keyboard.release(KEY_LEFT_SHIFT);
  }
}