/*
 * Bsafe uHSM io definitions
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_io.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */
/*
 * 	bsafe_io.h
 */

/* default compilation is for little endian host */
/* Endianesss.... all values would take care of endianess at driver, and  all pointers should be taken care by the application */

/* packing has been tried to fix at 4 bytes */

#include <bsafe_common.h>

#ifndef BSAFE_UHSM_IO_H_
#define BSAFE_UHSM_IO_H_


typedef enum bsafe_command_IOCTL {
	BSAFE_UHSM_INIT_DEVICEKEY_IOCTL=1,
	BSAFE_UHSM_SELFTEST_IOCTL,
	BSAFE_UHSM_DEV_KDI_PUBLIC_IOCTL,
	BSAFE_UHSM_DEV_KDC_PUBLIC_IOCTL,
	BSAFE_UHSM_KEK_STATUS_IOCTL,
	BSAFE_UHSM_AKEY_STATUS_IOCTL,
	BSAFE_UHSM_CLR_KEY_IOCTL,
	BSAFE_UHSM_LD_USERKEY_IOCTL,
	BSAFE_UHSM_LD_CFG_IOCTL,
	BSAFE_UHSM_DLIES_GEN_KS_IOCTL,
	BSAFE_UHSM_DLIES_MSG_DEC_IOCTL,
	BSAFE_UHSM_USERAUTH_IOCTL,
	BSAFE_UHSM_FIPS_CLR_IOCTL,
	BSAFE_UHSM_USER_STATUS_IOCTL,
	BSAFE_UHSM_USER_RANDOM_IOCTL,
	BSAFE_UHSM_USER_PKE_IOCTL,
	BSAFE_UHSM_USER_SHA1_IOCTL,
	BSAFE_UHSM_USER_3DES_HMACSHA1_IOCTL,
	BSAFE_UHSM_USER_NOP_IOCTL,
	/* special handling ioctl's  bignums in any format has to be in 4 byte aligned */
	BSAFE_LE_BIGNUM_IOCTL=0x0200,	/* Convert LE Bignum(openssl le machines) to BSAFE_BIGNUM */ 
	BSAFE_BN_2_BIGNUM_IOCTL=0x02001	/* Convert BN(network byte order) to BSAFE_BIGNUM */ 
}bsafe_command_IOCTL_t;



typedef struct bsafe_io_common_param {
	UINT32 returnCode;	/* has to be the first argument */
	INT returnStatus; /* return value from driver could be negative */
	/*unsigned long time_us;*/
} bsafe_io_common_param_t;



typedef struct bsafe_init_devicekey_io {
	bsafe_io_common_param_t param;
	UINT32 uhsmConfig;
	BYTE *prodConfig;		/* 4 bytes */
	BYTE *authorization;		/* 20 bytes */
	UINT16 numRowRetry;
	UINT16 numColRetry;
	BYTE *statistics;		/* 16 bytes */
} bsafe_init_devicekey_io_t;


typedef struct bsafe_selftest_io {
	bsafe_io_common_param_t param;
	UINT32 runtest;
	UINT32 result;
} bsafe_selftest_io_t; 



typedef struct bsafe_fips_clr_io {
	bsafe_io_common_param_t param;
	UINT32 clrVector;
	UINT16 numRetry;
} bsafe_fips_clr_io_t;

typedef struct bsafe_dev_kdi_public_io {
	bsafe_io_common_param_t param;
	UINT32 options;
	BYTE *configuration;	/* 8 bytes */	
	BYTE *authorization;	/* 20 bytes */
	bsafe_dsa_pub_t kdi_p;		/* p, q, g, y  total 404 bytes */
	UINT16 keyType;		/* moved down for aligned packing */
} bsafe_dev_kdi_public_io_t; 


typedef struct bsafe_dev_kdc_public_io {
	bsafe_io_common_param_t param;
	UINT32 options;
	BYTE *kdc_cp;			/* 256(kdc-cpub) bytes */
	BSAFE_BIGNUM *msgSig_r;		/* 20 bytes r */
	BSAFE_BIGNUM *msgSig_s;		/* 20 bytes s */
	UINT16 keyType;			/* moved down for aligned packing */
} bsafe_dev_kdc_public_io_t; 


typedef struct bsafe_kek_akey_status_io {
	bsafe_io_common_param_t param;
	BYTE *keyPolicy;	/* 6 bytes */		
	UINT16 location;
	UINT16 keyType;		
	UINT32 kSize;		
} bsafe_kek_akey_status_io_t;

typedef struct bsafe_clr_key_io {
	bsafe_io_common_param_t param;
	UINT16 cache;
	UINT16 location;
	UINT32 kSize;		
} bsafe_clr_key_io_t;


