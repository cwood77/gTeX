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
	$(OUT_DIR)/debug/front.dll \
	$(OUT_DIR)/debug/middle.dll \
	$(OUT_DIR)/debug/textTarget.dll \

all: \
	debug \
	$(OUT_DIR)/release/gTeX.exe \
	$(OUT_DIR)/release/front.dll \
	$(OUT_DIR)/release/middle.dll \
	$(OUT_DIR)/release/textTarget.dll \

include prattle/import.mak

clean:
	rm -rf bin
	rm -rf src/prattle

dirs: $(PRATTLE_IMPORTS)
	@mkdir -p $(OBJ_DIR)/debug/cmn
	@mkdir -p $(OBJ_DIR)/debug/front
	@mkdir -p $(OBJ_DIR)/debug/gTeX
	@mkdir -p $(OBJ_DIR)/debug/gTeX/pass
	@mkdir -p $(OBJ_DIR)/debug/middle
	@mkdir -p $(OBJ_DIR)/debug/prattle
	@mkdir -p $(OBJ_DIR)/debug/textTarget
	@mkdir -p $(OBJ_DIR)/release/cmn
	@mkdir -p $(OBJ_DIR)/release/front
	@mkdir -p $(OBJ_DIR)/release/gTeX
	@mkdir -p $(OBJ_DIR)/release/gTeX/pass
	@mkdir -p $(OBJ_DIR)/release/middle
	@mkdir -p $(OBJ_DIR)/release/prattle
	@mkdir -p $(OBJ_DIR)/release/textTarget
	@mkdir -p $(OUT_DIR)/debug
	@mkdir -p $(OUT_DIR)/release

.PHONY: debug all clean dirs test

# ----------------------------------------------------------------------
# cmn

CMN_SRC = \
	$(PRATTLE_SRC) \
	src/cmn/node.cpp \

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
	src/gTeX/defaultTargetPass.cpp \
	src/gTeX/main.cpp \

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

# ----------------------------------------------------------------------
# front

FRONT_SRC = \
	src/front/frontTarget.cpp \
	src/front/lexor.cpp \
	src/front/module.cpp \
	src/front/parsePass.cpp \
	src/front/parser.cpp \

FRONT_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(FRONT_SRC)))

$(OUT_DIR)/debug/front.dll: $(FRONT_DEBUG_OBJ) $(OUT_DIR)/debug/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -shared -o $@ $(FRONT_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -lcmn

$(FRONT_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

FRONT_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(FRONT_SRC)))

$(OUT_DIR)/release/front.dll: $(FRONT_RELEASE_OBJ) $(OUT_DIR)/release/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -shared -o $@ $(FRONT_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -lcmn

$(FRONT_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# middle

MIDDLE_SRC = \
	src/middle/entityRemover.cpp \
	src/middle/middleTarget.cpp \
	src/middle/module.cpp \

MIDDLE_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(MIDDLE_SRC)))

$(OUT_DIR)/debug/middle.dll: $(MIDDLE_DEBUG_OBJ) $(OUT_DIR)/debug/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -shared -o $@ $(MIDDLE_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -lcmn

$(MIDDLE_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

MIDDLE_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(MIDDLE_SRC)))

$(OUT_DIR)/release/middle.dll: $(MIDDLE_RELEASE_OBJ) $(OUT_DIR)/release/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -shared -o $@ $(MIDDLE_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -lcmn

$(MIDDLE_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@

# ----------------------------------------------------------------------
# textTarget

TEXTTARGET_SRC = \
	src/textTarget/module.cpp \
	src/textTarget/textPrintPass.cpp \
	src/textTarget/textTarget.cpp \

TEXTTARGET_DEBUG_OBJ = $(subst src,$(OBJ_DIR)/debug,$(patsubst %.cpp,%.o,$(TEXTTARGET_SRC)))

$(OUT_DIR)/debug/textTarget.dll: $(TEXTTARGET_DEBUG_OBJ) $(OUT_DIR)/debug/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -shared -o $@ $(TEXTTARGET_DEBUG_OBJ) $(DEBUG_LNK_FLAGS_POST) -Lbin/out/debug -lcmn

$(TEXTTARGET_DEBUG_OBJ): $(OBJ_DIR)/debug/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(DEBUG_CC_FLAGS) $< -o $@

TEXTTARGET_RELEASE_OBJ = $(subst src,$(OBJ_DIR)/release,$(patsubst %.cpp,%.o,$(TEXTTARGET_SRC)))

$(OUT_DIR)/release/textTarget.dll: $(TEXTTARGET_RELEASE_OBJ) $(OUT_DIR)/release/cmn.lib
	$(info $< --> $@)
	@$(LINK_CMD) -shared -o $@ $(TEXTTARGET_RELEASE_OBJ) $(RELEASE_LNK_FLAGS_POST) -Lbin/out/release -lcmn

$(TEXTTARGET_RELEASE_OBJ): $(OBJ_DIR)/release/%.o: src/%.cpp
	$(info $< --> $@)
	@$(COMPILE_CMD) $(RELEASE_CC_FLAGS) $< -o $@
