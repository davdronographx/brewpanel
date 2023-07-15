@echo off

cl.exe /I dependencies /I src\\ /Zi /Fe: bin\\brewpanel.exe src\\brewpanel-win32.cpp /Fd: bin\\brewpanel.pdb /Fo: bin\\brewpanel.obj /link user32.lib gdi32.lib   /DEBUG:FULL

@REM del bin\*.raf