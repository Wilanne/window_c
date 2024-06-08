#### Autheur : Léo AUBRY

# Nom du Projet

```plaintext
   ▄███████▄    ▄████████  ▄██████▄      ███      ▄██████▄   ▄████████  ▄██████▄   ▄█        ▄█       ███    █▄    ▄▄▄▄███▄▄▄▄  
  ███    ███   ███    ███ ███    ███ ▀█████████▄ ███    ███ ███    ███ ███    ███ ███       ███       ███    ███ ▄██▀▀▀███▀▀▀██▄
  ███    ███   ███    ███ ███    ███    ▀███▀▀██ ███    ███ ███    █▀  ███    ███ ███       ███       ███    ███ ███   ███   ███
  ███    ███  ▄███▄▄▄▄██▀ ███    ███     ███   ▀ ███    ███ ███        ███    ███ ███       ███       ███    ███ ███   ███   ███
▀█████████▀  ▀▀███▀▀▀▀▀   ███    ███     ███     ███    ███ ███        ███    ███ ███       ███       ███    ███ ███   ███   ███
  ███        ▀███████████ ███    ███     ███     ███    ███ ███    █▄  ███    ███ ███       ███       ███    ███ ███   ███   ███
  ███          ███    ███ ███    ███     ███     ███    ███ ███    ███ ███    ███ ███▌    ▄ ███▌    ▄ ███    ███ ███   ███   ███
 ▄████▀        ███    ███  ▀██████▀     ▄████▀    ▀██████▀  ████████▀   ▀██████▀  █████▄▄██ █████▄▄██ ████████▀   ▀█   ███   █▀ 
               ███    ███                                                         ▀         ▀                                   
```

## Description

Il est la première version du protocole de communication qui est utilisé dans le projet PATO lors de l'incrément premier, ce projet est écrit en C. 

Le but est d'établir une commnunication entre SYSTEMO et VISIO via les sockets UNIX sur un environemment de développement UNIX. Ce projet sera compilé sous x86_64 dans l'incrément premier et sera sur cible Raspberry Pi aarch_64 dans l'incrément 2. 

#### Ce protocole est développé par : 

* Léo AUBRY [(Kronos)](https://github.com/Wilanne)  
* Baptiste JOUBERT-LETHIN [(Bwapt)](https://github.com/Bwapt)  
* Theo MALINGE

## Fonctionnalités de protocol.h

* Messages sous forme de structure
* Sérialisation
* Déserialisation

## Fonctionnalités de sockets.h

* Création de socket
* Bind
* Listen
* Accept
* Connect
* Envoi / Réception de données

## Fonctionnalités de communication.h (wrapper de sockets.h et protocol.h)

* Création d'une socket serveur complète
* Création d'une socket client complète
* Démarrage / arrêt d'un thread de réception de données
* Envoi d'un message complet 

## Contributions

Les contributions sont les bienvenues.