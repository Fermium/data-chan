all:

%:
	@echo Executing \"make $@\" on all host-side data-chan elements.
	@echo
	$(MAKE) -C API $@
	$(MAKE) -C CPP_API $@
	$(MAKE) -C NODEJS_API $@
	$(MAKE) -C PYTHON_API $@
	@echo
	@echo host-side data-chan \"make $@\" operation complete.
