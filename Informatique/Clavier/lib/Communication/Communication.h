#ifndef CALCULATRICE_H
#define CALCULATRICE_H

#include <WiFi.h>
#include <esp_now.h>




typedef struct {
  char caractere; 
  char cmd;
  bool maj = false;
  bool ctrl = false;
} struct_message;

void EspNow_init();
void ChangeVolume(char c);
void Mute();
void TypeCtrl(bool c);
void TypeCarac(char c);
void TypeMaj(bool c);

#endif // CALCULATRICE_H