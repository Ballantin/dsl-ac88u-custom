/*
 * Bsafe uHSM common definitions.
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_common.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef __BSAFE_COMMON_H_
#define __BSAFE_COMMON_H_

/* include vers.h */
#include <bsafe_vers.h>

/* returnstatus returned by the chip */
#include <bsafe_returncode.h>

/* status returned by driver */
#include <bsafe_status.h>

/* devname */
#include <bsafe_devname.h>


typedef enum bsafe_keyloc_properties {
	BSAFE_INVALID_LOC=0xffff,
	BSAFE_KDI_S_LOC=0,
	BSAFE_KDC_S_LOC,
	BSAFE_KSESSION_LOC,
	BSAFE_KEK3_LOC,
	BSAFE_KEK4_LOC,
	BSAFE_MIN_AKEY_LOC=0,
	BSAFE_MAX_AKEY_LOC=25,
	BSAFE_MAX_KEKLOC=2,
	BSAFE_KEKLOC_BUKSIZE=1024,
	BSAFE_AKEYLOC_BUKSIZE=32,
	BSAFE_MAX_AKEYLOC=26
} bsafe_keyloc_properties_t;




typedef enum bsafe_clr_key_cache {
	BSAFE_KEK_CACHE=0x00,
	BSAFE_AKEY_CACHE=0x01
} bsafe_clr_key_cache_t;


/* the values  p, g, q. and y are in bn format MSB to LSB */
typedef struct bsafe_dsa_pub {
	unsigned char * p;	/* 128 bytes */
	unsigned char * g;	/* 128 bytes */
	unsigned char * q;	/* 20 bytes */
	unsigned char * y;	/* 128 bytes */
} bsafe_dsa_pub_t;


typedef enum bsafe_init_devicekey_uhsmConfig_bits {
	DC_DOMESTIC=0,
	DC_EXPORT=1,
	DC_DIS_USERSTATUS=1<<1 ,
	DC_DIS_RAWRNG=1<<2,
	DC_MODSIZE_1024=1<<3,
	DC_RUNSELFTEST=1<<4,
	DC_ENABLE_USERKEY=1<<5,
	DC_DIS_USERAUTH=1<<6,
	DC_USERAUTH_SHA1=1<<7,
	DC_DIS_DEBUG_ECODE=1<<8,
	DC_AUTH_VALID=1<<13,
	SC_EN_LOWFREQ=1<<16,
	SC_EN_RSTMON=1<<17,
	SC_EN_JMPMON=1<<18,
	SC_EN_EXCPTMON=1<<19,
	VOLATILE_MODE=1<<31
} bsafe_init_devicekey_uhsmConfig_bits_t;

typedef enum bsafe_selftest_bitvector_bits {
	RAM_BIST_ROM_BIST_TEST=1<<0,
	NVM_CHECKSUM_VERIFY_TEST=1<<1,
	HMAC_SHA1_ENGINE_TEST=1<<2,
	DES3_ENGINE_TEST=1<<3,
	FIPS186_2_PSEUDO_RANDOM_TEST=1<<4,
	DSA_SIGN_VERIFY_TEST=1<<5,
	DH_TEST=1<<6,
	RSA_ENC_DEC_TEST=1<<7,
	DSA_SIGN_VERFIY_KDI_TEST=1<<8,
	FIPS140_2_RANDOM_NUM_TEST=1<<9,
	RANDOM_NUM_STAT_TEST=1<<10,
} bsafe_selftest_bitvector_bits_t;

typedef enum bsafe_dev_kdikdc_public_bits {
	NONE_ON_GPIO=0,
	HASH_ON_GPIO=1,
	KDCDI_ON_GPIO=1<<1,
} bsafe_dev_kdikdc_public_bits_t;

typedef enum bsafe_dlies_msg_dec_msgid {
	BSAFE_CRYPTTEXT=0x22754804,
} bsafe_dlies_msg_dec_msgid_t;


typedef enum bsafe_user_order_mode {
	BSAFE_MODE_ENC_AUTH=0x0,
	BSAFE_MODE_AUTH_ENC=0x1
}bsafe_user_order_mode_t;

