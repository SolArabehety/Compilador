include macros.asm
include macros2.asm
include number.asm

.MODEL LARGE
.386
.STACK 200h

.CODE

MAIN:

MOV AX, @DATA
MOV DS, AX
MOV ES, AX

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

MOV AX, 4C00h
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

END MAIN
