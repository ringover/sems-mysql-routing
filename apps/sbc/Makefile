plug_in_name = sbc

module_ldflags = -g
module_cflags  = 

extra_target = make_call_control_mods
extra_install = install_tools install_call_control_mods
extra_clean = clean_call_control_mods

COREPATH ?= ../../core
include $(COREPATH)/plug-in/Makefile.app_module

install_tools:
	-@$(MAKE) -C tools/ install

make_call_control_mods:
	-@$(MAKE) -C call_control

install_call_control_mods:
	-@$(MAKE) -C call_control/ install

clean_call_control_mods:
	-@$(MAKE) -C call_control/ clean

install-cfg:
	if [ ! -f $(DESTDIR)$(app-cfg-target)sbc.conf ]; then \
		$(INSTALL-CFG) etc/sbc.conf $(DESTDIR)$(app-cfg-target)/sbc.conf; \
	fi
	if [ ! -f $(DESTDIR)$(app-cfg-target)standard.sbcprofile.conf ]; then \
		$(INSTALL-CFG) etc/standard.sbcprofile.conf $(DESTDIR)$(app-cfg-target)/standard.sbcprofile.conf; \
	fi
	if [ ! -f $(DESTDIR)$(app-cfg-target)refuse.sbcprofile.conf ]; then \
		$(INSTALL-CFG) etc/refuse.sbcprofile.conf $(DESTDIR)$(app-cfg-target)/refuse.sbcprofile.conf; \
	fi
	if [ ! -f $(DESTDIR)$(app-cfg-target)cc_mysql.conf ]; then \
		$(INSTALL-CFG) call_control/mysql/etc/cc_mysql.conf $(DESTDIR)$(app-cfg-target)/cc_mysql.conf; \
	fi
	if [ ! -f $(DESTDIR)$(app-cfg-target)mysql.sbcprofile.conf ]; then \
		$(INSTALL-CFG) call_control/mysql/etc/mysql.sbcprofile.conf $(DESTDIR)$(app-cfg-target)/mysql.sbcprofile.conf; \
	fi
