## 📡 Protocoles de Communication

### Communication Sans-fil ([ESP-NOW](https://wiki-fablab.grandbesancon.fr/doku.php?id=howto:arduino-esp:esp-now#:~:text=ESP%2DNOW%20est%20un%20protocole,de%20donn%C3%A9es%20directement%20et%20simplement.))
Pour minimiser la latence, le projet utilise **ESP-NOW**, un protocole de connexion sans fil à basse consommation d'Espressif annoncé avec une latence < 10ms.

### Communication PC
Le récepteur utilise les bibliothèques `USB.h` et `USBHIDKeyboard.h` pour simuler un clavier standard auprès du système d'exploitation.
