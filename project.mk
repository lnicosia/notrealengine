
L = lib
I = inc/notrealengine inc

LIB_TARGET = libnre.a
EXEC_TARGET =

SRC =	$S/mft/math_utils.cpp \
		$S/Object/GLMesh.class.cpp \
		$S/Object/GLObject.class.cpp \
		$S/Object/MeshData.class.cpp \
		$S/Object/Mesh.class.cpp \
		$S/Object/Texture.class.cpp \
		$S/Object/Animation.class.cpp \
		$S/Object/Bone.class.cpp \
		$S/Object/Transform.class.cpp \
		$S/Object/AssetManager.class.cpp \
		$S/Object/Asset.class.cpp \
		$S/Object/Light.class.cpp \
		$S/Object/Scene.class.cpp \
		$S/Object/InitBobby.cpp \
		$S/Object/AssimpHelpers.cpp \
		$S/SDL.class.cpp \
		$S/SDLWindow.class.cpp \
		$S/GLException.class.cpp \
		$S/GLShaderProgram.class.cpp \
		$S/GLContext.class.cpp \
		$S/GLContext_SDL.class.cpp \
		$S/TextRendering/GLCharacter.class.cpp \
		$S/TextRendering/GLFont.class.cpp \
		$S/TextRendering/Freetype.class.cpp \
    $S/Inputs/Binding.class.cpp \
		$S/Inputs/SDLEvents.class.cpp \
		$S/Camera.class.cpp \
		$S/ColladaParser/ColladaParser.class.cpp \
		$S/LXML/LXML.class.cpp \


CC = clang++ --std=c++20 -O3
CPPFLAGS =
LDFLAGS =
INCLUDE =

LIB_MOD =
CMAKE_LIB_MOD = SDL assimp freetype

#libft_LIB = libft.a
#libft_INC = inc

SDL_LIB =
SDL_INC = include

assimp_LIB = lib/libassimp.a
assimp_INC = include build/include

freetype_LIB = libfreetype.a
freetype_INC = include
