COMPILE_CMD = x86_64-w64-mingw32-g++
LINK_CMD = x86_64-w64-mingw32-g++
OBJ_DIR = bin/obj
OUT_DIR = bin/out
DEBUG_CC_FLAGS = -ggdb -c -Wall -D cdwDebugMode -D cdwTestBuild
RELEASE_CC_FLAGS = -O3 -c -Wall
DEBUG_LNK_FLAGS_POST = -ggdb -static-libgcc -static-libstdc++ -static
RELEASE_LNK_FLAGS_POST = -static-libgcc -static-libstdc++ -static

test: debug
	@bin/out/debug/gTeX.exe
	@cmd.exe /c "fc testdata.txt-out testdata.txt-out-expected"

debug: \
	dirs \
	$(OUT_DIR)/debug/gTeX.exe \

all: \
	debug \
	$(OUT_DIR)/release/gTeX.exe \

include prattle/import.mak

clean:
	rm -rf bin
	rm -rf src/prattle

dirs: $(PRATTLE_IMPORTS)
	@mkdir -p $(OBJ_DIR)/debug/cmn
	@mkdir -p $(OBJ_DIR)/debug/gTeX
	@mkdir -p $(OBJ_DIR)/debug/gTeX/pass
	@mkdir -p $(OBJ_DIR)/debug/prattle
	@mkdir -p $(OBJ_DIR)/release/cmn
	@mkdir -p $(OBJ_DIR)/release/gTeX
	@mkdir -p $(OBJ_DIR)/release/gTeX/pass
	@mkdir -p $(OBJ_DIR)/release/prattle
	@mkdir -p $(OUT_DIR)/debug
	@mkdir -p $(OUT_DIR)/release

.PHONY: debug all clean dirs test

# ----------------------------------------------------------------------
# cmn

CMN_SRC = \
	$(PRATTLE_SRC)

CMN_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(CMN_SRC)))

$(OUT_DIR)/debug/cmn.lib: $(PRATTLE_IMPORTS) $(CMN_DEBUG_OBJ)
	$(info $< --> $@)
	@ar crs $@ $^

$(CMN_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

CMN_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(CMN_SRC)))

$(OUT_DIR)/release/cmn.lib: $(PRATTLE_IMPORTS) $(CMN_RELEASE_OBJ)
	$(info $< --> $@)
	@ar crs $@ $^

$(CMN_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# gTeX

GTEX_SRC = \
	src/gTeX/lexor.cpp \
	src/gTeX/main.cpp \
	src/gTeX/middleTarget.cpp \
	src/gTeX/node.cpp \
	src/gTeX/parsePass.cpp \
	src/gTeX/parser.cpp \
	src/gTeX/pass/entityRemover.cpp \
	src/gTeX/textPrintPass.cpp \
	src/gTeX/textTarget.cpp \

GTEX_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(GTEX_SRC)))

$(OUT_DIR)/debug/gTeX.exe: $(GTEX_DEBUG_OBJ) $(OUT_DIR)/debug/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -o $@ $(GTEX_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -lcmn

$(GTEX_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

GTEX_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(GTEX_SRC)))

$(OUT_DIR)/release/gTeX.exe: $(GTEX_RELEASE_OBJ) $(OUT_DIR)/release/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -o $@ $(GTEX_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -lcmn

$(GTEX_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
