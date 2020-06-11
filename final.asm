.MODEL LARGE
.386
.STACK 200h

.CODE

MOVE EAX, @DATA
MOVE DS, EAX
MOVE ES, EAX

;Seccion de codigo
FLD a 
FLD b 
FADD 
FSTP @aux1
FLD @aux1 
FSTP a
FLD a 
FLD b 
FADD 
FSTP @aux3
FLD @aux3 
FSTP a
FLD b 
FLD _155 
FSUB 
FSTP @aux5
FLD @aux5 
FSTP b
FLD a 
FLD _4 
FMULT 
FSTP @aux7
FLD @aux7 
FSTP a
FLD b 
FLD _12 
FDIV 
FSTP @aux9
FLD @aux9 
FSTP b
FLD a 
FLD b 
FADD 
FSTP @aux11
FLD @aux11 
FSTP d
FLD e 
FLD f 
FMULT 
FSTP @aux13
FLD @aux13 
FSTP d
FLD _2 
FLD _5 
FADD 
FSTP @aux15
FLD @aux15 
FLD _3 
FMULT 
FSTP @aux16
FLD @aux16 
FSTP test_expr


FLD _sarasa 
FSTP g






FLD _sarasa 
FSTP h





FLD _zaraza 
FSTP h

FLD _5 
FLD _5 
FADD 
FSTP @aux35
FLD @aux35 
FSTP @factAux
FLD _1 
FSTP @fact



FLD @fact 
FLD @factAux 
FMULT 
FSTP @aux41
FLD @aux41 
FSTP @fact
FLD @factAux 
FLD _1 
FSUB 
FSTP @aux43
FLD @aux43 
FSTP @factAux


FLD _12 
FLD @fact 
FADD 
FSTP @aux47
FLD _15 
FLD @aux47 
FMULT 
FSTP @aux48
FLD @aux48 
FSTP b
FLD _5 
FSTP @factAux
FLD _1 
FSTP @fact



FLD @fact 
FLD @factAux 
FMULT 
FSTP @aux55
FLD @aux55 
FSTP @fact
FLD @factAux 
FLD _1 
FSUB 
FSTP @aux57
FLD @aux57 
FSTP @factAux


FLD @fact 
FSTP @combN
FLD _2 
FSTP @factAux
FLD _1 
FSTP @fact



FLD @fact 
FLD @factAux 
FMULT 
FSTP @aux67
FLD @aux67 
FSTP @fact
FLD @factAux 
FLD _1 
FSUB 
FSTP @aux69
FLD @aux69 
FSTP @factAux


FLD @fact 
FSTP @combK
FLD _5 
FLD _2 
FSUB 
FSTP @aux74
FLD @aux74 
FSTP @factAux
FLD _1 
FSTP @fact



FLD @fact 
FLD @factAux 
FMULT 
FSTP @aux80
FLD @aux80 
FSTP @fact
FLD @factAux 
FLD _1 
FSUB 
FSTP @aux82
FLD @aux82 
FSTP @factAux


FLD @combK 
FLD @fact 
FSUB 
FSTP @aux86
FLD @combN 
FLD @aux86 
FDIV 
FSTP @aux87
FLD @aux87 
FSTP a
FLD _0 
FSTP b






FLD b 
FLD _1 
FADD 
FSTP @aux96
FLD @aux96 
FSTP b


FLD d 
FLD _2 
FMULT 
FSTP @aux100
FLD @aux100 
FSTP d





MOVE EAX, 4C00h
INT 21h


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
@aux1 dd ?
@aux3 dd ?
@aux5 dd ?
@aux7 dd ?
@aux9 dd ?
@aux11 dd ?
@aux13 dd ?
@aux15 dd ?
@aux16 dd ?
@aux35 dd ?
@aux41 dd ?
@aux43 dd ?
@aux47 dd ?
@aux48 dd ?
@aux55 dd ?
@aux57 dd ?
@aux67 dd ?
@aux69 dd ?
@aux74 dd ?
@aux80 dd ?
@aux82 dd ?
@aux86 dd ?
@aux87 dd ?
@aux96 dd ?
@aux100 dd ?

END;
