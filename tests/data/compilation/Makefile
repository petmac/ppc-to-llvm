TESTS := empty simple
O_FILES := $(foreach test,$(TESTS),$(test).o)
TRANSLATE := $(BUILT_PRODUCTS_DIR)/translate

.SECONDARY: $(foreach test,$(TESTS),$(test).ll)

all: $(O_FILES)

clean:
	rm -f $(O_FILES)

%-ppc.o: %.s
	/usr/local/opt/llvm/bin/llvm-mc -arch ppc64 -filetype obj $< -o $@

%.bin: %-ppc.o
	gobjcopy --strip-all --output-target binary $< $@

%.ll: %.bin $(TRANSLATE)
	$(TRANSLATE) $< $@

%.o: %.ll
	clang -c -g -o $@ $<
