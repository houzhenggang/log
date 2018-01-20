#include"ntop_includes.h"

typedef enum _loglevel{
    LOG_TRACE_LEVEL_ERROR = 0,
    LOG_TRACE_LEVEL_WARNING,//1
    LOG_TRACE_LEVEL_NORMAL,//2
    LOG_TRACE_LEVEL_INFO,//3
    LOG_TRACE_LEVEL_INFO1,//test no exist
    LOG_TRACE_LEVEL_INFO2,//test   no exist
    LOG_TRACE_LEVEL_DEBUG ,//6
    LOG_TRACE_LEVEL_DEBUG1,//test no exist
    LOG_TRACE_LEVEL_DEBUG2,//test  no exist
    LOG_TRACE_LEVEL_TRACE//9
}LogLevel;



/*此接口对应后台lua   设置/Runtime Preferences/loging*/
void  set_logging_level(u_int8_t param) {
    ntop->getTrace()->traceEvent(TRACE_INFO, "%s() called", __FUNCTION__);
    switch (param) {
    case LOG_TRACE_LEVEL_ERROR:
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_ERROR);
        break;
    case LOG_TRACE_LEVEL_WARNING:
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_WARNING);
        break;
    case LOG_TRACE_LEVEL_NORMAL:
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_NORMAL);
        break;
    case LOG_TRACE_LEVEL_INFO:
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_INFO);
        break;
    case LOG_TRACE_LEVEL_DEBUG:
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_DEBUG);
        break;
    case LOG_TRACE_LEVEL_TRACE:
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_TRACE);
        break;
    default :
        ntop->getTrace()->set_trace_level(LOG_TRACE_LEVEL_ERROR);
        break;
    }
}

