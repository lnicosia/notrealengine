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
		
	)
	for /f "delims=" %%i in ('lib\vswhere.exe -version 16 -property installationPath -nologo') do (
		set msbuild-path="%%i\MSBuild\Current\Bin\MSBuild.exe"
		echo Launching !msbuild-path!...
		!msbuild-path! lib\SDL\build-windows\SDL2.sln
	)
)

if not exist lib\assimp\build-windows\lib\Debug\assimp-vc142-mtd.lib (

	if not exist lib\assimp\build-windows\Assimp.sln (

		if not exist lib\assimp\include\assimp\Importer.h (
			git submodule update --init lib/assimp
		)
		if not exist lib\assimp\build-windows (
			md lib\assimp\build-windows
		)
		cmake -S lib\assimp\ -D GENERATE_SHARED_LIBDS=OFF -B lib\assimp\build-windows
		
	)
	for /f "delims=" %%i in ('lib\vswhere.exe -version 16 -property installationPath -nologo') do (
		set msbuild-path="%%i\MSBuild\Current\Bin\MSBuild.exe"
		echo Launching !msbuild-path!...
		!msbuild-path! lib\assimp\build-windows\assimp.sln
	)
)
