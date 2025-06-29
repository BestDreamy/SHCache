TOP_NAME = SHCache

VERILATOR_INC_PATH  = $(addprefix -I, $(abspath ./vsrc/chi) \
									  $(abspath ./vsrc/include) \
									  $(abspath ./vsrc/flow) \
									  $(abspath ./vsrc/cbb) \
									  $(abspath ./vsrc/hnf) \
									  $(abspath ./vsrc/pipe) )
VERILATOR_FLAGS = -cc --exe --build --trace-fst --top-module $(TOP_NAME) $(VERILATOR_INC_PATH)
VERILATOR_FLAGS += -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC # TODO

CSRCS = $(shell find $(abspath .) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
VSRCS = $(shell find $(abspath ./vsrc) -name "$(TOP_NAME).*v")

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOP_NAME)

DEBUG ?= Y
CXXFLAGS = -MMD -MP -D${DEBUG}

# Just for testing
TEST_DIR = $(abspath ./benchmark)
test ?= all-reduce
TEST-SRC = $(TEST_DIR)/$(test).txt

default: run

$(BIN): $(CSRCS) $(VSRCS) | $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)
	verilator $(VERILATOR_FLAGS) $(CXXFLAGS) $^ \
	--Mdir $(OBJ_DIR) -o $(abspath $(BIN))

run: $(BIN) $(TEST-SRC)
	@$(BIN) $(TEST-SRC)

gtk: run
	gtkwave wave.fst

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR) wave.fst*

commit ?= update
git:
	git add .
	git commit -m "$(commit)"
	git push

config:
	python config/psrc/config_sys.py
	python config/psrc/gen_flit.py

.PHONY: default all clean run config git