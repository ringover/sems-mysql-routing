plug_in_name = cc_mysql
sbc_app_path = ../..

module_ldflags = -lmysqlclient -lzdb -I/usr/local/include/zdb
module_cflags  = -DMOD_NAME=\"$(plug_in_name)\"  -I$(sbc_app_path) -L/vagrant/lib/libzdb-3.0/lib -lzdb -I/usr/local/include/zdb

COREPATH =../../../../core
include $(COREPATH)/plug-in/Makefile.app_module

