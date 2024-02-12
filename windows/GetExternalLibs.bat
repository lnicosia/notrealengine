@echo off
SetLocal EnableDelayedExpansion

if not exist ..\lib\SDL\include\SDL.h (
	git submodule update --init ..\lib\SDL
)

:: assimp needs to be built to create config.h so the library needs to build it only for this.

echo Buildling assimp...

if not exist ..\lib\assimp\build-windows\lib\Debug\assimp-vc142-mtd.lib (

	if not exist ..\lib\assimp\build-windows\Assimp.sln (

		if not exist ..\lib\assimp\include\assimp\Importer.h (
			git submodule update --init
		)
		if not exist ..\lib\assimp\build-windows (
			md ..\lib\assimp\build-windows
		)
		cmake -S ..\lib\assimp\ -G "Visual Studio 16 2019" -T "v142" -B ..\lib\assimp\build-windows
		cmake --build ..\lib\assimp\build-windows
		
	)
)

if not exist ..\lib\freetype\include\freetype\freetype.h (
	git config --global http.sslverify false
	git submodule update --init ..\lib\freetype
)

if not exist src (
	mklink /j src ..\src
)

if not exist inc (
	mklink /j inc ..\inc
)