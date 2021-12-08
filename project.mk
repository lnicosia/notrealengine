
L = lib
I = inc/notrealengine inc

LIB_TARGET = libnre.a
EXEC_TARGET =

SRC =	$S/Object/GLMesh.class.cpp \
		$S/Object/GLObject.class.cpp \
		$S/Object/MeshData.class.cpp \
		$S/SDL.class.cpp \
		$S/SDLWindow.class.cpp \
		$S/GLException.class.cpp \
		$S/GLShaderProgram.class.cpp \
		$S/GLContext.class.cpp \
		$S/GLContext_SDL.class.cpp \

CC = clang++ --std=c++20
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

LDFLAGS += 
