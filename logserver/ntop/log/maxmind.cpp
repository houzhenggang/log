#include <errno.h>
#include "ntop_includes.h"
#include "xsdpilog.h"
#include <maxminddb.h>
#include <stdlib.h>
#include <string.h>

MMDB_s* g_mmdb = NULL;

void print_indentation(FILE *stream, int i)
{
    char buffer[1024];
    int size = i >= 1024 ? 1023 : i;
    memset(buffer, 32, size);
    buffer[size] = '\0';
    fputs(buffer, stream);
}

#define MAYBE_CHECK_SIZE_OVERFLOW(lhs, rhs, error) \
    if ((lhs) > (rhs)) {                           \
        return error;                              \
    }

char *bytes_to_hex(uint8_t *bytes, uint32_t size)
{
    char *hex_string;
    MAYBE_CHECK_SIZE_OVERFLOW(size, SIZE_MAX / 2 - 1, NULL);

    hex_string = (char*)malloc((size * 2) + 1);
    if (NULL == hex_string) {
        return NULL;
    }

    for (uint32_t i = 0; i < size; i++) {
        sprintf(hex_string + (2 * i), "%02X", bytes[i]);
    }

    return hex_string;
}

enum labs {
    CITY=1,
    COUNTRY,
    latitude,
    longitude,
    SUBDIVISIONS
};



MMDB_entry_data_list_s *dump_entry_data_list(
        FILE *stream, MMDB_entry_data_list_s *entry_data_list, int indent,
        int *status,int nNeed,LocalInfo* pInfo)
{
    switch (entry_data_list->entry_data.type) {
    case MMDB_DATA_TYPE_MAP:
        {
            uint32_t size = entry_data_list->entry_data.data_size;

            indent += 2;

            for (entry_data_list = entry_data_list->next;
                 size && entry_data_list; size--) {

                if (MMDB_DATA_TYPE_UTF8_STRING !=
                    entry_data_list->entry_data.type) {
                    *status = MMDB_INVALID_DATA_ERROR;
                    return NULL;
                }
                char *key =
                   strndup(
                        (char *)entry_data_list->entry_data.utf8_string,
                        entry_data_list->entry_data.data_size);
                if (NULL == key) {
                    *status = MMDB_OUT_OF_MEMORY_ERROR;
                    return NULL;
                }
                int nNeed = 0;
                if(strcasecmp(key,"city") == 0)
                {
                    pInfo->nLabs = CITY;
                }
                else if(strcasecmp(key,"country") == 0)
                {
                    pInfo->nLabs = COUNTRY;
                }
                else if(strcasecmp(key,"latitude") == 0)
                {
                    pInfo->nLabs = latitude;
                }
                else if(strcasecmp(key,"longitude") == 0)
                {
                    pInfo->nLabs = longitude;
                }
                else if(strcasecmp(key,"subdivisions") == 0)
                {
                    pInfo->nLabs = SUBDIVISIONS;
                }

                if(strcmp(key,"zh-CN") == 0 ||
                   strcmp(key,"latitude") == 0 ||
                   strcmp(key,"longitude") == 0)
                {
                    nNeed = 1;
                }

                free(key);

                entry_data_list = entry_data_list->next;
                entry_data_list =
                    dump_entry_data_list(stream, entry_data_list, indent + 2,
                                         status,nNeed,pInfo);

                if (MMDB_SUCCESS != *status) {
                    return NULL;
                }
            }

            indent -= 2;

        }
        break;
    case MMDB_DATA_TYPE_ARRAY:
        {
            uint32_t size = entry_data_list->entry_data.data_size;

            indent += 2;

            for (entry_data_list = entry_data_list->next;
                 size && entry_data_list; size--) {
                entry_data_list =
                    dump_entry_data_list(stream, entry_data_list, indent,
                                         status,0,pInfo);
                if (MMDB_SUCCESS != *status) {
                    return NULL;
                }
            }

            indent -= 2;
        }
        break;
    case MMDB_DATA_TYPE_UTF8_STRING:
        {
            char *string =
                strndup((char *)entry_data_list->entry_data.utf8_string,
                             entry_data_list->entry_data.data_size);
            if (NULL == string) {
                *status = MMDB_OUT_OF_MEMORY_ERROR;
                return NULL;
            }
            if(nNeed)
            {
                switch(pInfo->nLabs)
                {
                case CITY:
                    memcpy(pInfo->szCity,string,entry_data_list->entry_data.data_size);
                    pInfo->nLabs = 0;
                    break;
                case COUNTRY:
                    memcpy(pInfo->szCountry,string,entry_data_list->entry_data.data_size);

                    pInfo->nLabs = 0;
                    break;
                case SUBDIVISIONS:
                    memcpy(pInfo->szSubdivisions,string,entry_data_list->entry_data.data_size);

                    pInfo->nLabs = 0;
                    break;
                default:
                    break;
                }
            }
            free(string);
            entry_data_list = entry_data_list->next;
        }
        break;
    case MMDB_DATA_TYPE_BYTES:
        {


            entry_data_list = entry_data_list->next;
        }
        break;
    case MMDB_DATA_TYPE_DOUBLE:
        if(nNeed)
        {

            if(pInfo->nLabs == latitude)
            {
                pInfo->latitude = entry_data_list->entry_data.double_value;

            }
            else if(pInfo->nLabs == longitude)
            {
                pInfo->longitude = entry_data_list->entry_data.double_value;

            }

        }
        entry_data_list = entry_data_list->next;
        break;
    case MMDB_DATA_TYPE_FLOAT:

        entry_data_list = entry_data_list->next;
        break;
    case MMDB_DATA_TYPE_UINT16:
        {

            entry_data_list = entry_data_list->next;
        }
        break;
    case MMDB_DATA_TYPE_UINT32:
        {

            entry_data_list = entry_data_list->next;
        }
        break;
    case MMDB_DATA_TYPE_BOOLEAN:

        entry_data_list = entry_data_list->next;
        break;
    case MMDB_DATA_TYPE_UINT64:

        entry_data_list = entry_data_list->next;
        break;
   case MMDB_DATA_TYPE_UINT128:

        entry_data_list = entry_data_list->next;
        break;
     case MMDB_DATA_TYPE_INT32:

        entry_data_list = entry_data_list->next;
        break;
    default:
        *status = MMDB_INVALID_DATA_ERROR;
        return NULL;
    }

    *status = MMDB_SUCCESS;
    return entry_data_list;
}

