include macros2.asm
include number.asm

.MODEL LARGE
.386
.STACK 200h

.CODE

MOVE AX, @DATA
MOVE DS, AX
MOVE ES, AX

;Seccion de codigo
	displayString _Hola_:)
	FLD _Esta_es_una_string_de_prueba 
	FSTP str1
	displayString str1
	displayString _Inserte_su_nombre
	getString str1
	displayString _Su_nombre_es:
	displayString str1

MOVE EAX, 4C00h
INT 21h


.DATA
;Tabla de simbolos

str1 db 30 dup (?),"$"
f1 dd ?
f2 dd ?
i1 dd ?
i2 dd ?
_Hola_:) db "Hola :)", "$", 30 dup (?)
_Esta_es_una_string_de_prueba db "Esta es una string de prueba", "$", 30 dup (?)
_Inserte_su_nombre db "Inserte su nombre", "$", 30 dup (?)
_Su_nombre_es: db "Su nombre es:", "$", 30 dup (?)

END;
