@echo off

avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o bin\\brewpanel-control.o src\\control\\brewpanel-control-avr.cpp