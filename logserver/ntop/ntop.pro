TEMPLATE = app
TARGET = xslog
VERSION = 3.1.17
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR += ../bin

INCLUDEPATH += /usr/include/mysql    \
                          /usr/local/include/json

INCLUDEPATH +=/opt/xslog/include/third-party/LuaJIT-2.0.3  \
                        /opt/xslog/include/redis
#test
#INCLUDEPATH +=/ntop/include/ntop\
#                       /ntop/include/ndpi\
#                           /ntop/include/third-party/patricia \
#                         /ntop/include/third-party/mongoose

INCLUDEPATH += ./include \
                ./log \
                           ../nDPI/include \
                           ../zeromq/include \
                          ./third-party/patricia \
                         ./third-party/mongoose

LIBS += -lpthread -lz -lcrypto -lssl -lsqlite3 -lrrd -lpcap -lcurl \
            -L/usr/lib64/mysql -lmysqlclient

#LIBS += -L/ntop/lib -lluajit-5.1 -lzmq -ljson-c  -lhiredis
LIBS += -L/opt/xslog/lib  -ljson-c  -lhiredis -lndpi -lmaxminddb

LIBS += -L/usr/local/lib -lzmq

LIBS += -L/QTProject/clear -lrdstats_light -lsyscom
SOURCES += \
    src/ActivityStats.cpp \
    src/AddressList.cpp \
    src/AddressResolution.cpp \
    src/AddressTree.cpp \
    src/AlertCounter.cpp \
    src/AlertsManager.cpp \
    src/AutonomousSystem.cpp \
    src/AutonomousSystemHash.cpp \
    src/CategoryStats.cpp \
    src/CollectorInterface.cpp \
    src/DB.cpp \
    src/DivertInterface.cpp \
    src/DnsStats.cpp \
    src/DummyInterface.cpp \
    src/ElasticSearch.cpp \
    src/EthStats.cpp \
    src/ExportInterface.cpp \
    src/Flashstart.cpp \
    src/Flow.cpp \
    src/FlowHash.cpp \
    src/FrequentNumericItems.cpp \
    src/FrequentStringItems.cpp \
    src/GenericHash.cpp \
    src/GenericHashEntry.cpp \
    src/GenericHost.cpp \
    src/GenericTrafficElement.cpp \
    src/Geolocation.cpp \
    src/Grouper.cpp \
    src/Host.cpp \
    src/HostHash.cpp \
    src/HostPools.cpp \
    src/HTTPBL.cpp \
    src/HTTPserver.cpp \
    src/HTTPstats.cpp \
    src/ICMPstats.cpp \
    src/InterfaceStatsHash.cpp \
    src/IpAddress.cpp \
    src/LocalTrafficStats.cpp \
    src/Logstash.cpp \
    src/Lua.cpp \
    src/Mac.cpp \
    src/MacHash.cpp \
    src/MacManufacturers.cpp \
    src/main.cpp \
    src/Mutex.cpp \
    src/MySQLDB.cpp \
    src/nDPIStats.cpp \
    src/NetworkDiscovery.cpp \
    src/NetworkInterface.cpp \
    src/NetworkStats.cpp \
    src/Ntop.cpp \
    src/NtopGlobals.cpp \
    src/PacketDumper.cpp \
    src/PacketDumperTuntap.cpp \
    src/PacketStats.cpp \
    src/Paginator.cpp \
    src/ParserInterface.cpp \
    src/PcapInterface.cpp \
    src/PeriodicActivities.cpp \
    src/PF_RINGInterface.cpp \
    src/Prefs.cpp \
    src/ProtoStats.cpp \
    src/Redis.cpp \
    src/RuntimePrefs.cpp \
    src/StatsManager.cpp \
    src/StoreManager.cpp \
    src/TcpFlowStats.cpp \
    src/TcpPacketStats.cpp \
    src/ThreadedActivity.cpp \
    src/Trace.cpp \
    src/TrafficStats.cpp \
    src/Utils.cpp \
    src/ViewInterface.cpp \
    src/VirtualHost.cpp \
    src/VirtualHostHash.cpp \
    src/Vlan.cpp \
    src/VlanHash.cpp \
    src/ZCCollectorInterface.cpp \
    log/wechat.cpp \
    xsmysql/mysqldb.cpp \
    xsmysql/mydbop.cpp \
    lib/rbtree.cpp \
    lib/rbtree_callback.cpp \
    lib/getparamvalue.cpp \
    lib/xs_list.cpp \
    lib/xsarray.cpp \
    lib/trasharray.cpp \
    lib/uintsegtable.cpp \
    log/ntoplog.cpp \
    log/xsdpilog.cpp \
    web/web_interaction.cpp \
    lib/common.cpp \
    web/mac_msg.cpp \
    web/flow_msg.cpp \
    web/host_msg.cpp \
    web/run_preferences.cpp \
    web/interface_msg.cpp \
    log/httpcmulti1.cpp \
    log/httpcmulti.cpp \
    lib/xshash.cpp \
    lib/xssharefunc.cpp \
    log/savemsg.cpp \
    log/xs_idc.cpp \
    lib/cjson.cpp \
    lib/crypt.cpp \
    lib/key_elem.cpp \
    lib/rbset.cpp \
    lib/mygethostbyname.cpp \
    lib/xs_utils.cpp \
    lib/xsinifile.cpp \
    lib/xs_crypt.cpp \
    log/maintain.cpp \
    log/elasticsearch.cpp \
    log/radlog.cpp \
    log/maxmind.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/ActivityStats.h \
    include/AddressList.h \
    include/AddressResolution.h \
    include/AddressTree.h \
    include/AlertCounter.h \
    include/AlertsManager.h \
    include/AutonomousSystem.h \
    include/AutonomousSystemHash.h \
    include/CategoryStats.h \
    include/CollectorInterface.h \
    include/config.h \
    include/DB.h \
    include/DivertInterface.h \
    include/DnsStats.h \
    include/DummyInterface.h \
    include/ElasticSearch.h \
    include/EthStats.h \
    include/ExportInterface.h \
    include/Flashstart.h \
    include/Flow.h \
    include/FlowHash.h \
    include/FrequentNumericItems.h \
    include/FrequentStringItems.h \
    include/GenericHash.h \
    include/GenericHashEntry.h \
    include/GenericHost.h \
    include/GenericTrafficElement.h \
    include/Geolocation.h \
    include/Grouper.h \
    include/Host.h \
    include/HostHash.h \
    include/HostPools.h \
    include/HTTPBL.h \
    include/HTTPserver.h \
    include/HTTPstats.h \
    include/ICMPstats.h \
    include/InterfaceStatsHash.h \
    include/IpAddress.h \
    include/LocalTrafficStats.h \
    include/Logstash.h \
    include/Lua.h \
    include/Mac.h \
    include/MacHash.h \
    include/MacManufacturers.h \
    include/Mutex.h \
    include/MySQLDB.h \
    include/nDPIStats.h \
    include/NetworkDiscovery.h \
    include/NetworkInterface.h \
    include/NetworkStats.h \
    include/Ntop.h \
    include/ntop_defines.h \
    include/ntop_flow.h \
    include/ntop_includes.h \
    include/ntop_typedefs.h \
    include/ntop_win32.h \
    include/NtopGlobals.h \
    include/PacketDumper.h \
    include/PacketDumperGeneric.h \
    include/PacketDumperTuntap.h \
    include/PacketStats.h \
    include/Paginator.h \
    include/ParserInterface.h \
    include/PcapInterface.h \
    include/PeriodicActivities.h \
    include/PF_RINGInterface.h \
    include/Prefs.h \
    include/ProtoStats.h \
    include/Redis.h \
    include/RuntimePrefs.h \
    include/SPSCQueue.h \
    include/StatsManager.h \
    include/StoreManager.h \
    include/TcpFlowStats.h \
    include/TcpPacketStats.h \
    include/ThreadedActivity.h \
    include/Trace.h \
    include/TrafficStats.h \
    include/UserActivityStats.h \
    include/Utils.h \
    include/ViewInterface.h \
    include/VirtualHost.h \
    include/VirtualHostHash.h \
    include/Vlan.h \
    include/VlanHash.h \
    include/ZCCollectorInterface.h \
    third-party/mongoose/mongoose.h \
    third-party/patricia/patricia.h \
    log/wechat.h \
    xsmysql/mydbop.h \
    xsmysql/mysqldb.h \
    lib/xsbool.h \
    lib/rbtree.h \
    lib/rbtree_callback.h \
    lib/xsarray.h \
    lib/xs_list.h \
    lib/xs_mutex.h \
    lib/getparamvalue.h \
    lib/xs_prediction.h \
    lib/xspublicfun.h \
    lib/xstydef.h \
    lib/uintsegtable.h \
    lib/xsdefine.h \
    lib/xstydef2.h \
    lib/trasharray.h \
    log/ntoplog.h \
    log/xsdpilog.h \
    web/web_interaction.h \
    lib/common.h \
    log/httpcmulti1.h \
    lib/xshash.h \
    log/httpcmulti.h \
    lib/xssharefunc.h \
    log/savemsg.h \
    log/xs_idc.h \
    lib/cjson.h \
    lib/crypt.h \
    lib/key_elem.h \
    lib/mygethostbyname.h \
    lib/rbset.h \
    lib/xs_compatibility.h \
    lib/xs_crypt.h \
    lib/xs_def.h \
    lib/xs_utils.h \
    lib/xscompat.h \
    lib/xsinifile.h \
    lib/xsipcstruct.h \
    lib/xssharedef.h \
    lib/xswebaction.h \
    log/maintain.h \
    log/elasticsearch.h \
    log/radlog.h \
    include/maxminddb.h \
    include/maxminddb_config.h \
    log/iostat.h \
    include/common.h \
    include/count.h \
    include/ioconf.h \
    include/rd_stats.h \
    include/sysconfig.h \
    include/version.h

DISTFILES += \
    src/service_win32.c.bak \
    src/ntop_win32.c.bak

