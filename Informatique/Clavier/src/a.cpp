#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Keypad.h>
#include <ESP32Encoder.h>
ESP32Encoder encoder;


const int PIN_A = 16;  // CLK
const int PIN_B = 17;  // DT
const int PIN_SW = 18; // Bouton poussoir (Switch)
long lastEncoderValue = 0;

const byte ROWS = 2; // 2 lignes
const byte COLS = 2; // 1 colonne

// Définition des symboles des touches
char keys[ROWS][COLS] = {
  {'A', 'B'}, // Ligne 1 (Boutons 1 et 2)
  {'C', 'D'}  // Ligne 2 (Boutons 3 et 4)
};

// Vos broches mises à jour
byte rowPins[ROWS] = {4, 5};   // Connectées aux lignes (R1, R2)
byte colPins[COLS] = {6, 10};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

uint8_t broadcastAddress[] = {0x3C, 0xDC, 0x75, 0x62, 0x8A, 0x80};

typedef struct {
  char caractere; 
  char cmd;
  // bool maj = false;
} struct_message;

struct_message message;
esp_now_peer_info_t peerInfo;

void send_ascii(){
  for(int i = 97; i <= 122; i++){ 
    message.caractere = (char)i;
    Serial.print((char)i);
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    delay(50);
  }

  // char* tosend = R"(&é"'(-è_çà))";
  char* tosend = "1234567890";
  for(int i = 0; tosend[i]!='\0'; i++){
    message.caractere = tosend[i];
    Serial.print(tosend[i]);
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    delay(50); // Augmenté à 50ms pour la stabilité HID
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  
  ESP32Encoder::useInternalWeakPullResistors = puType::up; 
  encoder.attachHalfQuad(PIN_A, PIN_B);
  encoder.setCount(0);

  pinMode(PIN_SW, INPUT_PULLUP);
  if (esp_now_init() != ESP_OK) return;

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) return;

  delay(2000); // Laisse le temps au récepteur d'être prêt
  // send_ascii();
}

void loop() {
  // Gestion de l'encodeur 
    // Partie Rotative
  long rawValue = encoder.getCount();
  // On ne réagit que si l'encodeur a bougé de 2 unités
  if (abs(rawValue - (lastEncoderValue * 2)) >= 2) { 
      if (rawValue > (lastEncoderValue * 2)) {
        Serial.println("Volume -");
        message.cmd = '-';
        esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
      } else {
        Serial.println("Volume +");
        message.cmd = '+';
        esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
      }
      lastEncoderValue = rawValue / 2;
  }
    // Partie Bouton
  static bool lastBtnState = HIGH;
  bool currentBtnState = digitalRead(PIN_SW);
  if (lastBtnState == HIGH && currentBtnState == LOW) {
    Serial.println("Mute / Unmute");    
    message.cmd = 'm';
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    delay(50);
  }
  lastBtnState = currentBtnState;

  if (keypad.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) { // LIST_MAX est par défaut à 10 touches
      if (keypad.key[i].stateChanged) { // Si l'état d'une touche a changé
        switch (keypad.key[i].kstate) { 
          case PRESSED:
            Serial.print("Appuyé: ");
            Serial.println(keypad.key[i].kchar);            
            message.cmd = NULL;
            message.caractere = keypad.key[i].kchar;
            esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
            break;
          case RELEASED:
            Serial.print("Relâché: ");
            Serial.println(keypad.key[i].kchar);
            break;
          case HOLD:
            // Optionnel : gérer l'appui long
            break;
        }
      }
    }
  }



  // char key = keypad.getKey();
  // if (key) {
  //   message.cmd = NULL;
  //   message.caractere = key;
  //   Serial.print(key);
  //   esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
  // }
  // else 
  //   message.caractere = NULL;
}
