#ifndef ENCODER_H
#define ENCODER_H

#include <ESP32Encoder.h>
#include "USBHIDConsumerControl.h"
#include "Arduino.h"
class MyEncoder {
public:
    MyEncoder(int up, int down, int btn);
    void init();
    void loop(); 

private:
    int _up;
    int _down;
    int _btn;
    long _lastEncoderValue = 0;  
    bool _previousState = true;
    bool _SoundState = true;
    ESP32Encoder _encoder;    
    USBHIDConsumerControl _ConsumerControl;
};

#endif // ENCODER_H