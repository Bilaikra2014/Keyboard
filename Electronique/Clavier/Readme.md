 # Partie electronique du clavier

## Gestion d'alimentation et Chargeur Li-Po

Lorsque la batterie est déchargée, le clavier sera branché à l'ordinateur via une prise USB. La prise de l'ordinateur va alimenter le clavier, et charger la batterie avec le courant restant. Une prise USB A d'ordinateur peut fournir en général jusqu'à 500 mA. 

Pour gérer l'alimentation et la charge, le composant utilisé sera le [MCP73871](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP73871-Data-Sheet-20002090E.pdf) qui permet : 
  - **Gestion du Power Path** : Alimente le système depuis l'USB tout en gérant la charge la batterie
  - **Partage du courant** : Donne la priorité à l'alimentation du système, le courant restant est alloué à la charge de la batterie

<center><img width="490" height="271" alt="image" src="https://github.com/user-attachments/assets/9d26c01b-241d-4708-a64f-6cdc560a6c05" /></center>

**Alimentation et Puissance**
  - In (18) : Alimentation du composant -> USB
  - Out (20) : Alimentation du système -> Régulateur LDO
  - Vbat (14) : Charge de la batterie -> Vbat
  - Vss (10) : Masse -> GND

**Contrôle et Logique**:
  - SEL (3) : Limite de courant à tirer sur l'USB -> LOW pour 500 mA (HIGH pour 1,8)
  - PROG2 (4) : Courant d'entré max à tirer -> HIGH pour 500 mA (HIGH pour 100mA)  
  - CE (17) : Activation du système à l'état haut -> Vusb
  - TE (9) : Sécurité interne du temps de charge -> GND

**Programmation et Sécurité**:
  - PROG1 (13) : Courant de charge rapide -> R = 1000V/Ireg, on veut Ireg = 250mA donc R = 4k
  - PROG3 (12) : Seuil de fin de charge -> R = 40k
  - VPCC (2)   : 
  - VBAT_S (16): Sonde de tension -> Vbat
  - THERM (5)  : Capteur de temperature ->  

**Indicateurs visuel**: Ces broches tirent vers la masse, 
  - PG (6)    : Indication que l'USB est branché
  - STAT1 (8) : Indication de la phase de charge 
  - STAT2 (7) : Indication de la fin de charge 

## Régulateur de tension

Le régulateur de tension utilisé est un [TLV75533PDBV](https://www.ti.com/lit/ds/symlink/tlv755p.pdf). Il a une très faible tension de dropout, 238mV à 500mA.

<center><img width="485" height="313" alt="image" src="https://github.com/user-attachments/assets/4cba229e-1c08-4b6f-a833-131fdbe47bce" /></center>

## Mesure de la batterie

<center><img width="666" height="443" alt="image" src="https://github.com/user-attachments/assets/8bd8ae78-2606-4d83-96ab-d49c5fb32c62" /></center>

La mesure de la batterie permet d'indiquer à l'utilisateur si le clavier à besoin d'être chargé ou pas. La tension étant supérieur aux 3v3 de l'esp32, il faut diviser cette tension pour qu'elle puisse être lue par l'esp32 à travers un pont diviseur de tension.

**Ajout des transistors Q3 et Q4**
Utiliser simplement un pont diviseur consommerait du courant en permance, ici 13µA. C'est pour cela que des transistors ont été ajouté. Le transistor Q3 est un mosfet canal P, qui lie la batterie au pont diviseur lorsqu'il est passant. Ce transistor est piloté par son complémentaire Q4, un canal N commandé par l'esp32. Ce transistor sera passant uniquement lors de la mesure. La résistance R9 sert de pull-up pour maintenir le transistor Q3 bloqué quand Q4 est bloqué.








