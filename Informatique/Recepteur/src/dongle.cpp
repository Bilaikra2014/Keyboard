#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <USBHIDConsumerControl.h> // Ajouter ceci
USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;
bool MajState = false;
bool previousMajState = true;

bool CTRLState = false;
bool previousCTRLState = true;

typedef struct {
  char caractere; 
  char cmd;
  bool maj = false;
  bool ctrl = false;
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
  MajState = incomingData.maj;
  CTRLState = incomingData.ctrl;
  // 1. Gérer les touches multimédia (Volume/Mute)
  if (incomingData.cmd != '\0') {
    uint16_t cmd_val = 0;
    switch(incomingData.cmd){
      case '+': cmd_val = CONSUMER_CONTROL_VOLUME_INCREMENT; break;
      case '-': cmd_val = CONSUMER_CONTROL_VOLUME_DECREMENT; break;
      case 'm': cmd_val = CONSUMER_CONTROL_MUTE; break;
    }
    if (cmd_val != 0) {
      ConsumerControl.press(cmd_val);
      ConsumerControl.release();
    }
  }

  if (incomingData.maj) {
      Keyboard.press(KEY_LEFT_SHIFT);
  } else {
      Keyboard.release(KEY_LEFT_SHIFT);
  }
  if(incomingData.ctrl){
    Keyboard.press(KEY_LEFT_CTRL);
  } else {
    Keyboard.release(KEY_LEFT_CTRL);
  }

  // 2. Gérer le clavier (Caractère + Shift)
  if (incomingData.caractere != '\0') {    
    Keyboard.write(_convert(incomingData.caractere));
    Keyboard.releaseAll();
  }  
}

void setup() {
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(4,LOW);
  digitalWrite(4,LOW);
  Serial.begin(115200);
  Keyboard.begin();
  ConsumerControl.begin();
  USB.begin();
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  delay(2000);
}

void loop() {
  if(MajState != previousMajState){
    digitalWrite(4,MajState);
    previousMajState = MajState;
  }
  if(CTRLState != previousCTRLState){
    digitalWrite(5,CTRLState);
    previousCTRLState = CTRLState;
  }
}