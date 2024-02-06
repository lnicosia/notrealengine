@echo off
SetLocal EnableDelayedExpansion

if not exist ..\lib\SDL\include\SDL.h (
	git submodule update --init ..\lib\SDL
)

if not exist src (
	mklink /j src ..\src
)

if not exist inc (
	mklink /j inc ..\inc
)
