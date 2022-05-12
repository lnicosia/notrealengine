@echo off
SetLocal EnableDelayedExpansion

if not exist ..\lib\SDL\include\SDL.h (
	git submodule update --init ..\lib\SDL
)