typedef struct bsafe_ld_userkey_io {
	bsafe_io_common_param_t param;
	UINT32 kSize;
	BYTE * keyData;
	UINT32 keyData_len;
	UINT16 akeyLoc;		
} bsafe_ld_userkey_io_t;

typedef struct bsafe_ld_cfg_io {
	bsafe_io_common_param_t param;
	BYTE *keyPolicy;	/* 6 btyes */
	bsafe_dsa_pub_t *km_pub;
	BYTE *dlies_msg;
	UINT32 dlies_msg_len;
	BSAFE_BIGNUM *authSig_r;	/* 20 bytes */
	BSAFE_BIGNUM *authSig_s;	/* 20 bytes */
	UINT16 keyType;
} bsafe_ld_cfg_io_t;

typedef struct bsafe_dlies_gen_ks_io {
	bsafe_io_common_param_t param;
	UINT16 kekLoc;		/* in systems with only one session key it is  BSAFE_KSESSION_LOC */
	BYTE *challenge;	/* 20 bytes */
	BYTE *ks_cp;		/* 256 bytes */
	BSAFE_BIGNUM *msgSig_r;		/* 20 bytes */
	BSAFE_BIGNUM *msgSig_s;		/* 20 bytes */
	UINT16 keyType;
} bsafe_dlies_gen_ks_io_t;


typedef struct bsafe_userauth_io {
	bsafe_io_common_param_t param;
	BYTE *challenge;	/* 20 bytes */
	BSAFE_BIGNUM *msgSig_r;		/* 20 bytes */
	BSAFE_BIGNUM *msgSig_s;		/* 20 bytes */
} bsafe_userauth_io_t;

typedef enum bsafe_dlies_msg_dec_io_flags {
	BSAFE_UHSM_DRIVER_GET_KEY_LOC,
}bsafe_dlies_msg_dec_io_flags_t;

typedef struct bsafe_dlies_msg_dec_io {
	bsafe_io_common_param_t param;
	UINT16 *decLocArray;	
	BYTE *dlies_msg;
	UINT32 dlies_msg_len;
	UINT16 kekLoc;		/* is a return value if not set to NULL*/
	UINT16 akeyLoc;		/* is a return value if not set to NULL*/
	UINT32 msgId;
	UINT16 kType;
	UINT16 kSize;
	UINT32 flags;		/* for driver */
	UINT16 nestLevel;	/* max should be 2 */
} bsafe_dlies_msg_dec_io_t;



typedef struct bsafe_user_status_io {
	bsafe_io_common_param_t param;
	UINT32 salEventStatus;
} bsafe_user_status_io_t;

typedef struct bsafe_user_nop_io {
	bsafe_io_common_param_t param;
	BYTE *dataIn;
	UINT32 data_len;
	BYTE *dataOut;	/* dataOut length == data_len */
} bsafe_user_nop_io_t;

typedef struct bsafe_user_random_io {
	bsafe_io_common_param_t param;
	UINT32 rngType;
	BYTE *random;		/* 20 bytes */
} bsafe_user_random_io_t;


typedef struct bsafe_user_pke_io {
	bsafe_io_common_param_t param;
	BYTE *pkeMsgIn;		/* a bin array[normal format] array */
	UINT32 pkeMsg_len;
	BYTE *pkeMsgOut;	/* pkeMsgOut length == pkeMsgIn_len */ /* bin array */
	UINT16 akekLoc;
	UINT16 opCode;
} bsafe_user_pke_io_t;


/* requires revisit split pkeMsgIn with appropriate values of struct */
typedef struct bsafe_user_sha1_io {
	bsafe_io_common_param_t param;
	UINT16 prevLen;
	UINT16 mode;
	BYTE *inDigest;		/* 20 bytes */
	BYTE *data;
	UINT32 data_len;
	BYTE *outDigest;	/* 20 bytes */
} bsafe_user_sha1_io_t;



typedef struct bsafe_user_3des_hmacsha1_io {
	bsafe_io_common_param_t param;
	UINT32 mode;
	UINT16 akey3DesLoc;
	UINT16 akeyHashLoc;
	BYTE *iv;		/* 8 bytes */
	BYTE *dataIn;
	UINT32 data_len;
	BYTE *outDigest;	/* 20 bytes */
	BYTE *dataOut;		/* dataOut length == data_len */
} bsafe_user_3des_hmacsha1_io_t ;

/* misc ioctls */

typedef struct bsafe_bignum_prop_io {
	bsafe_io_common_param_t param;
} bsafe_bignum_prop_io_t;



#endif /* BSAFE_UHSM_IO_H_ */
