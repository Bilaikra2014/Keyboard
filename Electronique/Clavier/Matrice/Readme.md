 # Partie electronique - Matrice de touche et encodeur
 
## Matrice de touche

Le clavier comporte 71 touches. Avec un système de surveillance normal des touches, il faudrait 71 GPIOs ce qui est beaucoup trop. Un système de matrice à donc été utilisé, permettant ainsi de réduire le nombre de broches utilisées. La matrice de touche se compose de 5 lignes et 15 colonnes, ce qui fait que l'on utiliser que 20 GPIOs. 

**Fonctionnement de la matrice** : 
- Le µC met successivement chaque ligne à l'état BAS.
- Il lit ensuite l'état des colonnes maintenues à l'état HAUT par des résistances de pull-up.
- Si une colonne est detectée à l'état BAS, c'est que la touche correspondant au croisement est activée.

**Le problème du Ghosting** : 

Sur une matrice simple, presser trois touches simultanément (ex: A, Q et S) peut créer un chemin de retour parasite. Le courant "remonte" par une ligne adjacente, faisant croire au contrôleur qu'une quatrième touche fantôme (Ghost) est pressée.
Pour pallier ce problème, chaque interrupteur est couplé à une diode de signal 1N4148 : 
- Rôle : Elle force le courant à circuler dans un seul sens (Ligne → Colonne).
- Résultat : Cela permet le N-Key Rollover (NKRO), garantissant que chaque pression de touche est enregistrée indépendamment, quel que soit le nombre de touches pressées simultanément.

Les touches utilisées sont des switchs mécanique **Cherry MX Red** commandés sur AliExpress.

<img width="1485" height="494" alt="image" src="https://github.com/user-attachments/assets/f18871fd-a640-4438-92dd-5cda5dff32b6" />

## Gestion de l'encodeur

L'encodeur permet la gestion du volume.

L'encodeur utilisé (déjà présent à l'école) possède une fonction RGB avec les 3 diodes (pin 1,2 et 4) mais ne seront pas utilisée ici.

<img width="785" height="335" alt="image" src="https://github.com/user-attachments/assets/04e54f50-811a-4e21-904e-551fb077ab4c" />


## Résultat final

Voici la vue 3D du Résultat final via logiciel : 

<img width="1273" height="477" alt="image" src="https://github.com/user-attachments/assets/1e305cb9-c0e5-4f89-b4f4-5638d6034bb3" />
