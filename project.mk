
L = lib
I = inc/notrealengine inc

LIB_TARGET = libnre.a
EXEC_TARGET =

SRC =	$S/mft/math_utils.cpp \
		$S/notrealengine/Object/GLMesh.class.cpp \
		$S/notrealengine/Object/GLObject.class.cpp \
		$S/notrealengine/Object/MeshData.class.cpp \
		$S/notrealengine/Object/Mesh.class.cpp \
		$S/notrealengine/Object/Texture.class.cpp \
		$S/notrealengine/Object/Animation.class.cpp \
		$S/notrealengine/Object/Bone.class.cpp \
		$S/notrealengine/Object/Transform.class.cpp \
		$S/notrealengine/Object/AssetManager.class.cpp \
		$S/notrealengine/Object/Asset.class.cpp \
		$S/notrealengine/Object/Light.class.cpp \
		$S/notrealengine/Object/Scene.class.cpp \
		$S/notrealengine/Object/InitBobby.cpp \
		$S/notrealengine/Object/AssimpHelpers.cpp \
		$S/notrealengine/Object/ObjectImporter.class.cpp \
		$S/notrealengine/Object/AssimpObjectImporter.class.cpp \
		$S/notrealengine/Object/CustomObjectImporter.class.cpp \
		$S/notrealengine/SDL.class.cpp \
		$S/notrealengine/SDLWindow.class.cpp \
		$S/notrealengine/GLException.class.cpp \
		$S/notrealengine/GLShaderProgram.class.cpp \
		$S/notrealengine/GLContext.class.cpp \
		$S/notrealengine/GLContext_SDL.class.cpp \
		$S/notrealengine/TextRendering/GLCharacter.class.cpp \
		$S/notrealengine/TextRendering/GLFont.class.cpp \
		$S/notrealengine/TextRendering/Freetype.class.cpp \
    	$S/notrealengine/Inputs/Binding.class.cpp \
		$S/notrealengine/Inputs/SDLEvents.class.cpp \
		$S/notrealengine/Camera.class.cpp \
		$S/notrealengine/ColladaParser/ColladaParser.class.cpp \
		$S/notrealengine/ColladaParser/ColladaException.class.cpp \
		$S/notrealengine/ColladaParser/ColladaSceneBuilder.class.cpp \
		$S/LXML/LXML.class.cpp \
		$S/notrealengine/Tests/ColladaParserTests.cpp \
		$S/notrealengine/Tests/RunTests.cpp \
		$S/notrealengine/UI/Button.class.cpp \
		$S/notrealengine/UI/UIElement.class.cpp \
		$S/notrealengine/UI/UIManager.class.cpp \
		$S/notrealengine/CheckFileType.cpp

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
