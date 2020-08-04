################################################################################
#
# haveged
#
################################################################################

HAVEGED_VERSION = 1.9.9
HAVEGED_SITE = $(call github,jirka-h,haveged,v$(HAVEGED_VERSION))
HAVEGED_LICENSE = GPL-3.0+
HAVEGED_LICENSE_FILES = COPYING

ifeq ($(BR2_sparc_v8)$(BR2_sparc_leon3),y)
HAVEGED_CONF_OPTS += --enable-clock_gettime=yes
endif

define HAVEGED_INSTALL_INIT_SYSV
	$(INSTALL) -m 755 -D package/haveged/S21haveged \
		$(TARGET_DIR)/etc/init.d/S21haveged
endef

define HAVEGED_INSTALL_INIT_SYSTEMD
	$(INSTALL) -D -m 644 package/haveged/haveged.service \
		$(TARGET_DIR)/usr/lib/systemd/system/haveged.service
endef

$(eval $(autotools-package))
