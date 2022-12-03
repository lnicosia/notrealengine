S = src
O = obj
L = lib
I = inc
D = dep

define n


endef

CC = clang++ --std=c++17

include project.mk

ifeq ($L,)
	LIB_MOD := 
	CMAKE_LIB_MOD := 
else
	ifeq ($(wildcard $L),)
$(error lib directory not found : "$L"$n\
	If you are calling make from another directory, please give the path to the lib folder to this makefile)
	endif
endif

RM = rm -fv $1
RMDIR = $(if $(wildcard $1),$(if $(if $1,$(shell ls $1),),$(warning "$1 is not empty, not removed"),rmdir $1))

OS_AR := $(shell find "/usr/bin" -name "llvm-ar*" | sort -n | head -n1 )
OS_RANLIB := $(shell find "/usr/bin" -name "llvm-ranlib*" | sort -n | head -n1 )
AR = $(word 1,ar)
RANLIB = $(word 1,ranlib)

DEP =	$(SRC:$S/%.cpp=$D/%.d) $(CPPFLAGS)
OBJ =	$(SRC:$S/%.cpp=$O/%.o)

TMP_DIRS=$(sort $(foreach DIRS,$(OBJ) $(DEP),$(dir $(shell echo '$(DIRS)' | sed ':do;p;s,^\(.*\)/[^/]*$$,\1,;\%^\(.*\)/[^/]*$$%b do;d'))))

INCLUDES += $I

$(foreach MOD,$(LIB_MOD),$(eval $(MOD)_DIR?=$L/$(MOD)))

LIB = $(foreach MOD,$(LIB_MOD),$(patsubst %,$($(MOD)_DIR)/%,$($(MOD)_LIB)))
INCLUDES += $(foreach MOD,$(LIB_MOD),$(patsubst %,$($(MOD)_DIR)/%,$($(MOD)_INC)))
LDFLAGS += $(foreach LIBRARY,$(LIB),-L$(dir $(LIBRARY)) -l$(patsubst lib%.a,%,$(notdir $(LIBRARY))))

LIB_DEP = $(LIB:%=%.d)

$(foreach MOD,$(CMAKE_LIB_MOD),$(eval $(MOD)_DIR?=$L/$(MOD)))
$(foreach MOD,$(EXTERNAL_LIB_MOD),$(eval $(MOD)_DIR?=$L/$(MOD)))

CMAKE_LIB = $(foreach MOD,$(CMAKE_LIB_MOD),$(if $($(MOD)_LIB),$(patsubst %,$($(MOD)_DIR)/build/%,$($(MOD)_LIB))))
INCLUDES += $(foreach MOD,$(CMAKE_LIB_MOD),$(if $($(MOD)_INC),$(patsubst %,$($(MOD)_DIR)/%,$($(MOD)_INC))))
LDFLAGS += $(foreach LIBRARY,$(CMAKE_LIB),-L$(dir $(LIBRARY)) -l$(patsubst lib%.a,%,$(notdir $(LIBRARY))))

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CPPFLAGS +=
	LDFLAGS +=
else ifeq ($(UNAME_S),Darwin)
	CPPFLAGS +=
	LDFLAGS +=
else
$(error OS not supported)
endif

.PHONY: all, clean, fclean, libclean, realclean, re, relib, space, force, cmakelibs
.PHONY: $(foreach MOD,$(LIB_MOD),$(if $(shell $(MAKE) -C $($(MOD)_DIR) -q $($(MOD)_LIB) || echo false),,$($(MOD)_DIR)/$($(MOD)_LIB),))

all: $(LIB_TARGET) $(EXEC_TARGET)

$(TMP_DIRS) $I:
	@mkdir -p $@

.SECONDEXPANSION:
$D/%.d: $S/%.cpp Makefile | $$(dir $$@) $(INCLUDES)
	$(info Updating dep list for $<)
	@$(CC) -MM -MG -MP $(CPPFLAGS) $(INCLUDES:%=-I%) $< | \
		sed 's,$(notdir $*)\.o[ :]*,$O/$*.o $@ : ,g' > $@; \

.SECONDEXPANSION:
$(OBJ): $O/%.o: $S/%.cpp | $$(dir $$@) $(INCLUDES)
	$(CC) -c -o $@ $(CPPFLAGS) $(INCLUDES:%=-I%) $<

define submodule_init
$(if $(shell git submodule status $(1) | grep '^-'),git submodule update --init $(1),)
endef

define init_includes
$($(1)_INC:%=$($(1)_DIR)/%):
	$(call submodule_init,$($(1)_DIR))

endef

$(foreach MOD,$(LIB_MOD) $(CMAKE_LIB_MOD),$(eval $(call init_includes,$(MOD))))

$(foreach MOD,$(CMAKE_LIB_MOD),$(eval $($(MOD)_DIR)/build/$($(MOD)_LIB): MOD = $(MOD)))
$(CMAKE_LIB): DIR = $($(MOD)_DIR)/build

$(CMAKE_LIB):
	@echo "Creating lib $@"
	@$(call submodule_init,$($(MOD)_DIR))
	@mkdir -p $(DIR)
	sh -c "cd $(DIR); cmake .. $(CMAKE_FLAGS)"
	@$(MAKE) -C $(DIR)

$(foreach MOD,$(LIB_MOD),$(eval $($(MOD)_DIR)/$($(MOD)_LIB): MOD = $(MOD)))
$(LIB): DIR = $($(MOD)_DIR)

$(LIB):
	$(call submodule_init,$(DIR))
	$(MAKE) -C $(DIR) $($(MOD)_LIB) L='$(abspath $L)'

$(EXEC_TARGET): $(OBJ) $(LIB) project.mk | $(CMAKE_LIB)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(LIB_TARGET): CMAKE_LIB_MOD = SDL
$(LIB_TARGET): $(if $(wildcard $(LIB_TARGET_EXTERNAL)),fclean) $(OBJ) project.mk
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

$(LIB_TARGET_EXTERNAL): CPPFLAGS += -D USING_EXTERNAL_LIBS
$(LIB_TARGET_EXTERNAL): $(if $(wildcard $(LIB_TARGET)),fclean) $(OBJ) $(CMAKE_LIB) project.mk
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

$(patsubst %,clean@%,$(OBJ) $(DEP) $(EXEC_TARGET) $(LIB_TARGET) $(LIB_TARGET_EXTERNAL)): clean@%:
	@$(call RM,$*)

$(foreach FILE,$(OBJ) $(DEP) $(TMP_DIR),$(eval $(if $(filter-out ./,$(dir $(FILE))),clean@$(dir $(FILE)): clean@$(FILE),)))

$(TMP_DIRS:%=clean@%):
	@$(call RMDIR,$%)

clean: $(TMP_DIRS:%=clean@%)

fclean: clean $(patsubst %,clean@%,$(EXEC_TARGET) $(LIB_TARGET) $(LIB_TARGET_EXTERNAL))

$(LIB_MOD:%=libclean@$L/%): libclean@%:
	$(MAKE) -s -C $* fclean "L="

$(CMAKE_LIB_MOD:%=libclean@$L/%/build): libclean@%:
	rm -Rf $*

libclean: $(LIB_MOD:%=libclean@$L/%)

realclean: $(CMAKE_LIB_MOD:%=libclean@$L/%/build)

re: fclean all
relib: libclean all

force:
	@true

-include customrules.mk


ifeq ($(filter %clean relib re %.d,$(MAKECMDGOALS)),)
-include $(patsubst $O/%.o,$D/%.d,$(wildcard $(OBJ)))
endif
