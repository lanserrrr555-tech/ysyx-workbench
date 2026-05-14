TOPNAME = SW2_top
VSRCS = vsrc/SW2_top.v
CSRCS = csrc/SW2_main.cpp

VERILATOR = verilator
VERILATOR_FLAGS = -Wall --cc --exe --build --trace-vcd



all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	$(VERILATOR) $(VERILATOR_FLAGS) $(VSRCS) $(CSRCS)
	./obj_dir/V$(TOPNAME)

clean:
	rm -rf obj_dir wave.vcd

include ../Makefile
