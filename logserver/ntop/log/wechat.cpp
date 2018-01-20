//#include "wechat.h"
#include"ntop_includes.h"



/* *************************************** */

//WECHATstats::WECHATstats() {
//    this->id=0;
//    memset(&wechatID,0,sizeof(struct wechat_stats));
//}

/* *************************************** */

//WECHATstats::~WECHATstats() {
//    // if(virtualHosts) delete(virtualHosts);
//}

/* *************************************** */


//json_object* WECHATstats::getJSONObject() {
//    json_object *my_object = json_object_new_object();
//    if(!my_object) return(NULL);
//    json_object_object_add(my_object, "id",   json_object_new_int64(id));
//    return(my_object);
//}
/* **************************************** */


//void WECHATstats::luaStats(lua_State *vm, struct wechat_stats *stats, const char *label) {
//    lua_newtable(vm);
//    lua_push_int_table_entry(vm, "id", stats->num);
//    lua_pushstring(vm, label);
//    lua_insert(vm, -2);
//    lua_settable(vm, -3);
//}


/* ******************************************* */

//void WECHATstats::deserializeStats(json_object *o, struct wechat_stats *stats) {
//    json_object *obj, *s;

//    memset(stats, 0, sizeof(struct wechat_stats));

//    if(json_object_object_get_ex(o, "id", &obj)) stats->num = json_object_get_int64(obj);


//}

/* ******************************************* */
//void WECHATstats::deserialize(json_object *o) {
//    json_object *obj;

//    if(!o) return;

//    if(json_object_object_get_ex(o, "id", &obj))
//        deserializeStats(obj, &wechatID);
//}

/* ******************************************* */
//void WECHATstats::lua(lua_State *vm) {
//    lua_newtable(vm);
//    luaStats(vm, &wechatID, "id");
//    lua_pushstring(vm, "wechat");
//    lua_insert(vm, -2);
//    lua_settable(vm, -3);
//}

/* *************************************** */

//char* WECHATstats::serialize() {
//    json_object *my_object = getJSONObject();
//    char *rsp = strdup(json_object_to_json_string(my_object));
//    /* Free memory */
//    json_object_put(my_object);
//    return(rsp);
//}

/* ******************************************* */
