
L = lib

LIB_TARGET = libnre.a
EXEC_TARGET =

SRC =	$S/mft/matrix_project.cpp $S/mft/matrix_transform.cpp \
		$S/mft/vertex_utils.cpp $S/Object/Mesh.class.cpp \
		$S/Object/MeshObject.class.cpp \

CPPFLAGS = 
LDFLAGS =
INCLUDE =

LIB_MOD = 
CMAKE_LIB_MOD = SDL assimp

#libft_LIB = libft.a
#libft_INC = inc

SDL_LIB =
SDL_INC = include

assimp_LIB = lib/libassimp.a
assimp_INC = include build/include

LDFLAGS += 
