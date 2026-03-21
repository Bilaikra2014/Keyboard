#include <Arduino.h>
#include <Keypad.h>
#include <ESP32Encoder.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

// Instance du clavier USB
USBHIDKeyboard Keyboard;

// --- Configuration Encodeur ---
const int PIN_A = 16;  // CLK
const int PIN_B = 17;  // DT
const int PIN_SW = 18; // Bouton poussoir
long lastEncoderValue = 0;
ESP32Encoder encoder;

// --- Configuration Matrice (2x2) ---
const byte ROWS = 2;
const byte COLS = 2;
char keys[ROWS][COLS] = {
  {'w', 'x'},
  {'s', 'z'} 
};
byte rowPins[ROWS] = {4, 5};
byte colPins[COLS] = {6, 7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);  
  // Initialisation du clavier USB
  // Keyboard.begin();
  // USB.begin();
  ESP32Encoder::useInternalWeakPullResistors = puType::up;// Active les résistances de pull-up internes
  encoder.attachHalfQuad(PIN_A, PIN_B); // Compte deux fois moins d'étapes
  encoder.setCount(0); // Initialise le compteur à 0
}

void loop() {
  long rawValue = encoder.getCount();
  if (abs(rawValue - (lastEncoderValue * 2)) >= 2) { 
      if (rawValue > (lastEncoderValue * 2)) {
        Serial.println("Volume -");
        // ChangeVolume('-');
      } else {
        Serial.println("Volume +");
        // ChangeVolume('+');
      }
      lastEncoderValue = rawValue / 2;
  }
  // --- Gestion de la Matrice de touches ---
  // if (keypad.getKeys()) {
  //   for (int i=0; i<LIST_MAX; i++) {
  //     if (keypad.key[i].stateChanged) {
  //       switch (keypad.key[i].kstate) { 
  //         case PRESSED:
  //           // Appuie sur la touche sur l'ordinateur
  //           Keyboard.press(keypad.key[i].kchar);
  //           Serial.print("Appuyé: ");
  //           Serial.println(keypad.key[i].kchar);
  //           break;
            
  //         case RELEASED:
  //           // Relâche la touche sur l'ordinateur
  //           Keyboard.release(keypad.key[i].kchar);
  //           Serial.print("Relâché: ");
  //           Serial.println(keypad.key[i].kchar);
  //           break;

  //         case HOLD:
  //           break;
  //         case IDLE:
  //           break;
  //       }
  //     }
  //   }
  // }
}