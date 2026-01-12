# WirelessKeyboard

## Description du projet 

Ce projeta a pour objectif de créer un clavier qui pourra fonctionner de manière sans fil (avec un recepteur à brancher sur l'ordinateur), ou de manière filaire via un cable USB A - USB C. Il faudra donc créer 2 cartes : le recepteur pour la partie sans fil, ainsi que le clavier.

#### Partie clavier

Le clavier pouvant être filaire ou sans fil, il devra disposer d'une batterie pour fonctionner de manière sans fil, mais aussi d'un système de recharge lorsque le clavier sera branché en USB. Lorsque qu'il sera branché, le µC sera alimenté via l'USB, et la batterie se chargera en même temps.
Le microcontrôleur utilisé pour ce projet sera un ESP32 S3, car il dispose de l'USB natif. Ce µC fonctionne en 3v3. Une batterie lipo 1s 3v7 sera utilisée avec un régulateur de tension.
Il va falloir étudier un schéma pour pouvoir charger le clavier.

#### Recepteur

Le recepteur sera connecté à l'ordinateur, il recevra les données envoyées par l'emetteur. Il faudra chosir l'ESP32 à utiliser, pour l'instant un ESP32 C3 car plus compact/economique.
Il faudra réaliser une carte avec la puce de l'ESP32 associé à une prise USB-A Mâle, avec en plus un LDO pour convertir le 5v de l'USB en 3v3 pour alimenter le microcontrôleur. Il faudra aussi 2 boutons : 
- BOOT pour que l'ESP32 rentre en mode de téléchargement (le GPIO sera maintenu à la masse) pendant le chargement
- RESET pour les tests.
Au niveau de la prise USB-A :
- D+ à la broche 19
- D- à la broche 18
- V+ à l'entrée du LDO
- Masse commune à l'ESP32 et le LDO

### Communication Emetteur-Recepteur

Sachant que les 2 dispositifs se basent sur l'utilisation d'ESP32, le moyen de communication envisagé pour l'instant est le protocole ESP-NOW.

### Communication Recepteur-Ordinateur
Lorsque l'on branche un ESP32-C3 à un ordinateur, on peut le faire passer pour n'improte quel périphérique USB. Le type de périphérique (clavier) sera définit dans le code du dongle. Il faudra utiliser les bibliothèques "USB.h" et "USBHIDKeyboard.h"



#####   Schéma 


### Liens utiles

Datasheet ESP32 S3 : https://documentation.espressif.com/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf  
Pages importantes : Pages 10 et 11 -> Brochage
