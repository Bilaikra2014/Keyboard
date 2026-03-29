#include "MyEncoder.h"

MyEncoder::MyEncoder(int up, int down, int btn){
    _up = up;
    _down = down;
    _btn = btn;
    // On enregistre la fonction passée en argument
}

void MyEncoder::init(){
    _ConsumerControl.begin();
    pinMode(16,INPUT);
    ESP32Encoder::useInternalWeakPullResistors = puType::up;// Active les résistances de pull-up internes
    _encoder.attachHalfQuad(_up,_down); // Compte deux fois moins d'étapes
    _encoder.setCount(0); // Initialise le compteur à 0
}

void MyEncoder::loop(){
    long rawValue = _encoder.getCount();
    if (abs(rawValue - (_lastEncoderValue * 2)) >= 2) { 
        if (rawValue > (_lastEncoderValue * 2)) {
            Serial.println("Commande : Volume -");
            _ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT); // Envoie l'ordre de baisser le son
            delay(10);
            _ConsumerControl.release();
        } else {
            Serial.println("Commande : Volume +");
            _ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT); // Envoie l'ordre Windows/Mac de monter le son
            delay(10);
            _ConsumerControl.release();
        }
        _lastEncoderValue = rawValue / 2;
    }
    bool read = digitalRead(_btn);
    Serial.println(read);
    if(read != _previousState){
        _previousState = read;
        if(read == false){
            _ConsumerControl.press(CONSUMER_CONTROL_MUTE);
            _ConsumerControl.release();
        }
    }
}