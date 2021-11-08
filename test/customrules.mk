
lib/SDL:
	git submodule init $@
	git -C $@ checkout 25f9ed8

lib/SDL/build/$(SDL_LIB): lib/SDL
