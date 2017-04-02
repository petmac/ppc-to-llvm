TESTS_32 := empty
TESTS_64 := $(TESTS_32)
TESTS := $(foreach test,$(TESTS_32),$(test)-32) $(foreach test,$(TESTS_64),$(test)-64)
BIN_FILES := $(foreach test,$(TESTS),$(test).bin)
TEST_PERMS := $(foreach test,$(TESTS),$(test)-32 $(test)-64)
DLL_FILES := $(foreach test,$(TEST_PERMS),$(test).dylib)
LL_FILES := $(foreach test,$(TEST_PERMS),$(test).ll)
TRANSLATE := $(BUILT_PRODUCTS_DIR)/translate

all: $(BIN_FILES) $(DLL_FILES) $(LL_FILES)

clean:
	rm -f $(BIN_FILES) $(DLL_FILES) $(LL_FILES)

%-32.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc32 -filetype obj $< -o $@

%-64.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@

%-32-32.ll: %-32.bin $(TRANSLATE)
	$(TRANSLATE) 32 32 $< $@

%-32-64.ll: %-32.bin $(TRANSLATE)
	$(TRANSLATE) 32 64 $< $@

%-64-32.ll: %-64.bin $(TRANSLATE)
	$(TRANSLATE) 64 32 $< $@

%-64-64.ll: %-64.bin $(TRANSLATE)
	$(TRANSLATE) 64 64 $< $@

%.dylib: %.ll
	clang -dynamiclib -Wno-override-module -o $@ $<
