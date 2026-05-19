DESIGN ?= light
export NEMU_HOME ?= $(abspath .)
export NVBOARD_HOME ?= $(abspath ../nvboard)

ifeq ($(DESIGN),sw2)
TOPNAME = SW2_top
VSRCS = vsrc/SW2_top.v
SIM_CSRCS = csrc/SW2_main.cpp
NVBOARD_CSRCS = csrc/SW2_nvboard.cpp
NXDC_FILES = constr/SW2_top.nxdc
else ifeq ($(DESIGN),light)
TOPNAME = light
VSRCS = vsrc/light.v
SIM_CSRCS = csrc/light.cpp
NVBOARD_CSRCS = csrc/light_nvboard.cpp
NXDC_FILES = constr/light.nxdc
else ifeq ($(DESIGN),top)
TOPNAME = top
VSRCS = $(wildcard vsrc/sCPU/*.v)
SIM_CSRCS = csrc/sCPU.cpp
NVBOARD_CSRCS = csrc/sCPU_nvboard.cpp
NXDC_FILES = constr/sCPU.nxdc
else
$(error Unknown DESIGN=$(DESIGN), use DESIGN=sw2, DESIGN=light or DESIGN=top)
endif

VERILATOR = verilator

# Verilator + GTKWave/VCD simulation
SIM_BUILD_DIR = build/sim/$(DESIGN)
SIM_OBJ_DIR = $(SIM_BUILD_DIR)/obj_dir
SIM_BIN = $(SIM_BUILD_DIR)/V$(TOPNAME)
SIM_FLAGS = -Wall --cc --exe --build --trace-vcd --top-module $(TOPNAME)

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@mkdir -p $(SIM_BUILD_DIR)
	$(VERILATOR) $(SIM_FLAGS) $(VSRCS) $(SIM_CSRCS) --Mdir $(SIM_OBJ_DIR) -o $(abspath $(SIM_BIN))
	$(SIM_BIN)

sim-sw2:
	$(MAKE) sim DESIGN=sw2

sim-light:
	$(MAKE) sim DESIGN=light

sim-top:
	$(MAKE) sim DESIGN=top

wave:
	gtkwave wave.vcd

# NVBoard simulation
NVBOARD_BUILD_DIR_LOCAL = build/nvboard/$(DESIGN)
NVBOARD_OBJ_DIR = $(NVBOARD_BUILD_DIR_LOCAL)/obj_dir
NVBOARD_BIN = $(NVBOARD_BUILD_DIR_LOCAL)/$(TOPNAME)
SRC_AUTO_BIND = $(abspath $(NVBOARD_BUILD_DIR_LOCAL)/auto_bind.cpp)

$(SRC_AUTO_BIND): $(NXDC_FILES)
	@mkdir -p $(dir $@)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

INC_PATH ?=
ifneq ($(filter run run-sw2 run-light build/nvboard/%,$(MAKECMDGOALS)),)
include $(NVBOARD_HOME)/scripts/nvboard.mk
endif

INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS)
NVBOARD_VERILATOR_FLAGS = -MMD --build -cc -O3 --x-assign fast --x-initial fast --noassert

$(NVBOARD_BIN): $(VSRCS) $(NVBOARD_CSRCS) $(SRC_AUTO_BIND) $(NVBOARD_ARCHIVE)
	@rm -rf $(NVBOARD_OBJ_DIR)
	$(VERILATOR) $(NVBOARD_VERILATOR_FLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) \
		$(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(NVBOARD_OBJ_DIR) --exe -o $(abspath $(NVBOARD_BIN))

run:
	$(call git_commit, "run nvboard $(DESIGN)") # DO NOT REMOVE THIS LINE!!!
	$(MAKE) $(NVBOARD_BIN) DESIGN=$(DESIGN)
	$(NVBOARD_BIN)

run-sw2:
	$(MAKE) run DESIGN=sw2

run-light:
	$(MAKE) run DESIGN=light

run-top:
	$(MAKE) run DESIGN=top

clean:
	rm -rf build obj_dir wave.vcd

include ../Makefile

.PHONY: sim sim-sw2 sim-light sim-top wave run run-sw2 run-light run-top clean



# previous:

# TOPNAME = top
# VSRCS = vsrc/SW2_top.v
# CSRCS = csrc/SW2_main.cpp

# VERILATOR = verilator
# VERILATOR_FLAGS = -Wall --cc --exe --build --trace-vcd

# all:
# 	@echo "Write this Makefile by your self."

# sim:
# 	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
# 	@echo "Write this Makefile by your self."
# 	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS)
# 	./obj_dir/V$(TOPNAME)

# clean:
# 	rm -rf obj_dir wave.vcd

# include ../Makefile
