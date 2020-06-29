tasm NUMBERS.asm
tasm final.asm

tlink /3 final.obj numbers.obj /v /s /m

pause
final.exe Prueba.txt
pause

del final.obj
del numbers.lst
del final.obj
del numbers.lst

pause