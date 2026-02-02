# Protorob – Robot thermomètre à affichage analogique 🤖
> [!NOTE]
> Projet réalisé dans le cadre de l’UE *Prototypage des systèmes robotisés* visant la conception et la réalisation complète d’un robot thermomètre asservi en position.

![alt text](https://raw.githubusercontent.com/Emilien-Wolff/Protorob/refs/heads/main/Photos/Thermorob_led_V2.jpg)


## Objectifs

> Mesurer une température à l’aide d’un capteur dédié et l’afficher physiquement via une aiguille entraînée par un moteur

> Mettre en œuvre un asservissement en position précis et stable par l’implémentation d’un régulateur PID discret sur microcontrôleur


## Points abordés

- Gestion de projet sur un temps court (3 semaines) avec planification des tâches et travail en autonomie sur des créneaux dédiés  
- Conception d’un système volontairement simple dans son principe, mais nécessitant une implémentation rigoureuse et propre  
- Lecture de la température via un capteur **MPL3115A2** et communication **I2C**  
- Implémentation d’un **régulateur PID monovariable** avec gestion du pas d’échantillonnage  
- Codage du régulateur à l’aide d’un **timer matériel** et de **routines d’interruption** sous Arduino  
- Conception mécanique complète du robot thermomètre, de l’aiguille et du support moteur  
- Réalisation de la **CAO 3D intégrale sous Fusion 360**  
- Tests, réglages expérimentaux et validation du comportement du système  
- Communication et coordination au sein d’un groupe de 4 personnes

## Compétences mobilisées

- Programmation embarquée (Arduino, C/C++)
- Automatique : régulation PID discrète
- Gestion du temps réel (timer, interruptions)
- Prototypage mécanique et CAO 3D
- Tests, réglages et validation expérimentale
- Travail en équipe et gestion de projet court

*📄 Le rapport complet du projet, détaillant la conception, l’implémentation et les choix techniques, est disponible à la racine du dépôt (`main`).*

## FAQ

#### Quel est l'intérêt de ce robot ?
Evidemment, ce type de mesure de température n'est pas voire pas du tout le plus répandu, mais il s'avère que le projet était l'occasion à tout le monde de découvrir le prototypage à travers 3 fondamentaux : l'électronique et la programmation, la conception mécanique et le contrôle commande. Nous nous sommes appropriés le sujet en faisant notre propre version du robot.

#### Combien de temps ça vous a pris ?

Le projet s’est déroulé sur **3 semaines**, avec des créneaux dédiés, depuis la phase de conception jusqu’aux tests et à la soutenance finale.

## 🚀 Qui sommes-nous ?
Nous sommes actuellement étudiants en école d’ingénieur à **IMT Atlantique**, et ce projet s’inscrit dans notre formation en **automatique et systèmes cyber-physiques**. Merci à Loïc Fournier, Grégory Boursin et Cédric Léger pour le travail.
