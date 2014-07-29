#ifndef AAT_TYPES_H
#define AAT_TYPES_H

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned short uint16;
typedef unsigned short ushort16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned char uchar8;
typedef signed char char8;

/*****************************************************************************
 * the follwoing is IP related definition
 ****************************************************************************/

/* the maximum of IP string address is 39, plus 1 '\0' */
#define IPNG_ADDR_STRING_LENGTH     (39+1)
/* the maximum string address is 39, plus 1 '/', plus maxinum prefix length 3, plus 1 '\0' */
#define IPNG_PREFIX_STRING_LENGTH   (39+1+3+1)
/* the maximum of Mac string address is 17, plus 1 '\0' */
/* e.g. 00-00-00-00-00-00                               */
#define IPNG_MAC_STRING_LENGTH      (17+1)



typedef enum {
    IP_VERSION_UNKNOWN = 0,
    IP_VERSION_4,
    IP_VERSION_6
} T_global_IP_VERSION;


/* the following is IP address for IPv4 */
typedef union
{
    struct   {uchar8     s_b1, s_b2, s_b3, s_b4;} S_ip_b;
    struct   {ushort16   s_w1, s_w2;            } S_ip_w;
    uint32   S_addr;

} T_global_IP_ADDR;


/* the following is IP address for IPv6 */
typedef union {
    uchar8  s6_u8[16];
    uint16  s6_u16[8];
    uint32  s6_u32[4];
    uint64  s6_u64[2];

    /* the following is mainly used for type translation between
     * IPNG and IPNet, which is defined in Ip_in6_addr */
    union in6_addr_union
    {
        uchar8   addr8[16];
        uint16   addr16[8];
        uint32   addr32[4];
    } in6;

} T_global_IP6_ADDR;

/*
IP_PACKED1 struct Ip_in6_addr
{
    union Ip_in6_addr_union
    {
        Ip_u8    addr8[16];
        Ip_u16   addr16[8];
        Ip_u32   addr32[4];
    } in6;
}
*/

/* the following is IP address for IPv4 and IPv6
 * IP NG = IP next generarion, it is for the next generation
 * of IP in 751x platform, which will use the WindRiver IPNet
 * protocol suit
 * for IPv6 porting guide, refer to documenet: 803-04791-D001-DF
 */
typedef struct {
    T_global_IP_VERSION ipVer;
    union {
        T_global_IP_ADDR ip4;
        T_global_IP6_ADDR ip6;
    }u;

#define ipng_ip4 u.ip4
#define ipng_ip6 u.ip6

} T_global_IPNG_ADDR;

char* snprintIp4a(char *buf, uint32 len, const T_global_IP_ADDR *ip4_addr);
char* snprintIp6a(char *buf, uint32 len, const T_global_IP6_ADDR *ip6_addr);
char* snprintIpnga(char *buf, uint32 len, const T_global_IPNG_ADDR *ipng_addr);



typedef enum {
/**********************************
 * System Modules
 **********************************/
  AAT_MOD_MIN = 0xcc01,   /* First Module */
  AAT_MOD_CDBM = AAT_MOD_MIN, /* Timer processing */

  AAT_MOD_MAX
} AAT_MODULES;

#define AAT_MOD_MIN_INDEX  1                            /* Lowest module index */
#define AAT_MOD_MAX_INDEX  AAT_MOD_MAX-AAT_MOD_MIN+1    /* Highest module index */
#define AAT_MAX_MODULES    AAT_MOD_MAX_INDEX+1

#define AAT_MOD_INDEX(id)  (id & 0xff)



#endif /* AAT_TYPES_H */
