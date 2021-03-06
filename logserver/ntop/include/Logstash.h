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

#ifndef _LOGSTASH_H_
#define _LOGSTASH_H_

#include "ntop_includes.h"

class Logstash {
private:
    pthread_t lsThreadLoop;
    u_int num_queued_elems;
    struct string_list *head, *tail;
    Mutex listMutex;
    bool reportDrops;
    struct timeval lastUpdateTime;
    u_int32_t elkDroppedFlowsQueueTooLong;
    u_int64_t elkExportedFlows, elkLastExportedFlows;
    float elkExportRate;
    u_int64_t checkpointDroppedFlows,
    checkpointExportedFlows; /* Those will hold counters at checkpoints */

public:
    Logstash();
    virtual ~Logstash();
    void checkPointCounters(bool drops_only) {
        if(!drops_only)
            checkpointExportedFlows = elkExportedFlows;
        checkpointDroppedFlows = elkDroppedFlowsQueueTooLong;
    };
    inline u_int32_t numDroppedFlows() const { return elkDroppedFlowsQueueTooLong; };
    /*~~~~~~~~byzpl~~~~~~~~~~*/
    inline u_int64_t  getElkExportedFlows()  {return elkExportedFlows;}
    //  inline u_int32_t  getElkDroppedFlowsQueueTooLong()  {return elkDroppedFlowsQueueTooLong;}
    inline float  getElkExportRate()  {return elkExportRate;}
    inline u_int64_t  getCheckpointExportedFlows()  {return checkpointExportedFlows;}
    inline u_int64_t  getCheckpointDroppedFlows()  {return checkpointDroppedFlows;}
    /*~~~~~~~~~~~~~~~~~~*/
    int sendToLS(char* msg);
    void sendLSdata();
    void startFlowDump();

    void updateStats(const struct timeval *tv);
//    void lua(lua_State* vm, bool since_last_checkpoint) const;
};


#endif /* _LOGSTASH_H_ */
