lib/assimp/build/lib/libassimp.a: CMAKE_FLAGS += -D BUILD_SHARED_LIBS=OFF \
-D ASSIMP_BUILD_TESTS=OFF
lib/SDL/build/lib/libSDL2.a: CMAKE_FLAGS += -D SDL_BUILD_TESTS=OFF

lib/assimp/build/include: $(if $(filter $(LIB_TARGET_EXTERNAL),$(MAKECMDGOALS)),lib/assimp/build/lib/libassimp.a)
