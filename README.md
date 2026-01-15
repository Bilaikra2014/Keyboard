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
* ** Prise USB C pour le mode filaire.
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

Datasheet ESP32 C3 : https://documentation.espressif.com/esp32-c3_datasheet_en.pdf

Datasheet ESP32 S3 : https://documentation.espressif.com/esp32-s3_datasheet_en.pdf

## 📂 Structure du projet

```text
.
├── 📁 Electronics         # Schémas et typons (KiCad/Altium)
├── 📁 Firmware            # Code source Arduino/ESP-IDF
│   ├── 📁 Keyboard_S3     # Code de l'émetteur
│   └── 📁 Dongle_C3       # Code du récepteur (HID)
├── 📁 Mechanical          # Modèles 3D (STL/STEP)
└── README.md

