# Compilateur Pascal - Rendu Final
**Auteur :** Di Grande Nicolas

* Lien Github : https://github.com/ArchDGN/Compilateur
* Les cinq premiers TP ont été réalisés. Le code est complet, fonctionnel et inclut tout ce qui est demandé.
* Les TP sont répartis dans des dossiers distincts, numérotés de 1 à 5.

## 1. Travail personnel implémenté (Ce qui a été ajouté au code de base)

Voici les passages exacts de mon code qui correspondent à mon travail personnel :

**Dans `tokeniser.l` et `tokeniser.h` :**
* Ajout des règles lexicales pour les nouveaux mots-clés (`IF`, `THEN`, `ELSE`, `WHILE`, `DO`, `FOR`, `TO`, `BEGIN_TOK`, `END_TOK`, `DISPLAY`).

**Dans `compilateur.cpp` :**
* **Structures de contrôle (Génération d'assembleur et sauts) :** Création intégrale des fonctions `IfStatement()`, `WhileStatement()`, `ForStatement()` et `BlockStatement()`. Gestion de la variable globale `TagNumber` pour la création d'étiquettes uniques.
* **Instruction d'affichage :** Création de la fonction `Display()`, ajout de la section `.data` avec `FormatString1`, et appel sécurisé à `printf@PLT` avec sauvegarde de `%rbp`.
* **Analyse Sémantique (Typage) :** * Création de l'enum `TYPE` (`UNSIGNED_INT`, `BOOLEAN`, `UNKNOWN_TYPE`).
  * Modification des fonctions `Factor()`, `Term()`, `SimpleExpression()` et `Expression()` pour qu'elles retournent le type de l'expression calculée.
  * Ajout des vérifications de cohérence de types (génération d'erreurs si on mélange un entier et un booléen).
  * Modification de `AssignementStatement()` pour vérifier la compatibilité des types.

## 2. Exemples de code supportés

Voici un exemple complexe qui teste l'arithmétique, les boucles, les conditions et l'affichage, et qui compile parfaitement avec ce projet :

```pascal
[a, b, c, d]
BEGIN
    a := 0;
    
    FOR b := 1 TO 5 DO
        a := a + b;
        
    IF a == 15 THEN
        DISPLAY a
    ELSE
        c := 0;
        
    d := 3;
    WHILE d > 0 DO
        d := d - 1
END.
