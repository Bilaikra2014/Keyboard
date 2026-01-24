# Partie electronique du clavier

### Gestion d'alimentation et Chargeur Li-Po

Lorsque la batterie est déchargée, le clavier sera branché à l'ordinateur via une prise USB. La prise de l'ordinateur va alimenter le clavier, et charger la batterie avec le courant restant. Une prise USB A d'ordinateur peut fournir en général jusqu'à 500 mA. 

Pour gérer l'alimentation et la charge, le composant utilisé sera le MCP73871-2CC qui permet : 
  - **Gestion du Power Path** : Alimente le système depuis l'USB tout en gérant la charge la batterie
  - **Partage du courant** : Donne la priorité à l'alimentation du système, le courant restant est alloué à la charge de la batterie

#### Charge de la batterie
#### Régulation et commutation

<img width="539" height="296" alt="image" src="https://github.com/user-attachments/assets/37fb33a5-5239-41c6-a92a-1e32511fd0b8" />

Comme le clavier peut fonctionner avec ou sans fil, il doit être capable de choisir entre les 2 tensions. La tension prioritaire sera l'USB qui alimentera le clavier et rechargera la batterie. Peu importe la tension, elle doit être régulée avant d'alimenté l'ESP32, à l'aide d'un LDO.

Pour choisir le LDO, en plus de regarder sa tension de sortie, il faut regarder la tension d'entrée minimale, ainsi 


Quand l'USB n'est pas branché, la résistance R10 force le transistor à être passant, la tension qui rentre dans le LDO est donc celle de la batterie. Quand l'USB est branché, le transistor est bloqué et la schottky passante, c'est donc Vusb qui rentre dans le LDO.

#### Mesure de la batterie
