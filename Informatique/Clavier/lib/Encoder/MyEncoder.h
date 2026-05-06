#ifndef ENCODER_H
#define ENCODER_H

#include <ESP32Encoder.h>
#include "USBHIDConsumerControl.h"
#include "Arduino.h"
#include <functional>

typedef std::function<void(int)> EncoderCallback;

class MyEncoder {
public:
    MyEncoder(int up, int down, int btn);
    void init();
    void loop(); 

    void onIncrement(EncoderCallback cb, int command);
    void onDecrement(EncoderCallback cb, int command);
    void onPress(EncoderCallback cb, int command);

private:
    int _up, _down, _btn;
    long _lastEncoderValue = 0;  
    bool _previousState = true;
    
    ESP32Encoder _encoder;    
    USBHIDConsumerControl _ConsumerControl;

    // Stockage des callbacks
    EncoderCallback _onUp = nullptr;
    EncoderCallback _onDown = nullptr;
    EncoderCallback _onBtn = nullptr;
    
    int _cmdUp, _cmdDown, _cmdBtn;
};

#endif // ENCODER_H