@echo off

del bin\\*.* /Q

xcopy img\\*.* bin /Y

cl.exe /I dependencies /I src\\ /Zi /Fe: bin\\brewpanel.exe src\\panel\\brewpanel-win32.cpp /Fd: bin\\brewpanel.pdb /Fo: bin\\brewpanel.obj /link user32.lib gdi32.lib Shell32.lib  /DEBUG:FULL
