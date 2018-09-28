# Makefile for mx23 hw_objs

LIBNAME		?= hw

# Default goal
.PHONY: all
all: lib$(LIBNAME)



lib_objs = power/libhw_power.a lradc/libhw_lradc.a digctl/libhw_digctl.a \
	icoll/libhw_icoll.a core/libhw_core.a

#
# Make targets
#
.PHONY: build build_prep clean

lib$(LIBNAME):
	$(MAKE) -C power
	$(MAKE) -C lradc
	$(MAKE) -C icoll
	$(MAKE) -C digctl
	$(MAKE) -C core
	ar rs lib$(LIBNAME).a $(lib_objs)
#	ranlib lib$(LIBNAME).a

clean:
	@echo Cleaning...
	$(MAKE) clean -C power
	$(MAKE) clean -C lradc
	$(MAKE) clean -C icoll
	$(MAKE) clean -C digctl
	$(MAKE) clean -C core
	@echo Files:
	rm -f lib$(LIBNAME).a


