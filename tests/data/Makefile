TESTS_32 := empty li
TESTS_64 := $(TESTS_32)
TESTS := $(foreach test,$(TESTS_32),$(test)-ppc32) $(foreach test,$(TESTS_64),$(test)-ppc64)
BIN_FILES := $(foreach test,$(TESTS),$(test).bin)
TEST_PERMS := $(foreach test,$(TESTS),$(test)-addr32 $(test)-addr64)
DLL_FILES := $(foreach test,$(TEST_PERMS),$(test).dylib)
LL_FILES := $(foreach test,$(TEST_PERMS),$(test).ll)
TRANSLATE := $(BUILT_PRODUCTS_DIR)/translate

all: $(BIN_FILES) $(DLL_FILES) $(LL_FILES)

clean:
	rm -f $(BIN_FILES) $(DLL_FILES) $(LL_FILES)

%-ppc32.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc32 -filetype obj $< -o $@

%-ppc64.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@

%-ppc32-addr32.ll: %-ppc32.bin $(TRANSLATE)
	$(TRANSLATE) 32 32 $< $@

%-ppc32-addr64.ll: %-ppc32.bin $(TRANSLATE)
	$(TRANSLATE) 32 64 $< $@

%-ppc64-addr32.ll: %-ppc64.bin $(TRANSLATE)
	$(TRANSLATE) 64 32 $< $@

%-ppc64-addr64.ll: %-ppc64.bin $(TRANSLATE)
	$(TRANSLATE) 64 64 $< $@

%.dylib: %.ll
	clang -O1 -dynamiclib -Wno-override-module -o $@ $<
