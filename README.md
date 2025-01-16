# Battle-C bot 

## Installation 

### Pré-requis:
- Docker 

### Compilation 
```
docker build -t battle_bot -f Dockerfile .
```

### Lancement 
``` 
docker run battle_bot
```

### Partie Git

- La branche master se sert plus, utiliser la branche main pour un soucis de pratique
- Récupérer le projet en local -> git clone https://github.com/LucasBroda/BattleBotEnC
- Commande pour créer une branche -> git checkout -b "nom_de_votre_branche" (vous permez de créer une branche et de vous mettre dessus directement)
- Créer une branche pour une feature, comme ça on garde une structure propre et on se perd pas
- Faire des commits atomiques idéalement (une fonctionnalité = un commit), pour ça, l'extension Git sur VSCode est pratique, vous pouvez choisir les fichiers à commit etc, c'est plus rapide que taper les commandes à la main h24
- Si vous avez pas l'extension :
      - Pour commit un changement -> git commit -m "Votre description", ne jamais faire de git add . (ajoute tout), c'est vraiment pas propre et après pour se retrouver dans ce qu'on fait ça sera la galère
      - Pour mettre à jour votre branche par rapport à une autre -> git pull origin "nom_de_la_branche"
      - Pour pousser votre code (push) -> git push

- Ne jamais push directement sur le main, ici on aura pas trop de soucis vu que c'est un petit projet mais voilà on sait jamais, push sur votre branche, et faite ensuite une PR sur la branche dev
Si la branche dev est stable une fois les changements appliqués, alors, on fera une PR sur le main
- Pour les conflits, l'extension de base VSCode permet de les gérer rapidement, c'est assez intuitif mais au cas où, demander moi 
