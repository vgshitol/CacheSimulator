SUBDIR := code

.PHONY: all clean clobber run help

all:
	$(MAKE) -C $(SUBDIR) all

clean:
	$(MAKE) -C $(SUBDIR) clean

clobber:
	$(MAKE) -C $(SUBDIR) clobber

run:
	$(MAKE) -C $(SUBDIR) run

help:
	$(MAKE) -C $(SUBDIR) help
