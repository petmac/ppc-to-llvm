TESTS := blr
BIN_FILES := $(foreach test,$(TESTS),$(test).bin)

all: $(BIN_FILES)

clean:
	rm -f $(BIN_FILES)

%.o: %.s
	/usr/local/opt/llvm/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@
