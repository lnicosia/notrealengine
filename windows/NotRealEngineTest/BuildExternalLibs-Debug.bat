@echo off
SetLocal EnableDelayedExpansion

if not exist ..\..\lib\SDL\build-windows\Debug\SDL2d.lib (

	if not exist ..\..\lib\SDL\include\SDL.h (
		git submodule update --init ..\..\lib\SDL
	)
	if not exist ..\..\lib\SDL\build-windows (
		md ..\..\lib\SDL\build-windows
	)
	cmake -S ..\..\lib\SDL\ -B ..\..\lib\SDL\build-windows
	cmake --build ..\..\lib\SDL\build-windows --config Debug
	copy ..\..\lib\SDL\build-windows\Debug\SDL2d.dll ..\x64\UsingExternalLibs-Debug
		
)

if not exist ..\x64\UsingExternalLibs-Debug\SDL2d.dll  (
	copy ..\..\lib\SDL\build-windows\Debug\SDL2d.dll ..\x64\UsingExternalLibs-Debug
)

if not exist ..\..\lib\assimp\build-windows\lib\Debug\assimpd.lib (

	if not exist ..\..\lib\assimp\include\assimp\Importer.h (
		git submodule update --init ..\..\lib\assimp
	)
	if not exist ..\..\lib\assimp\build-windows (
		md ..\..\lib\assimp\build-windows
	)
	cmake -S ..\..\lib\assimp\ -D BUILD_SHARED_LIBS=OFF -B ..\..\lib\assimp\build-windows
	cmake --build ..\..\lib\assimp\build-windows --config Debug
		
)

if not exist ..\x64\UsingExternalLibs-Debug\assimpd.exe  (
	copy ..\..\lib\assimp\build-windows\bin\Debug\assimpd.exe ..\x64\UsingExternalLibs-Debug
)

if not exist ..\..\lib\freetype\build-windows\Debug\freetyped.lib (

	if not exist ..\..\lib\freetype\include\freetype\freetype.h (
		git config --global http.sslverify false
		git submodule update --init ..\..\lib\freetype
	)
	if not exist ..\..\lib\freetype\build-windows (
		md ..\..\lib\freetype\build-windows
	)
	cmake -S ..\..\lib\freetype\ -B ..\..\lib\freetype\build-windows
	cmake --build ..\..\lib\freetype\build-windows --config Debug
		
)