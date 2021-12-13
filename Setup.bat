@echo off
SetLocal EnableDelayedExpansion

if not exist lib\SDL\build-windows\Debug\SDL2d.lib (

	if not exist lib\SDL\build-windows\SDL2.sln (

		if not exist lib\SDL\include\SDL.h (
			git submodule update --init lib/SDL
		)
		if not exist lib\SDL\build-windows (
			md lib\SDL\build-windows
		)
		cmake -S lib\SDL\ -B lib\SDL\build-windows
		cmake --build lib\SDL\build-windows
		
	)
)

if not exist lib\assimp\build-windows\lib\Debug\assimp-vc142-mtd.lib (

	if not exist lib\assimp\build-windows\Assimp.sln (

		if not exist lib\assimp\include\assimp\Importer.h (
			git submodule update --init lib\assimp
		)
		if not exist lib\assimp\build-windows (
			md lib\assimp\build-windows
		)
		cmake -S lib\assimp\ -D BUILD_SHARED_LIBS=OFF -B lib\assimp\build-windows
		cmake --build lib\assimp\build-windows
		
	)
)
