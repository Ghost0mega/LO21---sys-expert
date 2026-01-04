# Rapport de Projet — Système Expert (Modèle)

> À dupliquer et compléter pour votre rendu. Ne pas inclure de listing C dans le rapport.

## Métadonnées
- Auteur(s) : 
- Groupe : 
- Date : 
- Encadrant : 

## 1. Introduction
Ce rapport présente la conception, les algorithmes et la démarche d’implémentation d’un système expert élémentaire réalisé en langage C dans le cadre du module LO21. L’objectif est de modéliser une base de connaissances sous forme de règles, de manipuler une base de faits et d’exécuter un moteur d’inférence par chaînage avant afin de déduire de nouveaux faits certains.

Périmètre et contraintes:
- Le projet s’appuie sur des types abstraits de données étudiés en cours, en particulier une liste séquentielle utilisée pour représenter les prémisses des règles, l’ensemble des règles d’une base de connaissances et la base de faits.
- Les algorithmes doivent être fournis en pseudocode, avec leur profil complet (Données, Résultat, Lexique), et ne pas être confondus avec des listings de programme C.
- Le rapport ne contient aucun listing C; la démonstration et la qualité d’implémentation sont évaluées sur machine.

Résumé fonctionnel:
- Une règle est composée d’une prémisse (liste de propositions) et d’une conclusion (proposition).
- Le moteur d’inférence parcourt les règles et, lorsque toutes les propositions de la prémisse sont présentes dans la base de faits, ajoute la conclusion à la base de faits; l’algorithme itère jusqu’au point fixe.

Démarche et organisation:
- Définition des TAD `Regle` et `BC` en s’appuyant uniquement sur les opérations de la liste abstraite vues en cours (création, accès, test de vacuité, ajout en queue, retrait, appartenance récursive).
- Mise en place d’un moteur d’inférence modulaire utilisant les TAD précédents.
- Environnement outillé: projet C structuré, compilation par CMake et exécutable unique; jeux d’essais pour valider la correction et la terminaison.

Structure du document:
- Section 2: choix de conception et structures de données.
- Section 3: algorithmes des sous-programmes (Q1–Q3) avec profils et pseudocode.
- Section 4: jeux d’essais.
- Section 5: commentaires sur les résultats.
- Section 6: remarques qualité et modalités de démonstration.

## 2. Choix de Conception et Structures de Données
- Type abstrait « Liste » utilisé : opérations disponibles (nommage du cours) et invariants.
- Type `Proposition` : représentation choisie, identifiant, égalité.
- Type abstrait `Regle` : prémisse (liste de propositions) + conclusion (proposition).
- Type abstrait `BC` (Base de Connaissances) : liste de règles.
- Démarche d’implémentation : pourquoi ces structures, complexité attendue, compromis.

---

## 3. Algorithmes des Sous-Programmes (avec opérations abstraites)

Rappels exigés par le sujet :
- Un algorithme n’est pas un programme C.
- Chaque algorithme doit fournir son profil : Données, Résultat, Lexique.
- Utiliser les opérations du type abstrait « Liste » vues en cours (ex.
  `creerListe`, `ajouterEnQueue`, `tete`, `queue`, `estVide`, `retirer`, `contient`, …).
- L’algorithme d’appartenance dans une règle doit être récursif.

### 3.1. Type Abstrait `Regle`

#### 3.1.1. Créer une règle vide
- Signature (interface) : `Regle creerRegleVide()`
- Profil :
  - Données : aucune
  - Résultat : une règle avec prémisse vide et conclusion non définie
  - Lexique : `r` (règle), `premisse` (Liste<Proposition>), `conclusion` (Proposition | ⊥)
- Algorithme (opérations de Liste) :
```
// Algorithme : creerRegleVide
// Données : — ; Résultat : Regle r
r.premisse ← creerListe()
r.conclusion ← ⊥
renvoyer r
```
- Explications : initialise les composants selon les invariants.

#### 3.1.2. Ajouter une proposition à la prémisse (en queue)
- Signature : `void ajouterPremisseQueue(Regle r, Proposition p)`
- Profil :
  - Données : `r`, `p`
  - Résultat : `r.premisse` contient `p` en fin
  - Lexique : opérations de liste (`ajouterEnQueue`)
- Algorithme :
```
ajouterEnQueue(r.premisse, p)
```
- Explications : insertion FIFO au niveau de la prémisse.

#### 3.1.3. Créer la conclusion d’une règle
- Signature : `void definirConclusion(Regle r, Proposition c)`
- Profil : Données : `r`, `c` ; Résultat : `r.conclusion = c`
- Algorithme :
```
r.conclusion ← c
```

#### 3.1.4. Tester l’appartenance d’une proposition à la prémisse (récursif)
- Signature : `bool appartientPremisse(Regle r, Proposition p)`
- Profil : Données : `r`, `p` ; Résultat : vrai/faux ; Lexique : `tete`, `queue`, `estVide`
- Algorithme (récursif) :
```
// Algorithme : appartientPremisse
si estVide(r.premisse) alors
  renvoyer faux
sinon si tete(r.premisse) = p alors
  renvoyer vrai
sinon
  r2 ← r ; r2.premisse ← queue(r.premisse)
  renvoyer appartientPremisse(r2, p)
```
- Explications : parcours récursif de la liste.

#### 3.1.5. Supprimer une proposition donnée de la prémisse
- Signature : `void supprimerDePremisse(Regle r, Proposition p)`
- Profil : Données : `r`, `p` ; Résultat : `r.premisse` sans `p`
- Algorithme :
```
retirer(r.premisse, p)
```

