/*
 * $Id: msg_hdrs.cpp 1713 2010-03-30 14:11:14Z rco $
 *
 * Copyright (C) 2007 Raphael Coeffic
 *
 * This file is part of SEMS, a free SIP media server.
 *
 * SEMS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. This program is released under
 * the GPL with the additional exemption that compiling, linking,
 * and/or using OpenSSL is allowed.
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


#include "msg_hdrs.h"


int copy_hdrs_len(const list<sip_header*>& hdrs)
{
    int ret = 0;

    list<sip_header*>::const_iterator it = hdrs.begin();
    for(;it != hdrs.end(); ++it){
	ret += copy_hdr_len(*it);
    }
    
    return ret;
}

void copy_hdrs_wr(char** c,
		  const list<sip_header*>& hdrs)
{
    list<sip_header*>::const_iterator it = hdrs.begin();
    for(;it != hdrs.end(); ++it){
	
	memcpy(*c,(*it)->name.s,(*it)->name.len);
	*c += (*it)->name.len;

	*((*c)++) = ':';
	*((*c)++) = SP;

	memcpy(*c,(*it)->value.s,(*it)->value.len);
	*c += (*it)->value.len;

	*((*c)++) = CR;
	*((*c)++) = LF;
    }
}

