S = src/
O = obj/
L = lib/
I = inc/
D = dep/

define n


endef

include project.mk

ifneq ($L,)
	ifeq ($(wildcard $L),)
		$(error lib directory not found : "$L"$n\
			If you are calling make from another directory, please give the path to the lib folder to this makefile)
	endif
endif

CC = clang++ --std=c++17
RM = rm -fv
RMDIR = rm -Rfv

DEP =	$(SRC:$S%.cpp=$D%.d)
OBJ =	$(SRC:$S%.cpp=$O%.o)

LIB_TARGET_DEP = $(LIB_TARGET:%=%.d)

INCLUDE += -I$I

$(eval $(foreach MOD,$(LIB_MOD),$(MOD)_DIR?=$L$(MOD)))

LIB = $(foreach MOD,$(LIB_MOD),$($(MOD)_DIR)/$($(MOD)_LIB))
CPPFLAGS += $(foreach MOD,$(LIB_MOD),-I$($(MOD)_DIR)/$($(MOD)_INC))
LDFLAGS += $(foreach LIBRARY,$(LIB),-L$(dir $(LIBRARY)) -l$(patsubst lib%.a,%,$(notdir $(LIBRARY))))

LIB_DEP = $(LIB:%=%.d)

$(eval $(foreach MOD,$(CMAKE_LIB_MOD),$(MOD)_DIR?=$L$(MOD)))

CMAKE_LIB = $(foreach MOD,$(CMAKE_LIB_MOD),$($(MOD)_DIR)/build/$($(MOD)_LIB))
CPPFLAGS += $(foreach MOD,$(CMAKE_LIB_MOD),-I$($(MOD)_DIR)/$($(MOD)_INC))
LDFLAGS += $(foreach LIBRARY,$(CMAKE_LIB),-L$(dir $(LIBRARY)) -l$(patsubst lib%.a,%,$(notdir $(LIBRARY))))

TMP = $(DEP) $(OBJ) $(LIB_TARGET_DEP)
TMP_DIR = $O $D

UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CPPFLAGS +=
	LDFLAGS +=
else ifeq ($(UNAME_S),Darwin)
	CPPFLAGS +=
	LDFLAGS +=
else
err: $(error OS not supported)
endif

.PHONY: all, clean, fclean, libclean, realclean, re, relib, space, force, $(LIB_DEP)

all: $(LIB_TARGET) $(EXEC_TARGET)

$D:
	@mkdir -p $D

$O:
	@mkdir -p $O

$D%.d: $S%.cpp | $D
	@echo "Creating dep list for $<"
	@set -e; rm -f $@; \
		$(CC) -I$I -MM $(CPPFLAGS) $< | \
		sed 's,\($*\)\.o[ :]*,$O\1.o $@ : ,g' > $@; \

$O%.o: -include $D%.d
$O%.o: $S%.cpp | $O
	$(CC) -c -o $@ $(CPPFLAGS) $(INCLUDE) $<

$(CMAKE_LIB): MOD = $(word 2,$(subst /, ,$@))
$(CMAKE_LIB): DIR = $($(MOD)_DIR)/build

$(CMAKE_LIB):
	@mkdir -p $(DIR)
	@sh -c "cd $(DIR); cmake .."
	@$(MAKE) -C $(DIR)

$(eval $(foreach MOD,$(LIB_MOD),$(LIB) $(LIB_DEP): MOD = $(MOD)))
$(LIB) $(LIB_DEP): DIR = $($(MOD)_DIR)
$(LIB) $(LIB_DEP): RELATIVE = $(subst $($(MOD)_DIR)/,$(eval),$@)

$(LIB_DEP):
	@make -s -C $(DIR) $(RELATIVE) L='$(abspath $L)' --no-print-directory

$(LIB):
	make -C $(DIR) $(RELATIVE) L='$(abspath $L)'

$(EXEC_TARGET): $(OBJ) $(LIB) project.mk | $(CMAKE_LIB)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(LIB_TARGET_DEP): $(DEP)
	@echo "Updating dependencies for $(LIB_TARGET)"
	@echo $$(cat $(DEP) | sed "s/^.*://; s/\\\//g") | tr ' ' '\n' | sort -u | \
		sed 's:^.*$$:$$($$(MOD)_DIR)/&:' | tr '\n' ' ' | \
		echo '$$($$(MOD)_DIR)/$(LIB_TARGET):'" $$(cat -)" | \
		fold -w80 -s | sed "s/^.*$$/&\\\/; 2,$$ s//\\t&/" > $@

$(LIB_TARGET): $(OBJ) project.mk
	ar -rc $@ $(OBJ)
	ranlib $@

clean:
	@$(foreach file,$(wildcard $(TMP)), \
		$(RM) $(file); \
		)

fclean: clean
	@$(foreach file,$(wildcard $(EXEC_TARGET) $(LIB_TARGET)), \
		$(RM) $(file); \
		)
	@$(foreach file,$(wildcard $(TMP_DIR)), \
		rmdir $(file); \
		)

libclean: fclean
	@$(foreach lib,$(LIB_MOD:%=$L%), \
		$(MAKE) -s -C $(lib) fclean "L="; \
		)

realclean: libclean
	@$(foreach DIR,$(wildcard $(CMAKE_LIB_MOD:%=$L%/build)), \
		$(RMDIR) $(DIR); \
		)

re: fclean space all
relib: libclean space all

space:
	@echo

force:
	@true

-include customrules.mk

ifeq ($(filter %clean relib re %.d,$(MAKECMDGOALS)),)
-include $(patsubst $O%.o,$D%.d,$(wildcard $(OBJ)))
endif
ifeq ($(filter %clean relib %.d,$(MAKECMDGOALS)),)
	ifneq ($(EXEC_TARGET),)
		-include $(patsubst %,%.d,$(wildcard $(LIB)))
	endif
endif
