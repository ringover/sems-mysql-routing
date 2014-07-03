/*
 * Copyright (C) 2011 Stefan Sayer
 *
 * This file is part of SEMS, a free SIP media server.
 *
 * SEMS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * For a license to use the SEMS software under conditions
 * other than those described here, or to purchase support for this
 * software, please contact iptel.org by e-mail at the following addresses:
 *    info@iptel.org
 *
 * SEMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _CC_MYSQL_H
#define _CC_MYSQL_H

#include <sstream>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <map>

#include <zdb/zdb.h>
#include <time.h>
#include <stdio.h>

#include "AmApi.h"
#include "log.h"
#include "pthread.h"
#include "SBCCallProfile.h"
#include "ampi/SBCCallControlAPI.h"

typedef struct _data_sip{
  string from;
  string fromUser;
  string fromDomain;
  string to;
  string toUser;
  string toDomain;
  string ruri;
  string ruriUser;
  string ruriDomain;
  string dest;
  string destUser;
  string destDomain;
  string methode;
  string uuid;
  int connect_ts_sec;
}data_sip;

class MysqlModule : public AmDynInvoke{
 protected:
  
  /*table enable or disable*/
  string tblRoutes;
  string tblCurrentCalls;
  string tblLogs;

  static MysqlModule* _instance;

  void start(const string& cc_name, const string& ltag, SBCCallProfile* call_profile,
	     int start_ts_sec, int start_ts_usec, const AmArg& values, const string& methode,
	     int timer_id, AmArg& res);
  void connect(const string& cc_name, const string& ltag, SBCCallProfile* call_profile,
	       const string& other_ltag, int connect_ts_sec, int connect_ts_usec);
  void end(const string& cc_name, const string& ltag, SBCCallProfile* call_profile,
	   int end_ts_sec, int end_ts_usec);

 public:
  MysqlModule();
  ~MysqlModule();
  static MysqlModule* instance();
  void invoke(const string& method, const AmArg& args, AmArg& ret);
  int onLoad();
};

////Query CREATE TABLES////

#define table_current_calls "CREATE TABLE IF NOT EXISTS `current_calls` (" \
    "  `id` int(11) NOT NULL AUTO_INCREMENT," \
    "  `user_from` varchar(64) NOT NULL," \
    "  `user_to` varchar(64) NOT NULL," \
    "  `server_from` varchar(64) NOT NULL," \
    "  `server_to` varchar(64) NOT NULL," \
    "  `start_time` timestamp NULL DEFAULT NULL," \
    "  `uuid` varchar(255) DEFAULT NULL," \
    "  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP," \
    "  PRIMARY KEY (`id`)" \
    ") ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;"

#define table_route "CREATE TABLE IF NOT EXISTS `routes` (" \
    "  `id` int(11) NOT NULL AUTO_INCREMENT," \
    "  `number` char(16) NOT NULL," \
    "  `dest` varchar(64) NOT NULL," \
    "  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP," \
    "  PRIMARY KEY (`id`)" \
    ") ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;"

#define table_logs_sms "CREATE TABLE IF NOT EXISTS `logs_sms` (" \
    "  `id` int(11) NOT NULL AUTO_INCREMENT," \
    "  `user_from` varchar(64) NOT NULL," \
    "  `user_to` varchar(64) NOT NULL," \
    "  `server_from` varchar(64) NOT NULL," \
    "  `server_to` varchar(64) NOT NULL," \
    "  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP," \
    "  PRIMARY KEY (`id`)" \
    ") ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;"

#define table_logs_calls "CREATE TABLE IF NOT EXISTS `logs_calls` (" \
    "  `id` int(11) NOT NULL AUTO_INCREMENT," \
    "  `user_from` varchar(64) NOT NULL," \
    "  `user_to` varchar(64) NOT NULL," \
    "  `server_from` varchar(64) NOT NULL," \
    "  `server_to` varchar(64) NOT NULL," \
    "  `duration` int(11) NOT NULL DEFAULT '0'," \
    "  `start_time` timestamp NULL DEFAULT NULL," \
    "  `end_time` timestamp NULL DEFAULT NULL," \
    "  `CREATION_DATE` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP," \
    "  PRIMARY KEY (`id`)" \
    ") ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;"

#endif 
