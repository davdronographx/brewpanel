@echo off

cl.exe /I dependencies /I src\\ /Zi /Fe: bin\\brewpanel.exe src\\brewpanel.cpp /Fd: bin\\brewpanel.pdb /Fo: bin\\brewpanel.obj /link user32.lib  /DEBUG:FULL

@REM del bin\*.raf