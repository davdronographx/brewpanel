@echo off

del bin\\*.* /Q

xcopy img\\*.* bin /Y

xcopy dependencies\\sdl2\\lib\\x64\\SDL2.dll bin /Y

cl.exe /I dependencies /I dependencies/sdl2/include /I dependencies/rs-232 /I src\\ /Zi /Fe: bin\\brewpanel.exe src\\panel\\brewpanel-sdl2.cpp /Fd: bin\\brewpanel.pdb /Fo: bin\\brewpanel.obj /link /LIBPATH:dependencies/sdl2/lib/x64 SDL2.lib SDL2main.lib user32.lib gdi32.lib SetupAPI.lib Advapi32.lib  /DEBUG:FULL
