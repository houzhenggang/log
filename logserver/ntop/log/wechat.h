#ifndef WECHAT_H
#define WECHAT_H
#include "ntop_includes.h"

class Host;


struct wechat_stats {
    u_int32_t num;
};


class WECHATstats{
private:
    void deserializeStats(json_object *o, struct wechat_stats *stats);
       struct wechat_stats wechatID;
//    void luaStats(lua_State *vm, struct wechat_stats *stats, const char *label);
public:
    WECHATstats();
 //   ~WECHATstats();
    u_int32_t id ;
    char *serialize();
    void deserialize(json_object * o);
    json_object* getJSONObject();
//    void lua(lua_State *vm);
};
#endif // WECHAT_H
