
extern void libxt_DSCP_init(void);
extern void libxt_MARK_init(void);
extern void libxt_NFQUEUE_init(void);
extern void libxt_SKIPLOG_init(void);
extern void libxt_TCPMSS_init(void);
extern void libxt_conntrack_init(void);
extern void libxt_dscp_init(void);
extern void libxt_length_init(void);
extern void libxt_limit_init(void);
extern void libxt_mac_init(void);
extern void libxt_mark_init(void);
extern void libxt_owner_init(void);
extern void libxt_policy_init(void);
extern void libxt_standard_init(void);
extern void libxt_tcp_init(void);
extern void libxt_udp_init(void);
void init_extensions(void);
void init_extensions(void)
{
 libxt_DSCP_init();
 libxt_MARK_init();
 libxt_NFQUEUE_init();
 libxt_SKIPLOG_init();
 libxt_TCPMSS_init();
 libxt_conntrack_init();
 libxt_dscp_init();
 libxt_length_init();
 libxt_limit_init();
 libxt_mac_init();
 libxt_mark_init();
 libxt_owner_init();
 libxt_policy_init();
 libxt_standard_init();
 libxt_tcp_init();
 libxt_udp_init();
}
