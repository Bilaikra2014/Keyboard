# ⌨️ WirelessKeyboard Project

Ce projet a pour objectif la conception et la fabrication d'un clavier mécanique **bi-mode** (Filaire USB-C / Sans-fil 2.4GHz) haute performance. Le système repose sur un écosystème ESP32 permettant une communication rapide et personnalisée.

---

## 📋 Présentation du projet

Le projet consiste à créer deux dispositifs distincts :
1.  **Le Clavier (Émetteur) :** Gère la matrice de touches, la batterie et la communication.
2.  **Le Récepteur (Dongle) :** Reçoit les données sans fil et les transmet à l'ordinateur via USB.

L'architecture se divise en trois axes de développement :
* **Électronique :** Conception des schémas et des PCB.
* **Informatique :** Développement des firmwares d'émission et de réception.
* **Mécanique :** Conception CAO 3D du boîtier et du dongle.

---

## 🛠️ Spécifications Matérielles

### 1. Le Clavier (ESP32-S3)
Le choix de l'**ESP32-S3** est motivé par son support **USB natif**, indispensable pour le mode filaire.

* **Alimentation :** Batterie LiPo 1S (3.7V).
* **Régulation :** Passage du 3.7V/5V vers 3.3V via un régulateur de tension (LDO).
* **Gestion de charge :** Circuit de charge intégré permettant :
    * L'alimentation du µC via USB lors du branchement.
    * La recharge simultanée de la batterie.
    * Le basculement automatique sur batterie lors du débranchement.

### 2. Le Récepteur (ESP32-C3)
L'**ESP32-C3** est utilisé pour son format compact et son coût réduit. Il fait office de pont entre le clavier et le PC.

* **Interface :** Prise USB-A mâle.
* **Alimentation :** LDO convertissant le 5V USB en 3.3V.
* **Contrôles physiques :**
    * **Bouton BOOT :** Maintien du GPIO 9 à la masse pour le mode téléchargement.
    * **Bouton RESET :** Redémarrage du contrôleur pour les phases de test.
* **Câblage USB natif :**
    * `D+` ↔ GPIO 19
    * `D-` ↔ GPIO 18
    * `V+` ↔ Entrée LDO
    * `GND` ↔ Masse commune

---

## 📡 Protocoles de Communication

### Communication Sans-fil (ESP-NOW)
Pour minimiser la latence, le projet utilise **ESP-NOW**, un protocole de connexion sans fil à basse consommation d'Espressif.
* Communication directe par adresse MAC (pas de handshake Wi-Fi lourd).
* Optimisé pour l'envoi de paquets courts (frappes de touches).

[Image of ESP-NOW communication protocol architecture]

### Communication PC (USB HID)
Le récepteur utilise les bibliothèques `USB.h` et `USBHIDKeyboard.h` pour simuler un clavier standard auprès du système d'exploitation.
* Reconnaissance **Plug & Play**.
* Aucun pilote spécifique requis sur l'ordinateur cible.

---

## 📂 Structure du projet

```text
.
├── 📁 Electronics         # Schémas et typons (KiCad/Altium)
├── 📁 Firmware            # Code source Arduino/ESP-IDF
│   ├── 📁 Keyboard_S3     # Code de l'émetteur
│   └── 📁 Dongle_C3       # Code du récepteur (HID)
├── 📁 Mechanical          # Modèles 3D (STL/STEP)
└── README.md

## Clavier
## Clavier

### Partie électronique 
### Partie informatique
### Partie mécanique


#### Partie clavier

Le clavier pouvant être filaire ou sans fil, il devra disposer d'une batterie pour fonctionner de manière sans fil, mais aussi d'un système de recharge lorsque le clavier sera branché en USB. Lorsque qu'il sera branché, le µC sera alimenté via l'USB, et la batterie se chargera en même temps.
Le microcontrôleur utilisé pour ce projet sera un ESP32 S3, car il dispose de l'USB natif. Ce µC fonctionne en 3v3. Une batterie lipo 1s 3v7 sera utilisée avec un régulateur de tension.
Il va falloir étudier un schéma pour pouvoir charger le clavier.

#### Partie Recepteur

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