typedef enum bsafe_user_direction_mode {
	BSAFE_MODE_DECRPT=0x0,
	BSAFE_MODE_ENCRYPT=0x1<<1
}bsafe_user_direction_mode_t;

typedef enum bsafe_user_hmac_mode {
	BSAFE_MODE_HMAC_INIT=0x0,
	BSAFE_MODE_HMAC_UPDATE=0x1<<4,
	BSAFE_MODE_HMAC_FINISH=0x2<<4,
	BSAFE_MODE_HMAC_NULL=0x3<<4 ,
}bsafe_user_hmac_mode_t;

typedef enum bsafe_user_des_mode {
	BSAFE_MODE_DES_CBC=0x0,			/* returns iv */
	BSAFE_MODE_DES_ECB=0x1<<6,		
	BSAFE_MODE_RESERVED=0x2<<6,
	BSAFE_MODE_DES_NULL=0x3<<6 ,
}bsafe_user_des_mode_t;

typedef enum bsafe_user_sha1_mode {
	BSAFE_MODE_SHA1_INIT=0x0,
	BSAFE_MODE_SHA1_UPDATE=0x01,
	BSAFE_MODE_SHA1_FINISH=0x02,
}bsafe_user_sha1_mode_t;


typedef enum bsafe_fips_clrvector {
	BSAFE_FIPS_CLR_VECTOR=0xFA1E4907
}bsafe_fips_clrvector_t;

typedef enum bsafe_user_rngtype {
	BSAFE_USER_RANDOM_RNGTYPE_RAW=0x00,
	BSAFE_USER_RANDOM_RNGTYPE_X=0x01,
	BSAFE_USER_RANDOM_RNGTYPE_K=0x02	
} bsafe_user_rngtype_t;

typedef enum bsafe_pke_opcode {
	BSAFE_DH_PUBLIC_KEY_GEN=0x01,
	BSAFE_DH_SECRET_KEY_GEN=0x02,
	BSAFE_RSA_PUBLIC_KEY=0x03,
	BSAFE_RSA_PRIVATE_KEY_CRT=0x04,
	BSAFE_DSA_SIGN=0x05,
	BSAFE_DSA_VERIFY=0x06,
	BSAFE_RNG_DIRECT=0x41,
	BSAFE_RNG_SHA1=0x42,
	BSAFE_MOD_ADD=0x43,
	BSAFE_MOD_SUB=0x44,
	BSAFE_MOD_MULTIPLY=0x45,
	BSAFE_MOD_REDUCTION=0x46,
	BSAFE_MOD_EXP=0x47,
	BSAFE_MOD_DBL_MOD_EXP=0x49
} bsafe_pke_opcode_t;




/* BSAFE_BIGNUM */
/* bignum is a big-endian array of BSAFE_BIGNUM_BYTES[4 bytes] bigendian-words */
/* where it is ordered in memory by words as LSW........MSW  where each word is in bigendian. ie words are BE and the array of words is in LE */
/*
In other words the BSAFE_BIGNUM is an array of 32 bit integers[Words] ordered from Least-Significant-Word to Most-Signifcant-Word, where each Word is ordered as Most-Signifcant byte to Least-Significant byte.
*/
/* eg:
	Array Index
	a            0   1   2   3   4   5   6   7 
		    MSB                          LSB

	Bin array [Normal format]
		    MSW              LSW                   
	a[8]     ={0x8,0x7,0x6,0x5,0x4,0x3,0x2,0x1}
			ie a[0]= 0x8 ....  a[7]=0x1

	Bsafe BIGNUM format
		    LSW              MSW                   
	BIGNUM[8]={0x4,0x3,0x2,0x1,0x8,0x7,0x6,0x5}	
		similar to openssl 4byte bignums on BE machines
	Note: openssl 4byte bignums on LE machines is
	OPENLE[8]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8}
*/
/* expecting unsigned char is 1 byte */
#define BSAFE_BIGNUM		unsigned char 
/* bignums are aligned to 4 byte boundaries */
#define BSAFE_BIGNUM_BYTES	4

#endif /* __BSAFE_COMMON_H__ */

