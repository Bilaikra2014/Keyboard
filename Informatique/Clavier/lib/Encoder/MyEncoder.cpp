#include "MyEncoder.h"

MyEncoder::MyEncoder(int up, int down, int btn) : _up(up), _down(down), _btn(btn) {}

void MyEncoder::init(){
    _ConsumerControl.begin();
    pinMode(_btn, INPUT_PULLUP); // Utilisation du pin bouton passé en paramètre
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    _encoder.attachHalfQuad(_up, _down);
    _encoder.setCount(0);
}

// Implémentation des setters de callbacks
void MyEncoder::onIncrement(EncoderCallback cb, int command) { _onUp = cb; _cmdUp = command; }
void MyEncoder::onDecrement(EncoderCallback cb, int command) { _onDown = cb; _cmdDown = command; }
void MyEncoder::onPress(EncoderCallback cb, int command) { _onBtn = cb; _cmdBtn = command; }

void MyEncoder::loop(){
    long rawValue = _encoder.getCount();
    
    if (abs(rawValue - (_lastEncoderValue * 2)) >= 2) { 
        if (rawValue > (_lastEncoderValue * 2)) {
            if (_onDown) _onDown(_cmdDown);
        } else {
            if (_onUp) _onUp(_cmdUp);
        }
        _lastEncoderValue = rawValue / 2;
    }

    bool read = digitalRead(_btn);
    if(read != _previousState){
        _previousState = read;
        if(read == LOW){
            if (_onBtn) _onBtn(_cmdBtn);
        }
    }
}