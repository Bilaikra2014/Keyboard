# ⌨️ WirelessKeyboard Project

Ce projet a pour objectif la conception et la fabrication d'un clavier mécanique (Filaire USB-C / Sans-fil). Le système repose sur un écosystème ESP32 permettant une communication rapide et personnalisée. Ce projet est une première version de test, une autre version pourrait être conçue dans le but de rechercher encore plus de perfomance en utilisant des composants plus adaptés.

## 📋 Présentation du projet

Le projet consiste à créer deux dispositifs distincts :
1.  **Le Clavier (Émetteur) :** Gère la matrice de touches, la batterie et la communication.
2.  **Le Récepteur (Dongle) :** Reçoit les données sans fil et les transmet à l'ordinateur via USB.

L'architecture se divise en trois axes de développement :
* **Électronique :** Conception des schémas et des PCB.
* **Informatique :** Développement des firmwares d'émission et de réception.
* **Mécanique :** Conception CAO 3D du boîtier du clavier et de celui dongle.

---

## 🛠️ Spécifications Matérielles

### 1. Le Clavier (ESP32-S3)
Le choix de l'**ESP32-S3** est motivé par son support **USB natif**, indispensable pour le mode filaire.
* **Prise USB C** pour le mode filaire.
* **Alimentation :** Batterie LiPo 1S (3.7V).
* **Régulation :** Passage du 5V(USB)/3v7(Batterie) vers 3.3V via un régulateur de tension (LDO).
* **Gestion de charge :** Circuit de charge intégré permettant :
    * L'alimentation du µC via USB lors du branchement.
    * La recharge simultanée de la batterie.
    * Le basculement automatique sur batterie lors du débranchement.

### 2. Le Récepteur (ESP32-S3-MINI)

<img width="192" height="217" alt="image" src="https://github.com/user-attachments/assets/e0a32e45-124e-4145-9cae-09294856f0fe" />      <img width="195" height="218" alt="image" src="https://github.com/user-attachments/assets/e0976d60-9b48-4f87-9636-3a40db272575" />


L'**ESP32-S3-MINI** est utilisé pour son format compact et son coût réduit. Il fait office de pont entre le clavier et le PC.
* **Prise USB A** pour le brancher à l'ordinateur.
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
Pour minimiser la latence, le projet utilise **ESP-NOW**, un protocole de connexion sans fil à basse consommation d'Espressif annoncé avec une latence < 10ms.

### Communication PC (USB HID)
Le récepteur utilise les bibliothèques `USB.h` et `USBHIDKeyboard.h` pour simuler un clavier standard auprès du système d'exploitation.

## 📂 Structure du projet

Chaque sous-structure du projet aura son propre fichier Readme dédiée à la partie qu'elle traite.

```text
.
├── 📁 Electronique        # Schémas et PCB (KiCad)
│   ├── 📁 Lib                # Librairie Kicad
│   ├── 📁 Clavier            # Code de l'émetteur
│   └── 📁 Recepteur          # Code du récepteur
├── 📁 Informatique        # Code source ESP32
│   ├── 📁 Clavier            # Code de l'émetteur
│   └── 📁 Recepteur          # Code du récepteur
├── 📁 Mecanique          # Modèles 3D (STL/STEP)
└── README.md               #README global du projet

