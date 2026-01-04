# Algorithmes — Projet Système Expert (Style cours)

> Pseudocode formel (Lexique / Donnee / Resultat / Algo). Utilise les opérations du type abstrait Liste vues en cours: `creerListe`, `estVide`, `tete`, `queue`, `ajouterEnQueue`, `retirer`, `contient`, `copie`.

## Question 1 — Type Abstrait `Regle`

### 1.1 Créer une règle vide
Lexique:
    Resultat : r : Regle
    Donnee : —
Algo:
    Fonction CREER_REGLE_VIDE() : Regle
    Debut
        r.premisse <- creerListe()
        r.conclusion <- ⊥
        CREER_REGLE_VIDE <- r
    Fin

### 1.2 Ajouter une proposition à la prémisse (en queue)
Lexique:
    Resultat : —
    Donnee : r : Regle, p : Proposition
Algo:
    Procedure AJOUTER_PREMISSE_QUEUE(r : Regle, p : Proposition)
    Debut
        ajouterEnQueue(r.premisse, p)
    Fin

### 1.3 Définir la conclusion d’une règle
Lexique:
    Resultat : —
    Donnee : r : Regle, c : Proposition
Algo:
    Procedure DEFINIR_CONCLUSION(r : Regle, c : Proposition)
    Debut
        r.conclusion <- c
    Fin

### 1.4 Tester l’appartenance d’une proposition à la prémisse (récursif)
Lexique:
    Resultat : b : Booleen
    Donnee : r : Regle, p : Proposition
Algo:
    Fonction APPARTIENT_PREMISSE(r : Regle, p : Proposition) : Booleen
    Debut
        Si estVide(r.premisse) Alors
            APPARTIENT_PREMISSE <- Faux
        Sinon Si tete(r.premisse) = p Alors
            APPARTIENT_PREMISSE <- Vrai
        Sinon
            r2 <- r
            r2.premisse <- queue(r.premisse)
            APPARTIENT_PREMISSE <- APPARTIENT_PREMISSE(r2, p)
        FSi
    Fin

### 1.5 Supprimer une proposition donnée de la prémisse
Lexique:
    Resultat : —
    Donnee : r : Regle, p : Proposition
Algo:
    Procedure SUPPRIMER_DE_PREMISSE(r : Regle, p : Proposition)
    Debut
        retirer(r.premisse, p)
    Fin

### 1.6 Tester si la prémisse est vide
Lexique:
    Resultat : b : Booleen
    Donnee : r : Regle
Algo:
    Fonction PREMISSE_EST_VIDE(r : Regle) : Booleen
    Debut
        PREMISSE_EST_VIDE <- estVide(r.premisse)
    Fin

### 1.7 Accéder à la proposition en tête de la prémisse
Lexique:
    Resultat : q : Proposition | ⊥
    Donnee : r : Regle
Algo:
    Fonction TETE_PREMISSE(r : Regle) : Proposition
    Debut
        Si estVide(r.premisse) Alors
            TETE_PREMISSE <- ⊥
        Sinon
            TETE_PREMISSE <- tete(r.premisse)
        FSi
    Fin

### 1.8 Accéder à la conclusion d’une règle
Lexique:
    Resultat : c : Proposition | ⊥
    Donnee : r : Regle
Algo:
    Fonction CONCLUSION(r : Regle) : Proposition
    Debut
        CONCLUSION <- r.conclusion
    Fin

---

## Question 2 — Type Abstrait `BC` (Base de Connaissances)

### 2.1 Créer une base vide
Lexique:
    Resultat : bc : BC
    Donnee : —
Algo:
    Fonction CREER_BC_VIDE() : BC
    Debut
        bc.regles <- creerListe()
        CREER_BC_VIDE <- bc
    Fin

### 2.2 Ajouter une règle à une base (en queue)
Lexique:
    Resultat : —
    Donnee : bc : BC, r : Regle
Algo:
    Procedure AJOUTER_REGLE_QUEUE(bc : BC, r : Regle)
    Debut
        ajouterEnQueue(bc.regles, r)
    Fin

### 2.3 Accéder à la règle se trouvant en tête de la base
Lexique:
    Resultat : r : Regle | ⊥
    Donnee : bc : BC
Algo:
    Fonction REGLE_TETE(bc : BC) : Regle
    Debut
        Si estVide(bc.regles) Alors
            REGLE_TETE <- ⊥
        Sinon
            REGLE_TETE <- tete(bc.regles)
        FSi
    Fin

---

## Question 3 — Moteur d’Inférence (avant-chaînage)

Lexique:
    Resultat : BFp : Liste<Proposition>
    Donnee : BF : Liste<Proposition>, bc : BC
    Variables : changement : Booleen, r : Regle, L : Liste<Proposition>, p : Proposition, c : Proposition
Algo:
    Fonction MOTEUR_INFERENCE(BF : Liste<Proposition>, bc : BC) : Liste<Proposition>
    Debut
        changement <- Vrai
        TantQue changement Faire
            changement <- Faux
            PourChaque r dans bc.regles Faire
                // Tester si toutes les propositions de la prémisse sont dans BF
                toutesVraies <- Vrai
                L <- copie(r.premisse)
                TantQue (toutesVraies = Vrai) ET (estVide(L) = Faux) Faire
                    p <- tete(L)
                    Si contiennent(BF, p) = Faux Alors
                        toutesVraies <- Faux
                    Sinon
                        L <- queue(L)
                    FSi
                FTantQue
                Si toutesVraies = Vrai Alors
                    c <- CONCLUSION(r)
                    Si contiennent(BF, c) = Faux Alors
                        ajouterEnQueue(BF, c)
                        changement <- Vrai
                    FSi
                FSi
            FPourChaque
        FTantQue
        MOTEUR_INFERENCE <- BF
    Fin

Notes:
- `contiennent(BF, p)` désigne l'opération d'appartenance sur la liste des faits (équivalente à `contient`).
- Le moteur applique les règles tant qu'il enrichit la base de faits; terminaison au point fixe.
