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
	FLD @int17 
	FSUB 
	FSTP @aux5
	FLD @aux5 
	FSTP b
	FLD a 
	FLD @int18 
	FMUL 
	FSTP @aux7
	FLD @aux7 
	FSTP a
	FLD b 
	FLD @int19 
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
	FMUL 
	FSTP @aux13
	FLD @aux13 
	FSTP d
	FLD @int20 
	FLD @int21 
	FADD 
	FSTP @aux15
	FLD @aux15 
	FLD @int22 
	FMUL 
	FSTP @aux16
	FLD @aux16 
	FSTP test_expr
	FLD a 
	FCOMP b 
	FSTSW AX 
	SAHF
	JNA TAG21
	MOV  SI, OFFSET @str23 
	MOV  DI, OFFSET g 
	STRCPY

TAG21:
	FLD a 
	FCOMP b 
	FSTSW AX 
	SAHF
	JNB TAG26
	FLD d 
	FCOMP e 
	FSTSW AX 
	SAHF
	JNAE TAG28

TAG26:
	MOV  SI, OFFSET @str23 
	MOV  DI, OFFSET h 
	STRCPY

TAG28:
	FLD d 
	FCOMP e 
	FSTSW AX 
	SAHF
	JNE TAG34
	FLD a 
	FCOMP f 
	FSTSW AX 
	SAHF
	JE TAG34
	MOV  SI, OFFSET @str24 
	MOV  DI, OFFSET h 
	STRCPY

TAG34:
	FLD @int21 
	FLD @int21 
	FADD 
	FSTP @aux35
	FLD @aux35 
	FSTP @factAux
	FLD @int28 
	FSTP @fact

TAG38:
	FLD @factAux 
	FCOMP @int28 
	FSTSW AX 
	SAHF
	JNA TAG46
	FLD @fact 
	FLD @factAux 
	FMUL 
	FSTP @aux41
	FLD @aux41 
	FSTP @fact
	FLD @factAux 
	FLD @int28 
	FSUB 
	FSTP @aux43
	FLD @aux43 
	FSTP @factAux
	JMP TAG38

TAG46:
	FLD @int19 
	FLD @fact 
	FADD 
	FSTP @aux47
	FLD @int25 
	FLD @aux47 
	FMUL 
	FSTP @aux48
	FLD @aux48 
	FSTP b
	FLD @int21 
	FSTP @factAux
	FLD @int28 
	FSTP @fact

TAG52:
	FLD @factAux 
	FCOMP @int28 
	FSTSW AX 
	SAHF
	JNA TAG60
	FLD @fact 
	FLD @factAux 
	FMUL 
	FSTP @aux55
	FLD @aux55 
	FSTP @fact
	FLD @factAux 
	FLD @int28 
	FSUB 
	FSTP @aux57
	FLD @aux57 
	FSTP @factAux
	JMP TAG52

TAG60:
	FLD @fact 
	FSTP @combN
	FLD @int20 
	FSTP @factAux
	FLD @int28 
	FSTP @fact

TAG64:
	FLD @factAux 
	FCOMP @int28 
	FSTSW AX 
	SAHF
	JNA TAG72
	FLD @fact 
	FLD @factAux 
	FMUL 
	FSTP @aux67
	FLD @aux67 
	FSTP @fact
	FLD @factAux 
	FLD @int28 
	FSUB 
	FSTP @aux69
	FLD @aux69 
	FSTP @factAux
	JMP TAG64

TAG72:
	FLD @fact 
	FSTP @combK
	FLD @int21 
	FLD @int20 
	FSUB 
	FSTP @aux74
	FLD @aux74 
	FSTP @factAux
	FLD @int28 
	FSTP @fact

TAG77:
	FLD @factAux 
	FCOMP @int28 
	FSTSW AX 
	SAHF
	JNA TAG85
	FLD @fact 
	FLD @factAux 
	FMUL 
	FSTP @aux80
	FLD @aux80 
	FSTP @fact
	FLD @factAux 
	FLD @int28 
	FSUB 
	FSTP @aux82
	FLD @aux82 
	FSTP @factAux
	JMP TAG77

TAG85:
	FLD @combK 
	FLD @fact 
	FMUL 
	FSTP @aux86
	FLD @combN 
	FLD @aux86 
	FDIV 
	FSTP @aux87
	FLD @aux87 
	FSTP a
	FLD @int32 
	FSTP b

TAG90:
	FLD a 
	FCOMP @int20 
	FSTSW AX 
	SAHF
	JNA TAG105

TAG93:
	FLD b 
	FCOMP @int22 
	FSTSW AX 
	SAHF
	JNB TAG99
	FLD b 
	FLD @int28 
	FADD 
	FSTP @aux96
	FLD @aux96 
	FSTP b
	JMP TAG93

TAG99:
	FLD a 
	FLD @int28 
	FSUB 
	FSTP @aux100
	FLD @aux100 
	FSTP a
	FLD d 
	FLD @int20 
	FMUL 
	FSTP @aux102
	FLD @aux102 
	FSTP d
	JMP TAG90

TAG105:
	displayString @str23
	newLine
	displayString h
	newLine
	getString g
	displayString g
	newLine
	DisplayFloat @flt33, 2
	newLine
	DisplayFloat test_expr, 2
	newLine
	GetFloat a1
	DisplayFloat a1, 2
	newLine
	DisplayFloat @int34, 2
	newLine
	DisplayFloat a, 2
	newLine
	GetFloat d
	DisplayFloat d, 2
	newLine

MOV EAX, 4C00h
INT 21h


.DATA
;Tabla de simbolos

a1 dd ?
b1 dd ?
test_expr dd ?
variable1 db 30 dup (?), "$"
g db 30 dup (?), "$"
h db 30 dup (?), "$"
p1 dd ?
p2 dd ?
p3 dd ?
a dd ?
b dd ?
d dd ?
e dd ?
f dd ?
@flt14 dd 9.460000
@flt15 dd 0.600000
@flt16 dd 6.000000
@int17 dd 155.000000
@int18 dd 4.000000
@int19 dd 12.000000
@int20 dd 2.000000
@int21 dd 5.000000
@int22 dd 3.000000
@str23 db "sarasa", "$", 30 dup (?)
@str24 db "zaraza", "$", 30 dup (?)
@int25 dd 15.000000
@fact dd ?
@factAux dd ?
@int28 dd 1.000000
@comb dd ?
@combN dd ?
@combK dd ?
@int32 dd 0.000000
@flt33 dd 1.400000
@int34 dd 77.000000
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
@aux102 dd ?

END MAIN
