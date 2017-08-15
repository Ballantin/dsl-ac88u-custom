
extern void libipt_DNAT_init(void);
extern void libipt_LOG_init(void);
extern void libipt_MASQUERADE_init(void);
extern void libipt_REDIRECT_init(void);
extern void libipt_SNAT_init(void);
extern void libipt_icmp_init(void);
void init_extensions4(void);
void init_extensions4(void)
{
 libipt_DNAT_init();
 libipt_LOG_init();
 libipt_MASQUERADE_init();
 libipt_REDIRECT_init();
 libipt_SNAT_init();
 libipt_icmp_init();
}
