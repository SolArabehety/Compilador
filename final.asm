.MODEL LARGE
.386
.STACK 200h

.DATA
;Tabla de simbolos

a1 dd ?
b1 dd ?
test_expr dd ?
variable1 db 30 dup (?),"$"
g db 30 dup (?),"$"
h db 30 dup (?),"$"
p1 dd ?
p2 dd ?
p3 dd ?
a dd ?
b dd ?
d dd ?
e dd ?
f dd ?
const1 dd 9.460000
const2 dd 0.600000
const3 dd 6.000000
_155 dd 155.000000
_4 dd 4.000000
_12 dd 12.000000
_2 dd 2.000000
_5 dd 5.000000
_3 dd 3.000000
_sarasa db "sarasa", "$", 30 dup (?)
_zaraza db "zaraza", "$", 30 dup (?)
_15 dd 15.000000
@fact dd ?
@factAux dd ?
_1 dd 1.000000
@comb dd ?
@combN dd ?
@combK dd ?
_0 dd 0.000000

.CODE

MOVE EAX, @DATA
MOVE DS, EAX
MOVE ES, EAX

;Seccion de codigo

MOVE EAX, 4C00h
INT 21h

END;
