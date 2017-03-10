KSBUILDDIR := keystone/build
KSTOOL := $(KSBUILDDIR)/kstool/kstool

all: $(KSTOOL)

clean:
	rm -rf $(KSBUILDDIR)

$(KSTOOL):
	mkdir -p $(KSBUILDDIR)
	cd $(KSBUILDDIR) && ../make-share.sh
