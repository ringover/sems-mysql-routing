# SEMS-MySQL-Routing

## Introduction
          
Forked from SIP Express Media Server (SEMS).

We have made a lot of tests using SEMS v 1.5, to build a robust call routing engine SBC. We needed a call routing table to send call to the right service/server.

 * After many debug and fixes, we now have a GigaSBC (1000 call simultaneous).

 * We also implemented SMS (SIP Message) support in SBC Module.

 * We have made some tests with Rest SBC Module but Curl implementation isn’t stable (not more than 150 calls simultaneous).

 * Finally, we made a MySQL SBC module:


      cc_mysql (usefull for routing call and/or SMS (SIP Message), current calls and CDR).

### To do

 * SMS (SIP Message) reply is not implemented. For the moment, I answer a message SIP/200 OK every time.

## Install

### Requirements

Install these dependencies :

    sudo apt-get install debhelper devscripts
        
    sudo apt-get install g++ make libspandsp-dev flite-dev libspeex-dev        \
       libssl-dev python-dev openssl libev-dev mysql-server libmysqlclient-dev \
       libmysqlclient18 libtool automake flex re2c
        
And ZDB library :

    git clone https://github.com/haiwen/libzdb.git libzdb
    cd libzdb
    ./bootstrap
    ./configure
    make && make install

### Installed files using 'make install'

    /usr/local/sems/bin                     : SEMS executable
    /usr/local/sems/lib/plug-in/*           : plug-ins
    /usr/local/sems/lib/audio/*             : default path for audio files
    /usr/local/sems/conf/sems.conf          : configuration file
    /usr/local/sems/conf/etc/*              : modules configuration files

### MySQL Schema

#### List of tables :

    routes(id,number,dest,creation_date)
      |-> used for the routing calls 
      eg: INSERT INTO `routes` (`number`,`dest`) VALUES ('0123456789','sip:0123456789@100.100.100.100:5060');
      
    current_calls(id,caller,callee,server,start_time,uuid,creation_date) : 
      |-> used for display of current calls 
      eg: INSERT INTO `current_calls` (`caller`,`callee`,`server`,`start_time`,) VALUES ('0123456789','0987654321','80.80.80.80:5090','2014-06-24 14:57:27')
      
    logs_calls(id,caller,callee,server,duration,start_time,end_time,creation_date)
      |-> save the calls log 
      eg: INSERT INTO `current_calls` (`caller`,`callee`,`server`,`duration`,`start_time`,`end_time`) VALUES ('0123456789','0987654321','80.80.80.80:5090','32','2014-06-24 14:57:27','2014-06-24 14:57:59');
      
    logs_sms(id,caller,callee,server,creation_date)
      |-> save the sms log
      eg: INSERT INTO `current_calls` (`caller`,`callee`,`server`) VALUES ('0123456789','0987654321','80.80.80.80:5090');

eg: We now have this routing type with the previous examples :
If 0123456789 is registered in routes table, call scheme from 0987654321 to 0123456789.

    0987654321              Server              0123456789
        |                    | |                    |  
        |       call         | |        call        |                
        |------------------->| |------------------->|
        |                    | |                    |
    80.80.80.80:5090                          100.100.100.100:5060


#### SQL file :

    <sems_repo>/apps/sbc/call_control/mysql/database.sql

This file allow to create a database and a local user ('sbc' with password 'sbcsbc') having INSERT/SELECT/UPDATE/DELETE privileges.

## Start SEMS

### Run SEMS

    /usr/local/sems/bin/sems -f /usr/local/sems/conf/sems.conf

### Run SEMS with Debug

    /usr/local/sems/bin/sems -f /usr/local/sems/conf/sems.conf -E -D 3"