int open_MMDB()
{
    g_mmdb = new MMDB_s;
    int status = MMDB_open("./mmdb/GeoLite2-City.mmdb", MMDB_MODE_MMAP, g_mmdb);

    if (MMDB_SUCCESS != status) {
        if (MMDB_IO_ERROR == status) {
            fprintf(stderr, "    IO error: %s\n", strerror(errno));
        }
        delete g_mmdb;
        g_mmdb = NULL;
        return -1;
    }
    return 1;
}

int XsGet_LocalInfo(ParseFiled* pParseFiled)
{
    if(g_mmdb == NULL && open_MMDB() == -1)
        return -1;
    timeval t1,t2;
    gettimeofday(&t1,NULL);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(atoi(pParseFiled->bufDip));
    char* pIP = inet_ntoa(addr.sin_addr);

    int gai_error, mmdb_error;
    MMDB_lookup_result_s result =
        MMDB_lookup_string(g_mmdb, pIP, &gai_error, &mmdb_error);

    if (0 != gai_error) {
        fprintf(stderr,
                "\n  Error from getaddrinfo for %s - %s\n\n",
                pIP, gai_strerror(gai_error));
        return -1;
    }

    if (MMDB_SUCCESS != mmdb_error) {
        fprintf(stderr,
                "\n  Got an error from libmaxminddb: %s\n\n",
                MMDB_strerror(mmdb_error));
        return -1;
    }

    MMDB_entry_data_list_s *entry_data_list = NULL;

    if (result.found_entry) {
        int status = MMDB_get_entry_data_list(&result.entry,
                                              &entry_data_list);

        if (MMDB_SUCCESS != status) {
            fprintf(
                stderr,
                "Got an error looking up the entry data - %s\n",
                MMDB_strerror(status));
            goto end;
        }

        if (NULL != entry_data_list) {
            int nRet;
            int nNeed = 0;
            dump_entry_data_list(stdout, entry_data_list, 2,&nRet,nNeed,&pParseFiled->localinfo);
            MMDB_free_entry_data_list(entry_data_list);
            char szPath[] = {"香港"};
            char szCountry[] = {"中国"};
            if(strstr(pParseFiled->localinfo.szCountry,szPath))
            {
                memcpy(pParseFiled->localinfo.szCity,
                       pParseFiled->localinfo.szCountry,strlen(pParseFiled->localinfo.szCountry));
                memset(pParseFiled->localinfo.szCountry,0,256);
                memcpy(pParseFiled->localinfo.szCountry,szCountry,strlen(szCountry));
            }
            return 0;
        }
    }
    end:
        MMDB_free_entry_data_list(entry_data_list);
        return -1;
}

