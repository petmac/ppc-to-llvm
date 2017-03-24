TESTS := empty
EXE_FILES := $(foreach test,$(TESTS),$(test).exe)
LL_FILES := $(foreach test,$(TESTS),$(test).ll)
TRANSLATE := $(BUILT_PRODUCTS_DIR)/translate

all: $(EXE_FILES) $(LL_FILES)

clean:
	rm -f $(EXE_FILES) $(LL_FILES)

%.o: %.s
	$(LLVM_DIR)/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@

%.ll: %.bin $(TRANSLATE)
	$(TRANSLATE) $< $@

%.exe: %.ll
	clang -Wno-override-module -o $@ $<
