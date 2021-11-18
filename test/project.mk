
L = ../lib

LIB_TARGET =
EXEC_TARGET = test

SRC =	$S/main.cpp \

CC = clang++ --std=c++20
CPPFLAGS =
LDFLAGS = -lpthread -ldl
INCLUDE =

LIB_MOD = notrealengine
CMAKE_LIB_MOD = SDL

notrealengine_DIR = ..
notrealengine_LIB = libnre.a
notrealengine_INC = inc

SDL_LIB = libSDL2.a
SDL_INC = include

LDFLAGS += 
