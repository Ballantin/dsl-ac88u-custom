#ifndef _DNS_CACHE_QUERY_H
#define _DNS_CACHE_QUERY_H

#include <time.h>
#include "udns.h"


typedef enum dns_query_type dns_query_type_t;
enum dns_query_type
{
	DNS_A,
	DNS_SRV,
	DNS_NAPTR,
};

typedef struct ipArr
{
	char *ip;
	struct ipArr *next;
} ipArr_t;

typedef struct srvAns
{
	char *domainName;
	struct ipArr *ipArrHead;
	int port;
	struct srvAns *next;
} srvAns_t;

typedef struct naptrAns
{
	char *flags;
	char *service;
	char *replacement;
	struct srvAns *srvList;
	struct naptrAns *next;
} naptrAns_t;

typedef struct dns_a4_ans
{
	char *domainName;
	time_t ttl;
	struct ipArr *ipArrHead;
	struct dns_a4_ans *next;
} dns_a4_ans_t;

typedef struct dns_a6_ans
{
	char *domainName;
	time_t ttl;
	struct ipArr *ipArrHead;
	struct dns_a6_ans *next;
} dns_a6_ans_t;

typedef struct dns_srv_ans
{
	char *domainName;
	time_t ttl;
	srvAns_t *srvHead;
	struct dns_srv_ans *next;
} dns_srv_ans_t;

typedef struct dns_naptr_ans
{
	char *domainName;
	time_t ttl;
	struct naptrAns *naptrList;
	struct dns_naptr_ans *next;
} dns_naptr_ans_t;

typedef struct dns_ip dns_ip_t;

typedef enum dns_txp_mode
{
	UDNS_UDP,
	UDNS_TCP,
} dns_txp_mode_t;

typedef enum dns_ip_format
{
	UDNS_AF_INET,   // IPv4
	UDNS_AF_INET6,	// IPv6
} dns_ip_format_t;

struct dns_ip
{
	char *ip;
	dns_ip_format_t type; //Add for indicate IPv4 or IPv6 format, yushiuan 20160711
	int port;
	dns_txp_mode_t proto_type;
	time_t ttl;
	struct dns_ip *next;
};
#ifdef HAVE_IPv6 //YS add it 2016.11.24 
dns_ip_t *dns_cache_query(char *server, char *service, dns_txp_mode_t proto, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout, unsigned short default_answer_port, dns_query_type_t dns_type, int local_ip_have_v6, int allowIPv6);
dns_srv_ans_t *dns_cache_query_srv(char *server, char *service, char *proto, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout,int local_ip_have_v6, int allowIPv6);
#else
dns_ip_t *dns_cache_query(char *server, char *service, dns_txp_mode_t proto, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout, unsigned short default_answer_port, dns_query_type_t dns_type);
dns_srv_ans_t *dns_cache_query_srv(char *server, char *service, char *proto, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout);
#endif
dns_a4_ans_t *dns_cache_query_a4(char *server, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout);
dns_a6_ans_t *dns_cache_query_a6(char *server, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout);
dns_naptr_ans_t *dns_cache_query_naptr(char *server, char *domain, char *my_ip, unsigned short my_port, unsigned int timeout);
int dns_a4_ans_free(dns_a4_ans_t *ans);
int dns_srv_ans_free(dns_srv_ans_t *ans);
int dns_naptr_ans_free(dns_naptr_ans_t *ans);
int dns_cache_release();
int dns_validIP(char *ipAddress);
int dns_validIPv6(char *ipAddress);
int dns_check_expire(char *domain);
int dns_ip_printf(dns_ip_t *ip);
int dns_ip_free(dns_ip_t *ip);
int dns_ip_check_ttl_timeout(dns_ip_t *ip);

#endif