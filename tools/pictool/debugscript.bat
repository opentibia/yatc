copy bin\debug\pictool.exe pictool.exe
copy ..\..\tibiaold.pic tibia.pic

rem pictool tibia.pic 0 tibia0.bmp 
rem pictool tibia.pic 0 tibia0.bmp --topic
rem move __tmp__.pic tibia.pic

pictool tibia.pic 1 tibia1.bmp 
pictool tibia.pic 1 tibia1.bmp --topic
move __tmp__.pic tibia.pic

rem pictool tibia.pic 2 tibia2.bmp 
rem pictool tibia.pic 2 tibia2.bmp --topic
rem move __tmp__.pic tibia.pic

pause

file2hex tibia.pic
file2hex tibiaoriginal.pic
C:\progra~1\tortoisesvn\bin\tortoisemerge.exe "tibia.pic.txt" "tibiaoriginal.pic.txt"
pause

pictool tibia.pic > tibia.pic.desc.txt
pictool tibiaoriginal.pic > tibiaoriginal.pic.desc.txt

C:\progra~1\tortoisesvn\bin\tortoisemerge.exe tibia.pic.desc.txt tibiaoriginal.pic.desc.txt
pause

copy tibia.pic C:\progra~1\tibia\tibia.pic
c:
cd progra~1\tibia\
tibia.exe

