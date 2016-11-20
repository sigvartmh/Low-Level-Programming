# -*-makefile-*-
#
# Copyright (C) 2016 by <>Sigvart M. Hovland
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_BUTTONDRIVER) += buttondriver

#
# Paths and names and versions
#
BUTTONDRIVER_VERSION	:= 0.0.1
BUTTONDRIVER		:= buttondriver-$(BUTTONDRIVER_VERSION)
BUTTONDRIVER_URL		:= lndir://$(PTXDIST_WORKSPACE)/local_src/$(BUTTONDRIVER)
BUTTONDRIVER_DIR		:= $(BUILDDIR)/$(BUTTONDRIVER)
BUTTONDRIVER_LICENSE	:= unknown

ifdef PTXCONF_BUTTONDRIVER
$(STATEDIR)/kernel.targetinstall.post: $(STATEDIR)/buttondriver.targetinstall
endif

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/buttondriver.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/buttondriver.compile:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(BUTTONDRIVER_DIR) \
		modules
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/buttondriver.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/buttondriver.targetinstall:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(BUTTONDRIVER_DIR) \
		modules_install
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/buttondriver.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, BUTTONDRIVER)

# vim: syntax=make
