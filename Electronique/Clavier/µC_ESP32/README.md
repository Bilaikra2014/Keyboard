 # Partie electronique du clavier
## Gestion d'alimentation et Chargeur Li-Po

Lorsque la batterie est déchargée, le clavier sera branché à l'ordinateur via une prise USB. La prise de l'ordinateur va alimenter le clavier, et charger la batterie avec le courant restant. Une prise USB A d'ordinateur peut fournir en général jusqu'à 500 mA. 

Pour gérer l'alimentation et la charge, le composant utilisé sera le [MCP73871](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP73871-Data-Sheet-20002090E.pdf) qui permet : 
  - **Gestion du Power Path** : Alimente le système depuis l'USB tout en gérant la charge la batterie
  - **Partage du courant** : Donne la priorité à l'alimentation du système, le courant restant est alloué à la charge de la batterie

<div align="center"><img width="757" height="426" alt="image" src="https://github.com/user-attachments/assets/2803e40a-cd94-4d11-8320-4aa35526f54a" />
</div>

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

**Indicateurs visuel**: Ces broches tirent vers la masse, on y connecte des leds
  - PG (6)    : Indication que l'USB est branché, à brancher à l'ESP32 pour qu'il detecte si on est sur batterie ou USB
  - STAT1 (8) : Indication de la phase de charge 
  - STAT2 (7) : Indication de la fin de charge 

## Régulateur de tension

Le µC sera alimenté par du 5v ou bien du 3v7, or il n'accepte que du 3v3. Il faut donc utiliser un régulateur de tension. Le régulateur de tension utilisé est un [BU33SD5WGTR](https://www.rohm.com/products/power-management/linear-regulators/single-output-ldo-regulators/bu33sd5wg-product).

<img width="673" height="385" alt="image" src="https://github.com/user-attachments/assets/ad128a30-9869-45a3-b217-4c7aa443c772" />


## Mesure de la batterie

La mesure de la batterie permet d'indiquer à l'utilisateur si le clavier à besoin d'être chargé ou pas. La tension étant supérieur aux 3v3 de l'esp32, il faut diviser cette tension pour qu'elle puisse être lue par l'esp32 à travers un pont diviseur de tension.

<img width="775" height="513" alt="image" src="https://github.com/user-attachments/assets/aaa39b78-a281-44c3-a80c-9545639daac7" />



**Pont Diviseur de tension**:

Le pont diviseur de tension formé par R16 et R17 permet d'ajuster la tension mesurée par L'ESP32. La tension de la batterie varie entre 4v2 et 3v7, ce qui donnera pour l'ESP32 une tension mesurée entre 3v et 2,64v, ce qui est dans la plage de donnée mesurable.

**Ajout des transistors Q3 et Q4**:

Utiliser simplement un pont diviseur consommerait du courant en permanance, aussi minime soit-il. C'est pour cela que des transistors ont été ajouté. Le transistor Q2 est un mosfet canal P, qui lie la batterie au pont diviseur lorsqu'il est passant. Ce transistor est piloté par son complémentaire Q1, un canal N commandé par l'esp32. Ce transistor sera passant uniquement lors de la mesure déclenchée par l'ESP32. La résistance R15 sert de pull-up pour maintenir le transistor Q2 bloqué quand Q1 est bloqué. Ce montage permet de relier le pont diviseur à la batterie uniquement lors de la mesure, limitant ainsi la consommation du pont diviseur.


## USB-C et protection ESD

Le port USB-C permet l'utilisation filaire du clavier, ainsi que la recharge de la batterie.

<img width="457" height="396" alt="image" src="https://github.com/user-attachments/assets/1799843e-ff6c-4a52-ae34-a319efdd94e6" />


Les broches D+ et D- permettent la communication ESP32-Ordinateur. 
Les broches CC1 et CC2 doivent être liées à des résistances de 5k1 de pull-down pour que la source détecte la présence du clavier et délivre ainsi le 5v.