#### 3.1.6. Tester si la prémisse est vide
- Signature : `bool premisseEstVide(Regle r)`
- Profil : Données : `r` ; Résultat : vrai/faux
- Algorithme :
```
renvoyer estVide(r.premisse)
```

#### 3.1.7. Accéder à la proposition en tête de la prémisse
- Signature : `Proposition tetePremisse(Regle r)`
- Profil : Données : `r` ; Résultat : proposition en tête ou ⊥ si vide
- Algorithme :
```
si estVide(r.premisse) alors renvoyer ⊥ sinon renvoyer tete(r.premisse)
```

#### 3.1.8. Accéder à la conclusion d’une règle
- Signature : `Proposition conclusion(Regle r)`
- Profil : Données : `r` ; Résultat : `r.conclusion`
- Algorithme :
```
renvoyer r.conclusion
```

---

### 3.2. Type Abstrait `BC` (Base de Connaissances)

#### 3.2.1. Créer une base vide
- Signature : `BC creerBCVide()`
- Profil : Données : — ; Résultat : `BC` avec liste de règles vide
- Algorithme :
```
bc.regles ← creerListe()
renvoyer bc
```

#### 3.2.2. Ajouter une règle à une base (en queue)
- Signature : `void ajouterRegleQueue(BC bc, Regle r)`
- Profil : Données : `bc`, `r` ; Résultat : `bc.regles` contient `r` en fin
- Algorithme :
```
ajouterEnQueue(bc.regles, r)
```

#### 3.2.3. Accéder à la règle en tête de la base
- Signature : `Regle regleTete(BC bc)`
- Profil : Données : `bc` ; Résultat : la règle en tête ou ⊥ si vide
- Algorithme :
```
si estVide(bc.regles) alors renvoyer ⊥ sinon renvoyer tete(bc.regles)
```

---

### 3.3. Moteur d’Inférence (avant-chainage, version cours)

- Signature : `Faits moteurInference(Faits BF, BC bc)`
- Profil :
  - Données : `BF` (Base de Faits : Liste<Proposition>), `bc` (Base de Connaissances)
  - Résultat : `BF'` (Faits déduits → BF enrichie)
  - Lexique : `changement` (bool), `r` (Regle), `premisse(r)`, `conclusion(r)`
- Algorithme (opérations de Liste) :
```
// Algorithme : moteurInference
changement ← vrai
alors que changement faire
  changement ← faux
  pour chaque r dans bc.regles faire
    // tester si toutes les propositions de la prémisse sont dans BF
    toutesVraies ← vrai
    L ← copie(r.premisse)
    tant que toutesVraies et non estVide(L) faire
      p ← tete(L)
      si non contient(BF, p) alors
        toutesVraies ← faux
      sinon
        L ← queue(L)
    fin tant que
    si toutesVraies alors
      c ← conclusion(r)
      si non contient(BF, c) alors
        ajouterEnQueue(BF, c)
        changement ← vrai
      fin si
    fin si
  fin pour
fin alors que
renvoyer BF
```
- Explications : boucle jusqu’à point fixe; utilisation des opérations de liste pour tester les prémisses et enrichir les faits.

---

## 4. Jeux d’Essais
Pour chaque jeu d’essai, préciser :
- Données de départ : `BF` initiale, `BC` (règles et leurs prémisses/conclusions).
- Procédure : séquence d’exécution (moteur, étapes observées).
- Résultat attendu : propositions ajoutées à `BF`.
- Critères de validation : cohérence, terminaison, absence de doublons.

Exemple de tableau à compléter :

| Jeu | BF initiale | Règles BC (résumé) | BF attendu |
|-----|-------------|--------------------|------------|
| 1   | {...}       | r1: {...}→X; r2: {...}→Y | {..., X, Y} |
| 2   | {...}       | r3: {...}→Z             | {..., Z}    |

---

## 5. Commentaires sur les Résultats
- Analyse : comportement observé, convergence, couverture des cas.
- Discussion : limites, ambiguïtés, évolutions possibles.
- Performance : taille des listes, nombre d’itérations.

---

## 6. Remarques, Qualité et Démonstration
- Rappels du sujet :
  - Un programme C et du pseudocode ne sont pas des algorithmes.
  - Chaque algorithme doit fournir son profil (données, résultat, lexique).
  - Un programme doit être commenté (utile).
  - La qualité d’implémentation est prise en compte.
  - Le rapport ne doit pas contenir le listing C.
- Checklist de qualité :
  - [ ] Toutes signatures et profils fournis pour Q1–Q3.
  - [ ] Algorithmes basés sur opérations de Liste du cours.
  - [ ] Test d’appartenance (prémisse) récursif correctement détaillé.
  - [ ] Jeux d’essais et résultats argumentés.
  - [ ] Aucun listing C dans le rapport.
- Démonstration (sur ordinateur devant l’enseignant) :
  - Compilation (exemple) :
    ```bash
    cmake -S . -B build
    cmake --build build
    ./build/sys_expert
    ```
  - Fichiers sources de référence (voir dépôt, pas à inclure dans le rapport) :
    - src/regle.h, src/regle.c
    - src/bc.h, src/bc.c
    - src/inference.h, src/inference.c

---

## 7. Annexes (optionnel)
- Invariants de structure.
- Schémas (diagrammes) des données et flux.
- Liens vers ressources extérieures.
