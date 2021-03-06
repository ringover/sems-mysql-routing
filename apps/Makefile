COREPATH ?= ../core
.DEFAULT_GOAL := all

local_exclude_app_modules=$(exclude_app_modules)

include $(COREPATH)/../Makefile.defs


# modules...

# if app_modules is set 
#   make $(app_modules)
# else 
#   if exclude_app_modules is set
#     make * - $(exclude_app_modules)
#   else
#     make * - Makefile.defs:exclude_app_modules +  local excludes
#   

ifneq ($(app_modules),)
 app_modules = $(app_modules)
else
 ifneq ($(local_exclude_app_modules),)
   exclude_app_modules = $(local_exclude_app_modules)
 else
   exclude_app_modules += mp3 examples py_sems xmlrpc2di gateway jsonrpc twit db_reg_agent ann_b2b announcement announce_transfer annrecorder callback click2dial conf_auth conference diameter_client dsm early_announce ivr mailbox msg_storage mwi pin_collect precoded_announce py_sems reg_agent registrar_client rtmp voicebox voicemail webconference xmlrpc2di

   ifneq ($(USE_MONITORING), yes)
   exclude_app_modules += monitoring
   endif

 endif

 $(info local_exclude_app_modules: $(local_exclude_app_modules))
 $(info exclude_app_modules: $(exclude_app_modules))
 app_modules = $(filter-out $(subst ;, ,$(exclude_app_modules))\
		$(wildcard Makefile*) CVS CMakeLists.txt, \
		$(wildcard *) examples/tutorial/cc_acc )

endif


$(info app_modules: $(app_modules))

.PHONY: all
all: modules

.PHONY: clean
clean:
	-@rm -f *.so
	-@for r in $(app_modules) "" ; do \
		if [ -n "$$r" ]; then \
			echo "" ; \
			echo "Cleaning $$r" ; \
			echo "" ; \
			COREPATH=../$(COREPATH) $(MAKE) -C $$r clean ; \
		fi ; \
	done

.PHONY: modules
modules:
	-@for r in $(app_modules) "" ; do \
		if [ -n "$$r" ]; then \
			echo  "" ; \
			echo "Making $$r" ; \
			echo  "" ; \
			COREPATH=../$(COREPATH) $(MAKE) -C $$r all; \
		fi ; \
	done 

.PHONY: install
install: install-bin install-cfg

.PHONY: install-bin
install-bin:
	-@for r in $(app_modules) "" ; do \
		if [ -n "$$r" ]; then \
			echo "" ; \
			echo "Installing $$r binaries" ; \
			echo "" ; \
			COREPATH=../$(COREPATH) $(MAKE) -C $$r install; \
		fi ; \
	done

.PHONY: install-cfg
install-cfg: $(DESTDIR)$(cfg-target)
	-@for r in $(app_modules) "" ; do \
		if [ -n "$$r" ]; then \
			echo "" ; \
			echo "Installing $$r configuration" ; \
			echo "" ; \
			COREPATH=../$(COREPATH) $(MAKE) -C $$r install-cfg; \
		fi ; \
	done
