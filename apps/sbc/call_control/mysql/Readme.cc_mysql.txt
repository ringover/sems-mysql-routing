MySQL Call Control module
=========================

---------------------
-- What is MySQL call control module ?
---------------------
This module allow to control all calls of SBC application with a MySQL database.
It offers the following functionnalities :
    * a routing calls using the routes table (calls refused if the destination number not exist).
    * display of current calls
    * log all calls and SMS (SIP Message)

---------------------
-- Dependencies : 
---------------------
  * libzdb (https://github.com/haiwen/libzdb.git) :
      - mysql-server
      - libmysqlclient-dev
      - libmysqlclient18
      - libtool
      - automake
      - flex
      - re2c

---------------------
-- How to install libzDB ?
---------------------
  ./bootstrap
  ./configure
  make && make install
  ldconfig

---------------------
-- How to create the database with the SBC user ?
---------------------
  mysql -u root -p < SEMS_PATH/apps/sbc/call_control/mysql/database.sql

SQL
===

The DB in the 'database.sql' file.

---------------------
-- Sample - insert into interco
---------------------

INSERT INTO `interco` (`id`, `number`, `dest`, `CREATION_DATE`) VALUES
(1, '123456789', 'sip:sipp@192.168.1.221:5060', '2014-03-25 13:42:51');

Configuration
=============

---------------------
-- In order to use the call control MySQL, you must configure the main configuration file 'cc_mysql.conf'
---------------------

###
# Connection MySQL
###
mysql_host=127.0.0.1
mysql_dbname=sbc
mysql_username=sbc
mysql_password=sbcsbc

###
# Enable(1) or disable(0) options : routing, current_call, logs
# By default, they are enabled
###
mysql_routing=0
mysql_current_calls=1
mysql_logs=1



---------------------
-- Finally, you must configure the 'sbcprofile.conf' file.
---------------------

###
# To activate the call control MySQL
###
call_control=mysql
mysql_module=cc_mysql

###
# data to import in the DB
###
mysql_FromUser=$fU
mysql_ToUser=$tU
mysql_FromDomain=$fd
mysql_ToDomain=$td
#mysql_RURI="sip:$rui"
mysql_RURI=$ru

