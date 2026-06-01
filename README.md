# Compilateur Pascal - Rendu Final
**Auteur :** Di Grande Nicolas

* Les cinq premiers TP ont été réalisés. Le code est complet, fonctionnel et inclut tout ce qui est demandé.
* Les TP sont répartis dans des dossiers distincts, numérotés de 1 à 5.

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
