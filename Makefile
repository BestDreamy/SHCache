CXX = g++
CXXFLAGS = -std=c++17
INCLUDES_DIR = $(addprefix -I, $(abspath ./csrc) \
							   $(abspath ./csrc/include) \
							   $(abspath ./csrc/chi) \
							   $(abspath ./csrc/cpu) \
							   $(abspath ./csrc/diff))

CSRCS = $(shell find $(abspath .) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
BIN := sim

TEST_DIR = $(abspath ./benchmark)
test ?= all-reduce
TEST_SRC = $(TEST_DIR)/$(test).txt

default: run

$(BIN): $(CSRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES_DIR) -o $@ $^

run: $(BIN) $(TEST_SRC)
	@$(BIN) $(TEST_SRC)

clean:
	rm -rf $(BIN)

commit ?= update
git:
	git add .
	git commit -m "$(commit)"
	git push -u origin dev

config:
	python config/psrc/config_sys.py
	python config/psrc/gen_flit.py

.PHONY: default all clean run config git