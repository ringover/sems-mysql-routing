FIND_PATH(MYSQLPP_INCLUDE_DIR mysql++/mysql++.h)
FIND_LIBRARY(MYSQLPP_LIBRARIES NAMES mysqlpp)

IF(MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARIES)
	SET(MYSQLPP_FOUND TRUE)
ENDIF(MYSQLPP_INCLUDE_DIR AND MYSQLPP_LIBRARIES)

IF(MYSQLPP_FOUND)
	IF (NOT MySQLpp_FIND_QUIETLY)
		MESSAGE(STATUS "Found mysql++ includes:	${MYSQLPP_INCLUDE_DIR}/mysql++/mysql++.h")
		MESSAGE(STATUS "Found mysql++ library: ${MYSQLPP_LIBRARIES}")
	ENDIF (NOT MySQLpp_FIND_QUIETLY)
ELSE(MYSQLPP_FOUND)
	IF (MySQLpp_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could NOT find mysql++ development files")
	ENDIF (MySQLpp_FIND_REQUIRED)
ENDIF(MYSQLPP_FOUND)

# Check for buried mysql_version.h
FIND_PATH(MYSQL_INCLUDE_MYSQL_VERSION_H mysql_version.h)
IF(NOT MYSQL_INCLUDE_MYSQL_VERSION_H)
	FIND_PATH(MYSQL_INCLUDE_BURIED_MYSQL_VERSION_H mysql/mysql_version.h)
	IF(MYSQL_INCLUDE_BURIED_MYSQL_VERSION_H)
		ADD_DEFINITIONS(-DMYSQLPP_MYSQL_HEADERS_BURIED)
	ENDIF(MYSQL_INCLUDE_BURIED_MYSQL_VERSION_H)
ENDIF(NOT MYSQL_INCLUDE_MYSQL_VERSION_H)
