# 🤖 Protorob – Robot Thermomètre Asservi

Projet réalisé dans le cadre de l’UE **Prototypage des systèmes robotisés** à **IMT Atlantique**.

L’objectif du projet est de concevoir un **robot thermomètre** capable de mesurer une température et de l’afficher physiquement à l’aide d’une **aiguille asservie en position**, en mettant en œuvre un **régulateur PID discret implémenté par timer et interruption**.

---

## 🎯 Objectifs techniques

- Lire une mesure de température via **I2C**
- Implémenter un **régulateur PID discret**
- Maîtriser le **pas d’échantillonnage** via un **timer matériel**
- Utiliser les **interruptions** Arduino
- Concevoir une **structure mécanique complète**
- Tester, régler et valider le comportement du système

---

## 💻 Logiciel & outils

- **Arduino IDE**
- **Fusion 360** (CAO mécanique)
- Langage **C / C++ embarqué**
- Communication **I2C**
- Timer + interruptions matérielles

---

## 🧮 Régulation

- Régulateur **PID monovariable**
- Implémentation **discrète**
- Gestion du pas d’échantillonnage par **timer**
- Anti-emballement numérique
- Réglage expérimental des gains

Le régulateur agit sur la commande moteur afin d’assurer :
- Précision
- Stabilité
- Temps de réponse maîtrisé

---

## 🧱 Conception mécanique

- CAO réalisée **entièrement sous Fusion 360**
- Support moteur
- Aiguille d’affichage
- Cadran de température
- Conception pensée pour l’assemblage et les tests

---

## 🧪 Tests & validation

- Tests unitaires des capteurs
- Vérification du timing d’échantillonnage
- Réglage progressif du PID
- Validation de l’affichage de la température
- Tests de stabilité et de répétabilité

---

## 👥 Organisation du projet

- Projet réalisé en **groupe de 4**
- Travail en autonomie sur des **créneaux dédiés**
- Itérations rapides entre :
  - Conception
  - Implémentation
  - Tests

