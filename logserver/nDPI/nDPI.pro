TEMPLATE = lib
TARGET = ndpi
VERSION = 2.1.0
#CONFIG = static
CONFIG -= qt

DESTDIR += ../lib

#test
#INCLUDEPATH +=/ntop/include/ndpi\
#                     /ntop/include/ndpi/third_party


INCLUDEPATH += ./include \
                         ./lib/third_party/include \



SOURCES += \
    lib/protocols/afp.c \
    lib/protocols/aimini.c \
    lib/protocols/amqp.c \
    lib/protocols/applejuice.c \
    lib/protocols/armagetron.c \
    lib/protocols/ayiya.c \
    lib/protocols/battlefield.c \
    lib/protocols/bgp.c \
    lib/protocols/bittorrent.c \
    lib/protocols/bjnp.c \
    lib/protocols/btlib.c \
    lib/protocols/ciscovpn.c \
    lib/protocols/citrix.c \
    lib/protocols/coap.c \
    lib/protocols/collectd.c \
    lib/protocols/corba.c \
    lib/protocols/crossfire.c \
    lib/protocols/dcerpc.c \
    lib/protocols/dhcp.c \
    lib/protocols/dhcpv6.c \
    lib/protocols/directconnect.c \
    lib/protocols/directdownloadlink.c \
    lib/protocols/dns.c \
    lib/protocols/dofus.c \
    lib/protocols/drda.c \
    lib/protocols/dropbox.c \
    lib/protocols/eaq.c \
    lib/protocols/edonkey.c \
    lib/protocols/fasttrack.c \
    lib/protocols/fiesta.c \
    lib/protocols/filetopia.c \
    lib/protocols/fix.c \
    lib/protocols/florensia.c \
    lib/protocols/ftp_control.c \
    lib/protocols/ftp_data.c \
    lib/protocols/git.c \
    lib/protocols/gnutella.c \
    lib/protocols/gtp.c \
    lib/protocols/guildwars.c \
    lib/protocols/h323.c \
    lib/protocols/halflife2_and_mods.c \
    lib/protocols/hangout.c \
    lib/protocols/hep.c \
    lib/protocols/http.c \
    lib/protocols/http_activesync.c \
    lib/protocols/iax.c \
    lib/protocols/icecast.c \
    lib/protocols/ipp.c \
    lib/protocols/irc.c \
    lib/protocols/jabber.c \
    lib/protocols/kakaotalk_voice.c \
    lib/protocols/kerberos.c \
    lib/protocols/kontiki.c \
    lib/protocols/ldap.c \
    lib/protocols/lotus_notes.c \
    lib/protocols/mail_imap.c \
    lib/protocols/mail_pop.c \
    lib/protocols/mail_smtp.c \
    lib/protocols/maplestory.c \
    lib/protocols/mdns.c \
    lib/protocols/megaco.c \
    lib/protocols/mgcp.c \
    lib/protocols/mms.c \
    lib/protocols/mpegts.c \
    lib/protocols/mqtt.c \
    lib/protocols/msn.c \
    lib/protocols/mssql_tds.c \
    lib/protocols/mysql.c \
    lib/protocols/netbios.c \
    lib/protocols/netflow.c \
    lib/protocols/nfs.c \
    lib/protocols/noe.c \
    lib/protocols/non_tcp_udp.c \
    lib/protocols/ntp.c \
    lib/protocols/openft.c \
    lib/protocols/openvpn.c \
    lib/protocols/oracle.c \
    lib/protocols/oscar.c \
    lib/protocols/pando.c \
    lib/protocols/pcanywhere.c \
    lib/protocols/postgres.c \
    lib/protocols/pplive.c \
    lib/protocols/ppstream.c \
    lib/protocols/pptp.c \
    lib/protocols/qq.c \
    lib/protocols/quake.c \
    lib/protocols/quic.c \
    lib/protocols/radius.c \
    lib/protocols/rdp.c \
    lib/protocols/redis_net.c \
    lib/protocols/rsync.c \
    lib/protocols/rtcp.c \
    lib/protocols/rtmp.c \
    lib/protocols/rtp.c \
    lib/protocols/rtsp.c \
    lib/protocols/rx.c \
    lib/protocols/sflow.c \
    lib/protocols/shoutcast.c \
    lib/protocols/sip.c \
    lib/protocols/skinny.c \
    lib/protocols/skype.c \
    lib/protocols/smb.c \
    lib/protocols/smpp.c \
    lib/protocols/snmp.c \
    lib/protocols/socks45.c \
    lib/protocols/socrates.c \
    lib/protocols/SOMEIP.c \
    lib/protocols/sopcast.c \
    lib/protocols/soulseek.c \
    lib/protocols/spotify.c \
    lib/protocols/ssdp.c \
    lib/protocols/ssh.c \
    lib/protocols/ssl.c \
    lib/protocols/starcraft.c \
    lib/protocols/stealthnet.c \
    lib/protocols/steam.c \
    lib/protocols/stun.c \
    lib/protocols/syslog.c \
    lib/protocols/tcp_udp.c \
    lib/protocols/teamspeak.c \
    lib/protocols/teamviewer.c \
    lib/protocols/telegram.c \
    lib/protocols/telnet.c \
    lib/protocols/teredo.c \
    lib/protocols/tftp.c \
    lib/protocols/thunder.c \
    lib/protocols/tinc.c \
    lib/protocols/tor.c \
    lib/protocols/tvants.c \
    lib/protocols/tvuplayer.c \
    lib/protocols/ubntac2.c \
    lib/protocols/usenet.c \
    lib/protocols/vhua.c \
    lib/protocols/viber.c \
    lib/protocols/vmware.c \
    lib/protocols/vnc.c \
    lib/protocols/warcraft3.c \
    lib/protocols/whoisdas.c \
    lib/protocols/world_of_kung_fu.c \
    lib/protocols/world_of_warcraft.c \
    lib/protocols/xbox.c \
    lib/protocols/xdmcp.c \
    lib/protocols/yahoo.c \
    lib/protocols/zattoo.c \
    lib/protocols/zeromq.c \
    lib/ndpi_main.c \
    lib/third_party/src/ahocorasick.c \
    lib/third_party/src/libcache.c \
    lib/third_party/src/ndpi_patricia.c \
    lib/third_party/src/node.c \
    lib/third_party/src/sort.c \
    lib/protocols/wechat.c \
    lib/protocols/oicq.c \
    lib/xshash_1.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/ndpi_api.h \
    include/ndpi_define.h \
    include/ndpi_includes.h \
    include/ndpi_main.h \
    include/ndpi_protocol_ids.h \
    include/ndpi_protocols.h \
    include/ndpi_unix.h \
    include/ndpi_win32.h \
    lib/protocols/btlib.h \
    lib/third_party/include/actypes.h \
    lib/third_party/include/ahocorasick.h \
    lib/third_party/include/libcache.h \
    lib/third_party/include/ndpi_patricia.h \
    lib/third_party/include/node.h \
    lib/third_party/include/sort.h \
    lib/xshash_1.h

