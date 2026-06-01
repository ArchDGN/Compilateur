[a, b, c, d]
BEGIN
    a := 0;
    
    FOR b := 1 TO 5 DO
        a := a + b;
        
    IF a == 15 THEN
        c := 99
    ELSE
        c := 0;
        
    d := 3;
    WHILE d > 0 DO
        d := d - 1
END.