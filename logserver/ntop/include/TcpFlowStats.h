/*
 *
 * (C) 2013-17 - ntop.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _TCP_FLOW_STATS_H_
#define _TCP_FLOW_STATS_H_

#include "ntop_includes.h"
/*TcpFlowStats.cpp这部分主要对TCP相关内容的统计,应用于后台展示-网卡的lua展示部分（ntop_interface->lua）*/
class TcpFlowStats {
private:
    u_int32_t numSynFlows, numEstablishedFlows, numResetFlows, numFinFlows;

public:
    TcpFlowStats();

    inline void incSyn()         { numSynFlows++;    }
    inline void incEstablished() { numEstablishedFlows++; }
    inline void incReset()       { numResetFlows++;       }
    inline void incFin()         { numFinFlows++;         }

    /*~~~~~~~~~~~byzpl~~~~~~~~~~~~~*/
    inline u_int32_t getNumSynFlows() { return(numSynFlows); }
    inline u_int32_t getNumEstablishedFlows() { return(numEstablishedFlows); }
    inline u_int32_t getNumResetFlows() { return(numResetFlows); }
    inline u_int32_t getNumFinFlows() { return(numFinFlows); }

    /*~~~~~~~~~~~~~~~~~~~~~~~~*/

    char* serialize();
    void deserialize(json_object *o);
    json_object* getJSONObject();
//    void lua(lua_State* vm, const char *label);
    inline void sum(TcpFlowStats *s) {
        s->numSynFlows += numSynFlows, s->numEstablishedFlows += numEstablishedFlows,
                s->numResetFlows += numResetFlows, s->numFinFlows += numFinFlows;
    };
};

#endif /* _TCP_FLOW_STATS_H_ */
