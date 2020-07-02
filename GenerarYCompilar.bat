flex Lexico.l
bison -dyv Sintactico.y
gcc.exe lex.yy.c y.tab.c -o Tercera.exe

Tercera.exe Prueba.txt

tasm NUMBERS.asm
tasm final.asm

tlink /3 final.obj numbers.obj /v /s /m

final.exe

del lex.yy.c
del y.tab.c
del y.output
del y.tab.h
del Primera.exe
pause
