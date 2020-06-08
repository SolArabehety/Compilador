.MODEL LARGE
.386
.STACK 200h

.DATA
;Tabla de simbolos


.CODE

MOVE EAX, @DATA
MOVE DS, EAX
MOVE ES, EAX

;Seccion de codigo

MOVE EAX, 4C00h
INT 21h

END;
