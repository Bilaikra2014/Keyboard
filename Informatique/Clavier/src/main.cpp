#include <Arduino.h>
#include <Keypad.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "MyEncoder.h"


// --- Configuration Matrice (5x15) ---
const byte ROWS = 5;
const byte COLS = 15;


char keys[ROWS][COLS] = {
  // Ligne 1 : Les chiffres (en QWERTY, pas besoin de Shift pour les chiffres 123...)
  {'\0','1','2','3','4','5','6','7','8','9','0','\0','=',KEY_BACKSPACE,KEY_DELETE},
  
  // Ligne 2 : Remplace 'a' par 'q' et 'z' par 'w'
  {KEY_RETURN,'q','w','e','r','t','y','u','i','o','p','\0','\0','\0',KEY_RETURN},
  
  // Ligne 3 : Remplace 'q' par 'a' et 'm' par ';' (Le M AZERTY est le ; QWERTY)
  {KEY_LEFT_SHIFT,'a','s','d','f','g','h','j','k','l',';','*','\0','\0'},
  
  // Ligne 4 : Remplace 'w' par 'z', ',' par 'm', ';' par ',', ':' par '.'
  {KEY_LEFT_SHIFT,'<','z','x','c','v','b','n','m',',','.','/',KEY_PAGE_DOWN,KEY_UP_ARROW,KEY_PAGE_UP},
  
  // Ligne 5
  {KEY_LEFT_CTRL,'\0',KEY_LEFT_ALT,'\0',' ','\0',KEY_RIGHT_ALT,'\0','\0','\0','\0','\0',KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW}, 
};        

// byte rowPins[ROWS] = {4,5,6,7,15};
// byte colPins[COLS] = {16,17,18,8,3,46,9,10,11,12,13,1,2,42,41};

byte rowPins[ROWS] = {41, 5, 7, 38, 45};
byte colPins[COLS] = {15, 17, 18, 8, 3, 9, 11, 13, 21, 40, 39, 47, 14, 12, 10};

// Objets
USBHIDKeyboard Keyboard;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

MyEncoder encoder = MyEncoder(6,4,16);

void setup() {
  Serial.begin(115200);  
  // Initialisation du clavier USB
  USB.begin();
  encoder.init();
  Keyboard.begin();
  delay(2000);
}

void loop() {
  encoder.loop();
  if (keypad.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {
      if (keypad.key[i].kchar != '\0' && keypad.key[i].stateChanged) {
        
        uint8_t keyToSend = (uint8_t)keypad.key[i].kchar;

        if (keypad.key[i].kstate == PRESSED) {
          Serial.print("Touche physique pressée: ");
          Serial.println(keypad.key[i].kchar);
          Keyboard.press(keyToSend);
        }
        
        if (keypad.key[i].kstate == RELEASED) {
          if (keyToSend == KEY_LEFT_SHIFT || keyToSend == KEY_LEFT_CTRL || keyToSend == KEY_LEFT_ALT || keyToSend == KEY_RIGHT_ALT) {            
            Serial.println("Modifier relâché -> Reset global pour sécurité");
            Keyboard.releaseAll(); 
          }else{
            Keyboard.release(keyToSend);
          }
        }
      }
    }
  }
}
