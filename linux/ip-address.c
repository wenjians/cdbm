
#include "aat-incl.h"

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


#if 0
uint32 ip4_is_valid_addr(const char *strptr)
{
    int result = ;

    /*
    inet_pton() returns 1 on success (network address was successfully converted).  0 is returned if  src  does  not
    contain a character string representing a valid network address in the specified address family.  If af does not
    contain a valid address family, -1 is returned and errno is set to EAFNOSUPPORT.
    */

    return TRUE;
}


uint32 ip6_is_valid_addr(const char *strptr)
{

}

uint32 ipng_is_valid_addr(const char *strptr)
{
    return (ip6_is_valid_addr(strptr) || ip4_is_valid_addr(strptr));
}



 int
 main(int argc, char *argv[])
 {
     unsigned char buf[sizeof(struct in6_addr)];
     int domain, s;
     char str[INET6_ADDRSTRLEN];

     if (argc != 3) {
         fprintf(stderr, "Usage: %s {i4|i6|<num>} string\n", argv[0]);
         exit(EXIT_FAILURE);
     }

     domain = (strcmp(argv[1], "i4") == 0) ? AF_INET :
              (strcmp(argv[1], "i6") == 0) ? AF_INET6 : atoi(argv[1]);

     s = inet_pton(domain, argv[2], buf);


     if (s <= 0) {
         if (s == 0)
            fprintf(stderr, "Not in presentation format");
         else
             perror("inet_pton");
         exit(EXIT_FAILURE);
     }

     if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        exit(EXIT_FAILURE);
     }

     printf("%s\n", str);

     exit(EXIT_SUCCESS);
}

#endif

