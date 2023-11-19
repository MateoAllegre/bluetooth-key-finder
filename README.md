# Key Finder en Bluetooth Low Energy sur une carte de développement NRF52, avec Zephyr

## Overview

Le but de ce projet est d'implémenter les fonctionnalités principales nécessaires pour le produit KeyFinder d'Action sur une carte NRF52. En particulier, la carte doit diffuser certains services et caractéristiques particuliers listés dans le document Tableau_services_carac.xlsx. Certaines caractéristiques doivent aussi être liées à des boutons et LEDs présents sur la carte.

## Démarrage du projet

Le projet peut être build via VSCode avec les extensions NRFConnect. Il doit être build avec le fichier de configuration prj.conf. Le projet a été testé sous une carte de développement NRF52 et il n'est pas garanti qu'il fonctionne normalement sous d'autres cartes permettant le Bluetooth Low Energy.

## Utilisation du projet

Une fois connecté à la carte (nommée "Key Finder 42") par l'intermédiaire de l'application NRFconnect par exemple, voici ce qu'il est possible de faire : 

- Dans le service d'UUID 0x1401 (System), il est possible de lire la version (caractéristique 0x1404) ainsi que le message de débug (caractéristique 0x1408). Ce dernier étant en Notify, il est possible de s'y abonner, cependant, la valeur de ce message n'est jamais modifiée et reste toujours à "No errors"
- Dans le service d'UUID 0x1402 (Input), il est possible de s'abonner à la caractéristique Bouton (0x1405). Lors d'une pression sur le Button1, la valeur envoyée par cette caractéristique passe à 1.
- Dans le service d'UUID 0x1403 (Output), il est possible d'écrire dans les caractéristiques LED (0x1407) et Buzzer (0x1406). Les valeurs acceptées et le comportement résultant sont comme suit : 
    - La caractéristique LED attend un booléen sur un octet. Si la valeur reçue est 0x01, la LED2 s'allume. Si la valeur reçue est 0x00, la LED2 s'éteint. Toute autre valeur n'est pas reconnue comme étant une valeur valide et ne modifie pas l'état de la LED.
    - La caractéristique Buzzer attend un entier sur 4 octets. La valeur reçue contrôle la vitesse du clignotement de la LED1 via le PWM (Pulse Width Modulation).

## Problèmes connus

Le PWM devrait pouvoir changer l'opacité de la LED1 lentement en fonction des valeurs reçues, cependant, la variation de vitesse du clignotement est beaucoup trop forte. Si les valeurs reçues sont petites (entre 0x00000001 et 0x00000005 environ), on peut observer une grande différence de vitesse de clignotement entre deux valeurs consécutives (après 0x00000003, le clignotement est discernable à l'oeil nu). Des valeurs trop grandes n'allument pas la LED.

Je pense que le problème est dû à une mauvaise utilisation de la période et de la pulse width. Actuellement, le code met la période à la valeur reçue et la pulsation à la moitié de la période, alors que la période devrait être fixée et la pulsation changée (avec éventuellement un changement de période si la pulsation change trop).

## Origine du code

Une grande partie de ce projet est basée sur des samples et fichiers de code de Zephyr.
L'arborescence globale du projet est basée sur le sample "peripheral_hr".
La définition des services et caractéristiques est inspirée des fichiers bas.c, contenant la définition du service standard pour la batterie (Battery Service), et hrs.c, contenant la définition du service standard pour un rythme cardiaque (Heart Rate Service).
Le fonctionnement du bouton (présent dans le main.c) et de la LED (présent dans output.c) sont basés sur le sample "button".
Le fonctionnement du PWM pour contrôler la LED1 est basé sur le sample "blinky_pwm".