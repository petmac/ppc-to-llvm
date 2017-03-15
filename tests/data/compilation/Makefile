TESTS := empty
ELF_FILES := $(foreach test,$(TESTS),$(test).elf)
TRANSLATE := $(BUILT_PRODUCTS_DIR)/translate

.SECONDARY: $(foreach test,$(TESTS),$(test).ll)

all: $(ELF_FILES)

clean:
	rm -f $(ELF_FILES)

%.o: %.s
	/usr/local/opt/llvm/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %.o
	gobjcopy --strip-all --output-target binary $< $@

%.ll: %.bin $(TRANSLATE)
	$(TRANSLATE) $< $@

%.elf: %.ll
	clang -o $@ $<
