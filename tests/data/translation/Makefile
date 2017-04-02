TESTS_32 := blr
TESTS_64 := $(TESTS_32)
TESTS := $(foreach test,$(TESTS_32),$(test)-32) $(foreach test,$(TESTS_64),$(test)-64)
BIN_FILES := $(foreach test,$(TESTS),$(test).bin)

all: $(BIN_FILES)

clean:
	rm -f $(BIN_FILES)

%-32.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc32 -filetype obj $< -o $@

%-64.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@
