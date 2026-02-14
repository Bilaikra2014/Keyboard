#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Keypad.h>
#include <ESP32Encoder.h>

unsigned long dernierAppui = 0;
const unsigned long DELAI_VEILLE = 5000; // 5 secondes

// Variables
typedef struct {
  char caractere; 
  char cmd;
  bool maj = false;
} struct_message;
const int PIN_A = 16;  // CLK
const int PIN_B = 17;  // DT
const int PIN_SW = 18; // Bouton poussoir (Switch)
long lastEncoderValue = 0;
const byte ROWS = 2;
const byte COLS = 2;
char keys[ROWS][COLS] = {
  {'a', 'b'},
  {'c', 'd'} 
};
byte rowPins[ROWS] = {4, 5};
byte colPins[COLS] = {6, 7};

// Configuration pour le deuxième clavier
const byte ROWS2 = 2; // On définit explicitement pour keypad2
const byte COLS2 = 2;
char keys2[ROWS2][COLS2] = {
  {'S', 'B'},
  {'V', 'D'}  
};
byte rowPins2[ROWS2] = {10, 11};
byte colPins2[COLS2] = {9,12}; // AJOUT DES CROCHETS ICI
bool shiftPresse = false;
bool globalCapsLock = false;
// Objets
ESP32Encoder encoder;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad keypad2 = Keypad(makeKeymap(keys2), rowPins2, colPins2, ROWS2, COLS2);


uint8_t broadcastAddress[] = {0x3C, 0xDC, 0x75, 0x63, 0x62, 0xE0};

struct_message message;
esp_now_peer_info_t peerInfo;

void ChangeVolume(char c){
  message.cmd = c;
  message.caractere = '\0';
  message.maj = '\0'; 
  esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
}

void TypeCarac(char c) {
    message.cmd = '\0';
    message.caractere = c;
    // On envoie l'état actuel du shift ou du caps lock
    message.maj = (shiftPresse || globalCapsLock); 
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
}

void TypeMaj(bool c) {
  message.cmd = '\0';
  message.caractere = '\0';
  message.maj = c;    
  esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));

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

  if (esp_now_add_peer(&peerInfo) != ESP_OK) return;gpio_wakeup_enable((gpio_num_t)4,  GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable((gpio_num_t)4,  GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable((gpio_num_t)5,  GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable((gpio_num_t)10, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable((gpio_num_t)11, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  
  dernierAppui = millis();
  // delay(2000);
}

void loop() {
//     // Gestion de l'encodeur 
//   //   Partie Rotative
  if (millis() - dernierAppui > DELAI_VEILLE) {
    Serial.println("Préparation au dodo...");
    
    // 1. Configurer les COLONNES en sorties à l'état BAS
    // C'est indispensable pour que le circuit se ferme quand on appuie sur une touche
    pinMode(6, OUTPUT);  digitalWrite(6, LOW);
    pinMode(7, OUTPUT);  digitalWrite(7, LOW);
    pinMode(9, OUTPUT);  digitalWrite(9, LOW);
    pinMode(12, OUTPUT); digitalWrite(12, LOW);

    // 2. Configurer les LIGNES en entrées PULLUP
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
    pinMode(PIN_SW, INPUT_PULLUP);

    delay(10); 

    // 3. Activer les sources de réveil (Lignes + Bouton encodeur)
    gpio_wakeup_enable((gpio_num_t)4,      GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable((gpio_num_t)5,      GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable((gpio_num_t)10,     GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable((gpio_num_t)11,     GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable((gpio_num_t)PIN_SW,  GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();

    Serial.println("Dodo...");
    Serial.flush();

    // 4. Sommeil
    esp_light_sleep_start();

    // --- REVEIL ICI ---
    
    // 5. REMETTRE LES COLONNES EN ENTRÉE pour redonner le contrôle à la librairie Keypad
    pinMode(6, INPUT); 
    pinMode(7, INPUT);
    pinMode(9, INPUT);
    pinMode(12, INPUT);

    dernierAppui = millis(); 
    Serial.println("Réveillé par une touche !");
    
    // On ne met PAS de gros delay() ici, pour laisser le loop() 
    // s'exécuter et détecter la touche via keypad.getKeys()
}
  long rawValue = encoder.getCount();
  if (abs(rawValue - (lastEncoderValue * 2)) >= 2) { 
      if (rawValue > (lastEncoderValue * 2)) {
        Serial.println("Volume -");
        ChangeVolume('-');
      } else {
        Serial.println("Volume +");
        ChangeVolume('+');
      }
      lastEncoderValue = rawValue / 2;
  }
    // Partie Bouton
  static bool lastBtnState = HIGH;
  bool currentBtnState = digitalRead(PIN_SW);
  if (lastBtnState == HIGH && currentBtnState == LOW) {
    Serial.println("Mute / Unmute");    
    message.cmd = 'm';
    message.caractere = '\0';
    esp_now_send(broadcastAddress, (uint8_t *)&message, sizeof(message));
    delay(50);
  }
  lastBtnState = currentBtnState;

  if (keypad.getKeys()) {
    dernierAppui = millis();
    for (int i=0; i<LIST_MAX; i++) { // LIST_MAX est par défaut à 10 touches
      if (keypad.key[i].stateChanged) { // Si l'état d'une touche a changé
        switch (keypad.key[i].kstate) { 
          case PRESSED:
            Serial.print("Appuyé: ");
            Serial.println(keypad.key[i].kchar); 
            TypeCarac(keypad.key[i].kchar);
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
  if (keypad2.getKeys()) {
    dernierAppui = millis();
    for (int i=0; i<LIST_MAX; i++) {
      if (keypad2.key[i].stateChanged) {
        char c = keypad2.key[i].kchar;
        switch (keypad2.key[i].kstate) { 
          case PRESSED:
            if (c == 'S') {
              shiftPresse = true;
              TypeMaj(true);
            } 
            else if (c == 'V') {
                globalCapsLock = !globalCapsLock; // Bascule l'état
                TypeMaj(globalCapsLock);          // Informe le récepteur (pour la LED et le mode)
                Serial.print("CapsLock dorenavant : ");
                Serial.println(globalCapsLock);
            }            
            else {
                TypeCarac(c); // Pour toutes les autres touches
            }
            break;

        case RELEASED:
            if (c == 'S') { 
              shiftPresse = false;
              TypeMaj(false);
            }

            break;
        }
      }
    }
  }
}

