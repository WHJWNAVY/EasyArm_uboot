# Makefile for mx23 ddi_objs

# Makefile for mx23 hw_objs

LIBNAME		?= ddi

# Default goal
.PHONY: all
all: lib$(LIBNAME)



lib_objs = power/libddi_power.a

#
# Make targets
#
.PHONY: build build_prep clean

lib$(LIBNAME):
	$(MAKE) -C power
	ar rs lib$(LIBNAME).a $(lib_objs)
#	ranlib lib$(LIBNAME).a

clean:
	@echo Cleaning...
	$(MAKE) clean -C power
	@echo Files:
	rm -f lib$(LIBNAME).a
