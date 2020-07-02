include macros.asm
include macros2.asm
include number.asm

.MODEL LARGE
.386
.STACK 200h

.CODE

MAIN:

MOV EAX, @DATA
MOV DS, EAX
MOV ES, EAX

;Seccion de codigo
	displayString @str5
	newLine
	MOV  SI, OFFSET @str6 
	MOV  DI, OFFSET str1 
	STRCPY
	displayString str1
	newLine
	displayString @str7
	newLine
	getString str1
	displayString @str8
	newLine
	displayString str1
	newLine
	displayString @str9
	newLine
	FLD @flt10 
	FSTP f1
	DisplayFloat f1, 2
	newLine
	displayString @str11
	newLine
	GetFloat f2
	displayString @str12
	newLine
	DisplayFloat f2, 2
	newLine
	displayString @str13
	newLine
	FLD @int14 
	FSTP i1
	DisplayFloat i1, 2
	newLine
	displayString @str15
	newLine
	GetFloat i2
	displayString @str16
	newLine
	DisplayFloat i2, 2
	newLine

MOV EAX, 4C00h
INT 21h


.DATA
;Tabla de simbolos

str1 db 30 dup (?), "$"
f1 dd ?
f2 dd ?
i1 dd ?
i2 dd ?
@str5 db "Hola :)", "$", 30 dup (?)
@str6 db "Esta es una string de prueba", "$", 30 dup (?)
@str7 db "Inserte su nombre", "$", 30 dup (?)
@str8 db "Su nombre es:", "$", 30 dup (?)
@str9 db "Float de ejemplo:", "$", 30 dup (?)
@flt10 dd 1.400000
@str11 db "Ingrese un float:", "$", 30 dup (?)
@str12 db "Su float es:", "$", 30 dup (?)
@str13 db "Int de ejemplo:", "$", 30 dup (?)
@int14 dd 7.000000
@str15 db "Ingrese un int:", "$", 30 dup (?)
@str16 db "Su int es:", "$", 30 dup (?)

END MAIN
