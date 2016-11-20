# -*-makefile-*-
#
# Copyright (C) 2016 by Sigvart M. Hovland
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_FLYBY) += flyby

#
# Paths and names
#
FLYBY_VERSION	:= 0.0.1
FLYBY_MD5		:=
FLYBY		:= flyby
FLYBY_URL		:= file://local_src/$(FLYBY)
FLYBY_DIR		:= $(BUILDDIR)/$(FLYBY)
FLYBY_LICENSE	:= unknown

# ----------------------------------------------------------------------------
# Extract
# ----------------------------------------------------------------------------

#$(STATEDIR)/flyby.extract:
#	@$(call targetinfo)
#	@$(call clean, $(FLYBY_DIR))
#	@$(call extract, FLYBY)
#	@$(call patchin, FLYBY)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

#FLYBY_PATH	:= PATH=$(CROSS_PATH)
FLYBY_CONF_TOOL	:= NO
FLYBY_MAKE_ENV	:= $(CROSS_ENV)

#$(STATEDIR)/flyby.prepare:
#	@$(call targetinfo)
#	@$(call world/prepare, FLYBY)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

#$(STATEDIR)/flyby.compile:
#	@$(call targetinfo)
#	@$(call world/compile, FLYBY)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

#$(STATEDIR)/flyby.install:
#	@$(call targetinfo)
#	@$(call world/install, FLYBY)
#	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/flyby.targetinstall:
	@$(call targetinfo)

	@$(call install_init, flyby)
	@$(call install_fixup, flyby,PRIORITY,optional)
	@$(call install_fixup, flyby,SECTION,base)
	@$(call install_fixup, flyby,AUTHOR,"Sigvart M. Hovland")
	@$(call install_fixup, flyby,DESCRIPTION,missing)

#
# TODO: Add here all files that should be copied to the target
# Note: Add everything before(!) call to macro install_finish
#
	@$(call install_copy, flyby, 0, 0, 0755, $(FLYBY_DIR)/flyby, /usr/bin/flyby)

	@$(call install_finish, flyby)

	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

$(STATEDIR)/flyby.clean:
	@$(call targetinfo)
	@-cd $(FLYBY_DIR) && \
		$(FLYBY_ENV) $(FLYBY_PATH) $(MAKE) clean
	@$(call clean_pkg, FLYBY)

# vim: syntax=make
