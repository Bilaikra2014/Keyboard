# Partie electronique du clavier

### Gestion d'alimentation


#### Régulation et commutation

<img width="539" height="296" alt="image" src="https://github.com/user-attachments/assets/37fb33a5-5239-41c6-a92a-1e32511fd0b8" />

Comme le clavier peut fonctionner avec ou sans fil, il doit être capable de choisir entre les 2 tensions. La tension prioritaire sera l'USB qui alimentera le clavier et rechargera la batterie. Peu importe la tension, elle doit être régulée avant d'alimenté l'ESP32, à l'aide d'un LDO.

Pour choisir le LDO, en plus de regarder sa tension de sortie, il faut regarder la tension d'entrée minimale, ainsi 


Quand l'USB n'est pas branché, la résistance R10 force le transistor à être passant, la tension qui rentre dans le LDO est donc celle de la batterie. Quand l'USB est branché, le transistor est bloqué et la schottky passante, c'est donc Vusb qui rentre dans le LDO.

#### Mesure de la batterie
#### Charge de la batterie
