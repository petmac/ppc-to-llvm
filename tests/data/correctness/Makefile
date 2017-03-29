TESTS := empty
BIN_FILES := $(foreach test,$(TESTS),$(test).bin)
DLL_FILES := $(foreach test,$(TESTS),$(test).dylib)
LL_FILES := $(foreach test,$(TESTS),$(test).ll)
TRANSLATE := $(BUILT_PRODUCTS_DIR)/translate

all: $(BIN_FILES) $(DLL_FILES) $(LL_FILES)

clean:
	rm -f $(BIN_FILES) $(DLL_FILES) $(LL_FILES)

%.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@

%.ll: %.bin $(TRANSLATE)
	$(TRANSLATE) $< $@

%.dylib: %.ll
	clang -dynamiclib -Wno-override-module -o $@ $<
