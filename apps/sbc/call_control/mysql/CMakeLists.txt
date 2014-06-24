SET(sems_module_name cc_mysql)
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/config.rules.txt)

SEMS_INSTALL_CONFIGS("cc_mysql;mysql.sbcprofile.conf")
