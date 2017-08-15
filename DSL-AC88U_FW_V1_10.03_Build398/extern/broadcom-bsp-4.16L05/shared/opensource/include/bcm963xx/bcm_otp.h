
#if !defined(_BCM_OTP_H_)
#define _BCM_OTP_H_

int bcm_otp_get_row(int row, unsigned int* val);
int bcm_otp_is_btrm_boot(void);
int bcm_otp_is_boot_secure(void);
#if defined (CONFIG_BCM96848) || defined(_BCM96848_)
int bcm_otp_get_revId(void);
int bcm_otp_get_max_clksel(void);
#endif

#endif  /* _BCM_OTP_H_ */


