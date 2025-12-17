# LO21---sys-expert
Projet de Lo21 en A25

## Description
Mini système expert booléen (propositions vraies/fausses) avec:
- **Base de connaissances (BC)**: liste de règles `prémisse => conclusion`.
- **Base de faits (BF)**: propositions vraies connues.
- **Moteur d'inférence**: chaînage avant, déduit de nouveaux faits.

L'exemple fourni modélise un diagnostic auto simplifié.

## Construire et exécuter
```bash
cmake -S . -B build
cmake --build build -j
./build/sys_expert                 # mode TUI (si ncurses installé)
./build/sys_expert -t              # mode texte uniquement
./build/sys_expert --text-only     # idem, texte uniquement
```

En mode texte, le programme imprime le graphe ASCII de la base d'exemple.
Si ncurses n'est pas installé et que vous lancez sans `-t/--text-only`, une erreur explicite est affichée.

## Structure du code
- `src/proposition.h`: type `Proposition` (nom + négation `¬`).
- `src/list_proposition.{h,c}`: liste chaînée de `Proposition`.
- `src/regle.{h,c}`: type abstrait `Regle` et ses opérations (création, ajout prémisse en queue, conclusion, appartenance récursive, suppression, accès tête, etc.).
- `src/list_regle.{h,c}`: liste de `Regle`.
- `src/bc.{h,c}`: type abstrait `BC` (base de connaissances), opérations (créer vide, ajouter règle en queue, accéder tête).
- `src/inference.{h,c}`: `BaseFaits` et moteur d'inférence par chaînage avant.
- `src/main.c`: construit l'exemple du sujet et affiche les faits avant/après inférence.

## Ajouter des propositions/règles
Voir `src/main.c` pour un exemple. Les propositions s'écrivent par nom et un indicateur de négation (1 pour `¬`, 0 pour positif).

## Remarques
- Les opérations utilisent un TAD liste simple conforme au cours.
- Mémoire libérée dans `bc_free` et `facts_free`.
