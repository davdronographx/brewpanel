@echo off

cl.exe /w /I dependencies /I src\\ /LD /Zi src\\brewpanel.cpp /Fd: bin\\brewpanel.pdb /Fo: bin\\brewpanel.obj /Fe: bin\\brewpanel.dll /link user32.lib gdi32.lib opengl32.lib /DEBUG:FULL

@REM del bin\*.raf