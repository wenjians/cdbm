
#include "aat-incl.h"
#include "return_codes.h"

#include <string.h>
#include <arpa/inet.h>


char* snprintIp4a(char *buf, uint32 len, const T_global_IP_ADDR *ip4_addr)
{
    int  out_len;

    AAT_STD_ASSERT(ip4_addr != NULL);
    AAT_STD_ASSERT(buf != NULL);

    buf[0] = '\0';
    if (inet_ntop (AF_INET, ip4_addr, buf, len) == NULL)
        return buf;

    out_len = strlen(buf);
    AAT_FMT_ASSERT((out_len<len), ("buffer len(%d) too small, need (%d)\n", len, out_len));

    return buf;
} /* snprintIp4a */

char* snprintIp6a(char *buf, uint32 len, const T_global_IP6_ADDR *ip6_addr)
{
    int  out_len;

    AAT_STD_ASSERT(ip6_addr != NULL);
    AAT_STD_ASSERT(buf != NULL);

    buf[0] = '\0';
    if (inet_ntop (AF_INET6, &(ip6_addr->in6), buf, len) == NULL)
        return buf;

    out_len = strlen(buf);

    AAT_FMT_ASSERT((out_len<len), ("buffer len(%d) too small, need (%d)\n", len, out_len));
    return buf;
} /* snprintIp6a */


char* snprintIpnga(char *buf, uint32 len, const T_global_IPNG_ADDR *ipng_addr)
{
    AAT_STD_ASSERT(ipng_addr != NULL);
    AAT_STD_ASSERT(buf != NULL);

    buf[0] = '\0';

    if (ipng_addr->ipVer == IP_VERSION_4)
        return snprintIp4a(buf, len, &(ipng_addr->ipng_ip4));

    else if (ipng_addr->ipVer == IP_VERSION_6)
        return snprintIp6a(buf, len, &(ipng_addr->ipng_ip6));

    else
        return buf;
} /* snprintIpnga */


T_global_rc ip4_get_addr_from_str(const char *buf, T_global_IP_ADDR *ip4_addr)
{
    AAT_STD_ASSERT(buf != NULL);
    AAT_STD_ASSERT(ip4_addr != NULL);

    if (inet_pton(AF_INET, buf, ip4_addr) != 1)
        return RC_FAILED;

    return RC_OK;
}

T_global_rc ip6_get_addr_from_str(const char *buf, T_global_IP6_ADDR *ip6_addr)
{
    AAT_STD_ASSERT(buf != NULL);
    AAT_STD_ASSERT(ip6_addr != NULL);

    if (inet_pton(AF_INET6, buf, ip6_addr)!= 1)
        return RC_FAILED;

    return RC_OK;
}


T_global_rc ipng_get_addr_from_str(const char *buf, T_global_IPNG_ADDR *ipng_addr)
{
    AAT_STD_ASSERT(buf != NULL);
    AAT_STD_ASSERT(ipng_addr != NULL);

    /* it is IPv6 address */
    if (inet_pton(AF_INET6, buf, &(ipng_addr->ipng_ip6)) == 1)
    {
        ipng_addr->ipVer = IP_VERSION_6;

        return RC_OK;
    }

    /* it is IPv4 address */
    if (inet_pton(AF_INET, buf, &(ipng_addr->ipng_ip4)) == 1)
    {
        ipng_addr->ipVer = IP_VERSION_4;

        return RC_OK;;
    }

    return RC_FAILED;
}


uint32 ipng_is_addr_equal(const T_global_IPNG_ADDR *addr1, const T_global_IPNG_ADDR *addr2)
{
    if (addr1->ipVer != addr2->ipVer)
        return 0;

    if (addr1->ipVer == IP_VERSION_4)
        return ip4_is_addr_equal(addr1->ipng_ip4, addr2->ipng_ip4);

    else if (addr1->ipVer == IP_VERSION_6)
        return ip6_is_addr_equal(&addr1->ipng_ip6, &addr2->ipng_ip6);

    return 0;  // failure case
}

