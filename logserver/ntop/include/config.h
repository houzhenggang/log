/* include/config.h.  Generated from config.h.in by configure.  */
/* include/config.h.in.  Generated from configure.ac by autoheader.  */

/* GeoIP support is present */
/* #undef HAVE_GEOIP */

/* Define to 1 if you have the <GeoIP.h> header file. */
/* #undef HAVE_GEOIP_H */

/* GeoIP not supported: library too old, please upgrade first */
/* #undef HAVE_GEOIP_IPv6 */

/* Local hiredis package present */
/* #undef HAVE_HIREDIS */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* LDAP support is present */
/* #undef HAVE_LDAP */

/* Define to 1 if you have the `GeoIP' library (-lGeoIP). */
/* #undef HAVE_LIBGEOIP */

/* Define to 1 if you have the `netfilter_queue' library (-lnetfilter_queue).
   */
/* #undef HAVE_LIBNETFILTER_QUEUE */

/* Define to 1 if you have the <libnetfilter_queue/libnetfilter_queue.h>
   header file. */
/* #undef HAVE_LIBNETFILTER_QUEUE_LIBNETFILTER_QUEUE_H */

/* Define to 1 if you have the `nfnetlink' library (-lnfnetlink). */
/* #undef HAVE_LIBNFNETLINK */

/* Define to 1 if you have the `wrap' library (-lwrap). */
/* #undef HAVE_LIBWRAP */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* use mysql */
#define HAVE_MYSQL 1

/* NFQ is present */
/* #undef HAVE_NETFILTER */

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define if nfq_set_verdict2 exists in netfilter_queue. */
/* #undef HAVE_NFQ_SET_VERDICT2 */

/* Native PF_RING support */
//#define HAVE_PF_RING 1

/* We have sqlite */
#define HAVE_SQLITE 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* zlib is present */
#define HAVE_ZLIB 1

/* has openssl */
#define NO_SSL_DL 1

/* Last GIT change */
#define NTOPNG_GIT_DATE "2017年 07月 01日 星期六 00:32:25 CST"

/* GIT Release */
#define NTOPNG_GIT_RELEASE ":3.1.170701"

/* Building ntopng professional */
/* #undef NTOPNG_PRO */

/* ntopng professional date */
/* #undef NTOPNG_PRO_GIT_DATE */

/* ntopng professional release */
/* #undef NTOPNG_PRO_GIT_RELEASE */

/* ntopng has license */
/* #undef NTOPNG_PRO_HAVE_LICENSE */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Architecture of this host */
#define PACKAGE_MACHINE "x86_64"

/* Define to the full name of this package. */
#define PACKAGE_NAME "ntopng"

/* OS Name */
#define PACKAGE_OS "CentOS Linux release 7.2.1511 (Core) "

/* OS name */
#define PACKAGE_OSNAME "x86_64-unknown-linux-gnu"

/* GIT release of this package */
#define PACKAGE_RELEASE "r3.1.170701"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "ntopng 3.1.170701"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "ntopng"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.1.170701"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Disable warning on windows */
#define _CRT_SECURE_NO_WARNINGS 1
