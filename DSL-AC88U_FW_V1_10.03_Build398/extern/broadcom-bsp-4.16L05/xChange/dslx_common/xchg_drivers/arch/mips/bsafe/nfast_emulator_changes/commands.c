/*
 * commands.c
 *
 * The software emulator example code. Contains an implementation of 
 * parts of the uHSM-KDP V1.1 specification.
 *
 * Copyright 2003 nCipher Corporation Limited.
 *
 * This example source code is provided for your information and
 * assistance.  See the file LICENCE.TXT for details and the
 * terms and conditions of the licence which governs the use of the
 * source code. By using such source code you will be accepting these
 * terms and conditions.  If you do not wish to accept these terms and
 * conditions, DO NOT OPEN THE FILE OR USE THE SOURCE CODE.
 *
 * Note that there is NO WARRANTY.
 */

#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#define PRINTF		printf

#include "errmacros.h"
#include "handy.h"
#include "readpp.h"
#define NF_REPORT_NO_ABBREVS
#include "report.h"

#include "emulator.h"
#include "kdphandy.h"
#include "dlies.h"

#include "openssl/bn.h"
#include "openssl/dsa.h"
#include "openssl/evp.h"
#include "openssl/sha.h"

#define MAX_RECEIVER_STATES 16
#define MAX_APPKEY_STATES 16

/* Four keys plus some bits and pieces shouldn't take more than 8K */
#define NVRAM_SIZE 8192

static M_KDPKey kdi_priv, kdi_pub;
static M_KDPKey kdc_priv, kdc_pub;

static struct NF_UserData ud = { 0 };


#define BSAFE_bin2bn BN_bin2bn
#define BSAFE_bn2bin BN_bn2bin
#ifdef USE_BSAFE_UHSM
#include <bsafe_uhsm_lib.h>
//#define BSAFE_OUTPUT	1
int try_bsafe_init_devicekey(int bsafe_fd);
#endif /* USE_BSAFE_UHSM */



#ifndef USE_BSAFE_UHSM
/* to use fixed keys on sw for testing */
#define USE_BSAFE_FIXED_KEYS	1
#endif

/* to do both hw and sw */
//#define DO_SW_ALSO	1
#ifdef DO_SW_ALSO
/* to use fixed keys on sw for testing */
#define USE_BSAFE_FIXED_KEYS	1
#endif

#define BSAFE_FIXED_KEYS	1

/* all arrays here are in bn formart ..MSB...to LSB */
#ifdef BSAFE_FIXED_KEYS
unsigned char g_dsa_p[128]={
0xb7, 0x56, 0xe5, 0x02, 0xda, 0x69, 0x22, 0xb4, 0xff, 0x25, 0xdf, 0x06, 0x28, 0x19, 0x54, 0xc2, 
0x19, 0x73, 0x6e, 0xcc, 0xc8, 0xc6, 0xc7, 0xda, 0x21, 0xed, 0x71, 0xc1, 0xf8, 0x6c, 0x34, 0x76, 
0xb2, 0xac, 0x00, 0x48, 0xf3, 0x66, 0x1a, 0x6c, 0x22, 0x63, 0xad, 0x32, 0x03, 0xd9, 0x31, 0x0e, 
0x6e, 0x5a, 0x24, 0xf7, 0xe0, 0x36, 0xe3, 0x87, 0x62, 0x10, 0x26, 0x0e, 0xb7, 0x7f, 0x30, 0x21, 
0x8e, 0x15, 0x69, 0x82, 0xd1, 0xba, 0x7f, 0x1e, 0xcc, 0x34, 0xb9, 0x70, 0x46, 0x74, 0x3a, 0x2f, 
0x0e, 0x53, 0x42, 0x29, 0x0f, 0xe1, 0x97, 0x34, 0x77, 0xbf, 0xf3, 0x62, 0xb4, 0x25, 0xb9, 0xd3, 
0xc5, 0xeb, 0x16, 0xe9, 0xd2, 0xb5, 0x55, 0x5e, 0xde, 0x7e, 0xfc, 0x3a, 0xdf, 0x76, 0x34, 0xf2, 
0xc1, 0x96, 0xdd, 0x3d, 0x86, 0xaa, 0xbe, 0x66, 0x30, 0x6f, 0x46, 0x67, 0x0f, 0x5a, 0xa8, 0xd7,
};
unsigned char g_dsa_g[128]={
0x3f, 0xe2, 0xbb, 0x9c, 0x3f, 0x15, 0x93, 0xca, 0xd0, 0xc0, 0x4e, 0x33, 0x37, 0x97, 0x3f, 0xfe, 
0x2b, 0xcb, 0x13, 0x89, 0x64, 0x69, 0x4d, 0xb6, 0x1a, 0xa3, 0x61, 0x6d, 0x56, 0x12, 0x6c, 0x0b, 
0x7a, 0x5b, 0xfe, 0x16, 0x13, 0x72, 0x3a, 0xf5, 0x1f, 0xf9, 0xac, 0x3d, 0x4d, 0x3b, 0xf3, 0x67, 
0xdc, 0x1c, 0x32, 0x22, 0x13, 0x35, 0x3d, 0x85, 0x5d, 0x77, 0xe4, 0xe9, 0x02, 0x25, 0x7b, 0xe5, 
0x78, 0x75, 0x0b, 0x52, 0x1a, 0xc6, 0xd4, 0x38, 0xc5, 0x5b, 0x77, 0x73, 0x25, 0x95, 0xab, 0xc1, 
0xe8, 0x4f, 0x1c, 0x24, 0xd5, 0xf2, 0x34, 0xde, 0xee, 0x2e, 0xda, 0x2e, 0x81, 0x86, 0x65, 0x6a, 
0x10, 0x07, 0x18, 0x92, 0x77, 0xc7, 0xb1, 0xd1, 0x29, 0x82, 0x67, 0xd5, 0x5e, 0x2e, 0x9f, 0xe9, 
0x63, 0xfb, 0xd8, 0x65, 0x6a, 0xd9, 0x90, 0x4c, 0x73, 0x24, 0xc4, 0xa8, 0x48, 0xf1, 0xaf, 0xa1,
};
unsigned char g_dsa_q[20]={
0xdc, 0x33, 0xf7, 0x54, 0x10, 0xf0, 0x95, 0x82, 0x5e, 0x5c, 0xc0, 0xc4, 0x76, 0x42, 0xe3, 0x06, 
0x7e, 0xc4, 0xe6, 0x8d,
};
unsigned char g_dsa_priv_key[20]={
0x72, 0xb7, 0x5d, 0xbe, 0xdd, 0x75, 0x89, 0xc5, 0x7d, 0x43, 0x2e, 0xcf, 0xd0, 0xf8, 0x78, 0xba, 
0xc9, 0x04, 0x4e, 0x4f,
};

#if 0
unsigned char g_dh_p[256]={
0x84, 0x81, 0x28, 0x1b, 0x4c, 0xa7, 0x28, 0x94, 0xc4, 0x17, 0x56, 0xce, 0x72, 0x48, 0x87, 0x67, 
0x0d, 0xc5, 0xa5, 0x93, 0xca, 0xf9, 0xb4, 0x53, 0x82, 0x20, 0x89, 0x84, 0x5b, 0x86, 0xff, 0x1b, 
0xe3, 0x5d, 0x21, 0xe6, 0x27, 0xa1, 0x3d, 0x9d, 0xe1, 0xd0, 0x29, 0x66, 0x4e, 0x2a, 0x81, 0x08, 
0x16, 0x8d, 0x98, 0xd1, 0xc5, 0xbc, 0xe7, 0x5e, 0x9d, 0xa3, 0x61, 0xa3, 0xf7, 0x55, 0x27, 0x18, 
0x75, 0xe0, 0x8b, 0xed, 0x7e, 0xb7, 0xe7, 0x59, 0xaf, 0x63, 0xf3, 0x83, 0x77, 0xc2, 0x26, 0x51, 
0x22, 0x22, 0xba, 0xb8, 0x50, 0x8c, 0x62, 0x58, 0x92, 0xd7, 0x78, 0x08, 0x61, 0xd0, 0xe6, 0x3e, 
0xba, 0x23, 0xb6, 0xa3, 0xdf, 0x85, 0xfd, 0x81, 0xa4, 0x1d, 0x11, 0x16, 0x45, 0xd6, 0x1b, 0x49, 
0x39, 0x95, 0xdd, 0xe8, 0x55, 0xf6, 0x04, 0xb1, 0xcd, 0xce, 0xba, 0x3a, 0xbc, 0xe5, 0xa8, 0x05, 
0xb7, 0xc9, 0x00, 0x59, 0xac, 0x79, 0xda, 0x20, 0x65, 0x34, 0x9f, 0xc4, 0x18, 0xd8, 0x79, 0x1e, 
0xf7, 0xe6, 0x18, 0x8b, 0xad, 0x1e, 0x8c, 0x2c, 0xc2, 0xb9, 0x1a, 0x8d, 0xb1, 0x5b, 0x9a, 0x62, 
0x64, 0x2a, 0xf9, 0xfb, 0x14, 0xfc, 0x30, 0x51, 0x05, 0xc0, 0xf2, 0x75, 0xdb, 0x3b, 0xad, 0x2b, 
0xc8, 0x6f, 0xec, 0x76, 0x4e, 0x90, 0xb5, 0x9e, 0xb6, 0x0a, 0x87, 0xad, 0xad, 0x6b, 0xb6, 0xa0, 
0xd2, 0x0b, 0xf4, 0xec, 0x19, 0xdc, 0xfb, 0xa3, 0x5f, 0x33, 0xf3, 0x90, 0x31, 0xec, 0x1f, 0x34, 
0xd9, 0xda, 0xf2, 0x6d, 0x3c, 0x53, 0x90, 0x3e, 0xe3, 0x6b, 0x83, 0xcf, 0xc9, 0xa6, 0x54, 0xd9, 
0x00, 0x33, 0x61, 0x91, 0x62, 0xc6, 0x90, 0xe0, 0x78, 0xeb, 0xb4, 0xbe, 0x45, 0xb8, 0x92, 0xc5, 
0x4e, 0xed, 0x6f, 0x66, 0x9c, 0x38, 0xe5, 0xc8, 0x83, 0x52, 0x36, 0x8b, 0xb4, 0x3c, 0x27, 0xe5,
};
unsigned char g_dh_g[256]={
0x27, 0x70, 0x8d, 0x3c, 0x83, 0x5d, 0x62, 0x75, 0xb0, 0x6f, 0xcf, 0x6f, 0x19, 0xd5, 0xd4, 0x61, 
0x49, 0xd2, 0x39, 0xe3, 0x6c, 0x2b, 0xad, 0xbc, 0x41, 0x0e, 0x62, 0x10, 0x2f, 0x9b, 0xb7, 0xf8, 
0x37, 0xfe, 0x49, 0x15, 0x8a, 0x2e, 0x72, 0x7e, 0xd3, 0xa2, 0xd0, 0x58, 0x92, 0x96, 0xff, 0x72, 
0x81, 0x9a, 0xff, 0x44, 0x12, 0xeb, 0xe2, 0xa0, 0x80, 0xd2, 0x8f, 0xaf, 0xb4, 0x42, 0x42, 0x7b, 
0x98, 0x4d, 0x22, 0xa4, 0xd0, 0xe8, 0x7b, 0x53, 0x72, 0xb0, 0x13, 0x36, 0x15, 0x94, 0x18, 0x5b, 
0xc4, 0x2f, 0x7e, 0x88, 0x5e, 0x33, 0xc1, 0x26, 0xef, 0xf9, 0x34, 0x3a, 0x98, 0x1d, 0xb9, 0xe4, 
0x83, 0x98, 0x54, 0xe2, 0x69, 0x47, 0x0b, 0x1c, 0x30, 0x81, 0xb3, 0x00, 0xf4, 0x26, 0x96, 0xe7, 
0xe8, 0xef, 0x15, 0x3d, 0x80, 0x6f, 0xc3, 0x43, 0x91, 0x10, 0x2c, 0x9d, 0x94, 0xce, 0x56, 0x53, 
0x76, 0x07, 0xa3, 0x86, 0x78, 0x9e, 0x21, 0xa0, 0x1f, 0xaa, 0x0a, 0xef, 0x44, 0x76, 0x4e, 0xc7, 
0x99, 0xc7, 0x13, 0x51, 0x33, 0xb1, 0xae, 0xc9, 0x9f, 0x1f, 0xfe, 0xe7, 0x2a, 0xee, 0xa4, 0xb7, 
0xa4, 0x26, 0x80, 0x91, 0x48, 0xde, 0x0c, 0x40, 0x55, 0x5d, 0xee, 0xbd, 0xc0, 0x38, 0xd9, 0xd1, 
0x2c, 0xe3, 0x2a, 0x08, 0x31, 0x5e, 0x96, 0xde, 0xfb, 0x5c, 0x03, 0x5a, 0x8d, 0xf5, 0xb7, 0x81, 
0x32, 0xa6, 0xbe, 0x3e, 0x31, 0x54, 0xba, 0x84, 0x95, 0xf9, 0x15, 0x5b, 0x82, 0xc8, 0xd2, 0xa3, 
0x85, 0xc0, 0x75, 0x77, 0x13, 0x07, 0xac, 0x68, 0x66, 0x71, 0xfb, 0x74, 0x6a, 0x51, 0x51, 0x2a, 
0xde, 0x29, 0x68, 0x87, 0x49, 0xe4, 0xa6, 0x04, 0x98, 0x88, 0x32, 0xd0, 0xd0, 0x4c, 0x88, 0xbf, 
0xa8, 0x3b, 0x54, 0x22, 0x5d, 0xe9, 0x87, 0x57, 0x2e, 0x3e, 0xc4, 0x40, 0xa5, 0xea, 0xab, 0xfd,
};
unsigned char g_dh_priv_key[32]={
0xb5, 0x89, 0x78, 0xe6, 0xe5, 0x7f, 0xbb, 0x07, 0x90, 0xa1, 0x8e, 0x87, 0xc7, 0x68, 0x14, 0xea, 
0xc9, 0xba, 0xc6, 0x1e, 0x13, 0xaf, 0x07, 0x91, 0x2c, 0x0d, 0xf8, 0x18, 0xa7, 0x34, 0x90, 0xd3, 
};

#else
unsigned char g_dh_g[4]={
0x00, 0x00, 0x00, 0x02,
};
unsigned char g_dh_p[256]={
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc9, 0x0f, 0xda, 0xa2, 0x21, 0x68, 0xc2, 0x34, 
0xc4, 0xc6, 0x62, 0x8b, 0x80, 0xdc, 0x1c, 0xd1, 0x29, 0x02, 0x4e, 0x08, 0x8a, 0x67, 0xcc, 0x74, 
0x02, 0x0b, 0xbe, 0xa6, 0x3b, 0x13, 0x9b, 0x22, 0x51, 0x4a, 0x08, 0x79, 0x8e, 0x34, 0x04, 0xdd, 
0xef, 0x95, 0x19, 0xb3, 0xcd, 0x3a, 0x43, 0x1b, 0x30, 0x2b, 0x0a, 0x6d, 0xf2, 0x5f, 0x14, 0x37, 
0x4f, 0xe1, 0x35, 0x6d, 0x6d, 0x51, 0xc2, 0x45, 0xe4, 0x85, 0xb5, 0x76, 0x62, 0x5e, 0x7e, 0xc6, 
0xf4, 0x4c, 0x42, 0xe9, 0xa6, 0x37, 0xed, 0x6b, 0x0b, 0xff, 0x5c, 0xb6, 0xf4, 0x06, 0xb7, 0xed, 
0xee, 0x38, 0x6b, 0xfb, 0x5a, 0x89, 0x9f, 0xa5, 0xae, 0x9f, 0x24, 0x11, 0x7c, 0x4b, 0x1f, 0xe6, 
0x49, 0x28, 0x66, 0x51, 0xec, 0xe4, 0x5b, 0x3d, 0xc2, 0x00, 0x7c, 0xb8, 0xa1, 0x63, 0xbf, 0x05, 
0x98, 0xda, 0x48, 0x36, 0x1c, 0x55, 0xd3, 0x9a, 0x69, 0x16, 0x3f, 0xa8, 0xfd, 0x24, 0xcf, 0x5f, 
0x83, 0x65, 0x5d, 0x23, 0xdc, 0xa3, 0xad, 0x96, 0x1c, 0x62, 0xf3, 0x56, 0x20, 0x85, 0x52, 0xbb, 
0x9e, 0xd5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6d, 0x67, 0x0c, 0x35, 0x4e, 0x4a, 0xbc, 0x98, 0x04, 
0xf1, 0x74, 0x6c, 0x08, 0xca, 0x18, 0x21, 0x7c, 0x32, 0x90, 0x5e, 0x46, 0x2e, 0x36, 0xce, 0x3b, 
0xe3, 0x9e, 0x77, 0x2c, 0x18, 0x0e, 0x86, 0x03, 0x9b, 0x27, 0x83, 0xa2, 0xec, 0x07, 0xa2, 0x8f, 
0xb5, 0xc5, 0x5d, 0xf0, 0x6f, 0x4c, 0x52, 0xc9, 0xde, 0x2b, 0xcb, 0xf6, 0x95, 0x58, 0x17, 0x18, 
0x39, 0x95, 0x49, 0x7c, 0xea, 0x95, 0x6a, 0xe5, 0x15, 0xd2, 0x26, 0x18, 0x98, 0xfa, 0x05, 0x10, 
0x15, 0x72, 0x8e, 0x5a, 0x8a, 0xac, 0xaa, 0x68, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
unsigned char g_dh_priv_key[32]={
0xb5, 0x89, 0x78, 0xe6, 0xe5, 0x7f, 0xbb, 0x07, 0x90, 0xa1, 0x8e, 0x87, 0xc7, 0x68, 0x14, 0xea, 
0xc9, 0xba, 0xc6, 0x1e, 0x13, 0xaf, 0x07, 0x91, 0x2c, 0x0d, 0xf8, 0x18, 0xa7, 0x34, 0x90, 0xd3,
};
#endif	/* old */

unsigned char g_kdp_start_priv_key[32]={
0x56, 0xa3, 0x3d, 0xcd, 0x13, 0x5d, 0x83, 0x32, 0xf9, 0x97, 0x11, 0xd4, 0xa9, 0x23, 0x70, 0x08,
0x16, 0x40, 0x2b, 0x72, 0x61, 0xcf, 0x8f, 0xe8, 0xc4, 0xe6, 0x98, 0xdf, 0x4d, 0xeb, 0x0b, 0x96,
};

unsigned char g_n_s[20]={
0xdc, 0x33, 0xf7, 0x54, 0x10, 0xf0, 0x95, 0x82, 0x5e, 0x5c, 0xc0, 0xc4, 0x76, 0x42, 0xe3, 0x06, 
0x7e, 0xc4, 0xe6, 0x8d,
};
unsigned char g_user_input[]={
0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 
0x37, 0x38, 0x39, 0x30, 0x00 
};
#endif



/* handly bsafe bignum to openssl bignum routines */
#ifdef USE_BSAFE_UHSM

int try_bsafe_init_devicekey(int bsafe_fd)
{
	unsigned char auth_digest[20]={"12345678901234567890"};
	unsigned int uhsm_config = DC_ENABLE_USERKEY |  DC_AUTH_VALID | SC_EN_LOWFREQ |SC_EN_RSTMON|SC_EN_JMPMON|SC_EN_EXCPTMON | DC_USERAUTH_SHA1;
	unsigned int prod_config = 0x9c6eb875;
	unsigned short num_row_retry = 3;
	unsigned short num_col_retry =  3;
	unsigned char statistics[16];

	unsigned int bsafe_status=0;

	printf("CALLING: bsafe_init_devicekey \n");
                                

	bsafe_status = bsafe_init_devicekey(bsafe_fd, uhsm_config, (unsigned char *) &prod_config, auth_digest, num_row_retry, num_col_retry, statistics);

	return bsafe_status;

}

#if 1
/* Expected OPENSSL BN_BYTES is 4 */

void bsafeinit_set_bsafebn_endian()
{
	unsigned char one[8]={ 0x8, 0x7, 0x6, 0x5,0x04,0x03,0x02,0x01};
	BIGNUM *one_bn= BN_new();
	int bsafe_fd=-1;
	
	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("check_and_set_bsafebn_endian: bsafe_open failed %d\n", bsafe_fd);
		goto return_check_and_set_bsafebn_endian;
	}
	one_bn=BN_bin2bn(&one,sizeof(one),one_bn);
	try_bsafe_init_devicekey(bsafe_fd);
	bsafe_test_set_bsafe_bignum_endianess(bsafe_fd,(unsigned char *) one_bn->d);
return_check_and_set_bsafebn_endian:
	BN_free(one_bn);
	bsafe_close(bsafe_fd);
	return;
}

BIGNUM *BN_create_opensslbn(int len, BIGNUM *ret)
{
        unsigned int i;
        unsigned int n;
        n=len;
        if (ret == NULL) ret=BN_new();
        if (bn_expand(ret,(int)(n+2)*8) == NULL)
                return(NULL);
        i=((n-1)/BN_BYTES)+1;
        ret->top=i;
        ret->neg=0;
        return(ret);
}
BIGNUM *BN_fix_opensslbn_top(BIGNUM *ret)
{
        /* need to call this due to clear byte at top if avoiding
         * having the top bit set (-ve number) */
        bn_fix_top(ret);
        return(ret);
}


#endif
#endif


/* ---------------------------------------------------------------------- */
/* State handling */

typedef struct KDP_ReceiverState {
  time_t t_start;
  time_t t_timeout;
  M_KDPPolicy kek_priv_policy;
  M_KDPKey kek_priv;
} KDP_ReceiverState;

typedef struct KDP_AppKeyState {
  time_t t_timeout;
  M_KDPPolicy policy;
  M_KDPKey kapp;
} KDP_AppKeyState;

static KDP_ReceiverState *receiver_state[MAX_RECEIVER_STATES];
static KDP_AppKeyState *appkey_state[MAX_APPKEY_STATES];

/* Allocate a new KDP Key Receiver State. */
static M_Status alloc_receiver_state(KDP_SessionID *sessid)
{
  KDP_SessionID i;
  M_Status err;

  for (i = 0; i < MAX_RECEIVER_STATES; ++i)
    if (!receiver_state[i]) {
      em ( receiver_state[i] = NF_Malloc(sizeof *receiver_state[i], &ud) );
      FILLZERO(*receiver_state[i]);
      *sessid = i;
      return Status_OK;
    }

  err = Status_BufferFull;

x_err:
  return err;
}

/* Free a KDP Key Receiver State. */
void emulator_free_receiver_state(KDP_SessionID id)
{
#if !defined(USE_BSAFE_UHSM) || defined(DO_SW_ALSO)
  if (receiver_state[id]) {
    NF_FREE(KDPPolicy, &receiver_state[id]->kek_priv_policy, &ud);
    NF_FREE(KDPKey, &receiver_state[id]->kek_priv, &ud);
    FILLZERO(*receiver_state[id]);
    NF_Free(receiver_state[id], &ud);
    receiver_state[id] = 0;
  }
#endif
}

/* Allocate a new KDP Application Key State. */
static M_Status alloc_appkey_state(KDP_AppKeyID *appid)
{
  KDP_AppKeyID i;
  M_Status err;

  for (i = 0; i < MAX_APPKEY_STATES; ++i)
    if (!appkey_state[i]) {
      em ( appkey_state[i] = NF_Malloc(sizeof *appkey_state[i], &ud) );
      FILLZERO(*appkey_state[i]);
      *appid = i;
      return Status_OK;
    }

  err = Status_BufferFull;

x_err:
  return err;
}

/* Free a KDP Application Key State. */
void emulator_free_appkey_state(KDP_AppKeyID id)
{
  if (appkey_state[id]) {
    NF_FREE(KDPPolicy, &appkey_state[id]->policy, &ud);
    NF_FREE(KDPKey, &appkey_state[id]->kapp, &ud);
    FILLZERO(*appkey_state[id]);
    NF_Free(appkey_state[id], &ud);
    appkey_state[id] = 0;
  }
}

/* ---------------------------------------------------------------------- */
/* OpenSSL-related utility functions */

static M_Status sign_dsa(const unsigned char *pt, unsigned int ptlen,
                         const M_KDPKey *signer,
                         M_KDPBignum *r, M_KDPBignum *s)
{
  DSA *dsactx = NULL;
  EVP_MD_CTX *evpctx = NULL;
  M_Hash digest;
  DSA_SIG *dsasig = NULL;
  M_Status err = Status_OK;

#ifdef EMULATOR_DEBUG_DSA
  {
    int i;
    printf("pt = ");
    for (i = 0; i < ptlen; ++i)
      printf("%02X", pt[i]);
    printf("\n");
  }
#endif

  *r = NULL;
  *s = NULL;

  em ( dsactx = DSA_new() );
  assert(signer->type == KDPKeyType_DSAPrivate); /* checked by emu_init */
  em ( dsactx->p = BN_dup(signer->data.dsaprivate.p) );
  em ( dsactx->g = BN_dup(signer->data.dsaprivate.g) );
  em ( dsactx->q = BN_dup(signer->data.dsaprivate.q) );
  em ( dsactx->priv_key = BN_dup(signer->data.dsaprivate.x) );

	dumpbn("dsa p", dsactx->p);
	dumpbn("dsa g", dsactx->g);
	dumpbn("dsa q", dsactx->q);
	dumpbn("dsa priv_key", dsactx->priv_key);

  em ( evpctx = EVP_MD_CTX_create() );
  if (!EVP_DigestInit_ex(evpctx, EVP_dss1(), NULL))
    eg ( Status_Failed );
  assert(EVP_MD_CTX_size(evpctx) == 20); /* SHA-1 */
  if (!EVP_DigestUpdate(evpctx, pt, ptlen))
    eg ( Status_Failed );
  if (!EVP_DigestFinal_ex(evpctx, digest.bytes, NULL))
    eg ( Status_Failed );

	dumpArray("digest.bytes", digest.bytes,20);


#ifdef BSAFE_FIXED_KEYS
#if 0
/* pre calculated ...*/
unsigned char g_fixed_k[]={
0x44, 0x40, 0x25, 0x7c, 0x0f, 0xd5, 0x14, 0xc4, 0xbe, 0x37, 0x52, 0x2d, 0x5e, 0x45, 0x7e, 0x05,
0x2f, 0xe9, 0x95, 0xcd
};
unsigned char g_fixed_kinv[]={
0xab, 0xba, 0x80, 0xe5, 0x52, 0x69, 0x0a, 0x6b, 0x27, 0x1d, 0x94, 0x7e, 0xd2, 0x17, 0x44, 0x37,
0x63, 0x08, 0x3f, 0x71
};
unsigned char g_fixed_r[]={
0xa0, 0x52, 0x5a, 0xf9, 0xc1, 0x25, 0xaa, 0xd4, 0x80, 0x6b, 0x66, 0xd2, 0x06, 0xef, 0xe7, 0x9c,
0x8d, 0xd7, 0x16, 0x61
};
	dsactx->kinv = BSAFE_bin2bn(g_fixed_kinv,20, dsactx->kinv);
	dsactx->r = BSAFE_bin2bn(g_fixed_r,20, dsactx->r);
#endif
#endif


  dsasig = DSA_do_sign(digest.bytes, 20, dsactx);
  if (!dsasig)
    eg ( Status_Failed );

	dumpbn("dsa r ", dsasig->r);
	dumpbn("dsa s ", dsasig->s);

  em ( *r = BN_dup(dsasig->r) );
  em ( *s = BN_dup(dsasig->s) );

x_err:
  if (dsasig)
    DSA_SIG_free(dsasig);
  if (evpctx)
    EVP_MD_CTX_destroy(evpctx);
  if (dsactx)
    DSA_free(dsactx);
  if (err) {
    if (*r)
      BN_free(*r);
    if (*s)
      BN_free(*s);
  }

  return err;
}

/* ---------------------------------------------------------------------- */
/* Main entry points */

M_Status emulator_setup(const char *filename)
{
  FILE *nvfile = NULL;
  unsigned char *buf = NULL;
  int len;
  M_EmulatorFile nvram;
  M_Status err = Status_OK;



  FILLZERO(nvram);

  /* Initialize DLIES constants */
  eg ( DLIES_init() );


#ifdef USE_BSAFE_UHSM	/* done after DLIES_init */
	{
  	M_KDPKey kdc_pub_tmp, kdi_pub_tmp;	/* not used lazy to create new api */ 
	bsafeinit_set_bsafebn_endian();

	/* setup the globals kdc_pub and kdi_pub */
	if ((kdp_getpubkeys(&kdc_pub_tmp, &kdi_pub_tmp)) !=  0) {
		PRINTF("emulator_setup: kdp_getpubkeys failed \n");
		err = Status_Failed;
	}

#ifndef DO_SW_ALSO
	goto emulator_setup_return;
#endif
	}
#endif

  nvfile = fopen(filename, "rb");
  if (!nvfile)
    return Status_OSErrorErrno;

  em ( buf = NF_Malloc(NVRAM_SIZE, &ud) );
  len = fread(buf, sizeof *buf, NVRAM_SIZE, nvfile);
  if (len == 0)
    eg ( Status_HostDataInvalid );
  else if ( ferror(nvfile) )
    eg ( Status_OSErrorErrno );

  if (fclose(nvfile))
    eg ( Status_OSErrorErrno );
  nvfile = NULL;

  /* If the file is too large because the marshalled structure is bigger
   * than we expected, then the unmarshalling function will fail.
   */
  KDP_UNMARSHAL(EmulatorFile, buf, len, &nvram, &ud);

  if (nvram.magic != EmulatorFileMagic_EmulatorHeader ||
      nvram.type != EmulatorFileType_NVRAM)
    eg ( Status_HostDataInvalid );

  if (nvram.data.nvram.kdc_priv.type != KDPKeyType_DHPrivate2048Oakley ||
      nvram.data.nvram.kdc_pub.type != KDPKeyType_DHPublic2048Oakley ||
      nvram.data.nvram.kdi_priv.type != KDPKeyType_DSAPrivate ||
      nvram.data.nvram.kdi_pub.type != KDPKeyType_DSAPublic)
    eg ( Status_HostDataInvalid );

  NF_Free(buf, &ud);
  buf = NULL;
  KDP_COPY(KDPKey, &kdc_priv, &nvram.data.nvram.kdc_priv, buf, &ud);
  KDP_COPY(KDPKey, &kdc_pub, &nvram.data.nvram.kdc_pub, buf, &ud);
  KDP_COPY(KDPKey, &kdi_priv, &nvram.data.nvram.kdi_priv, buf, &ud);
  KDP_COPY(KDPKey, &kdi_pub, &nvram.data.nvram.kdi_pub, buf, &ud);


	print_keys();

x_err:
  if (nvfile)
    fclose(nvfile);
  if (buf)
    NF_Free(buf, &ud);
  NF_FREE(EmulatorFile, &nvram, &ud);
emulator_setup_return:
	print_keys();
  return err;
}

M_Status emulator_newkeys(const char *filename)
{
  DSA *dsa = NULL;
  DH *dh = NULL;
  FILE *nvfile = NULL;
  unsigned char *buf = NULL;
  int buflen;
  M_EmulatorFile nvram;
  M_Status err;


  FILLZERO(nvram);
  nvram.magic = EmulatorFileMagic_EmulatorHeader;
  nvram.type = EmulatorFileType_NVRAM;

  /* Initialize DLIES constants */
  eg ( DLIES_init() );

#ifdef USE_BSAFE_UHSM	/* done after dlies_init */
	{
  	M_KDPKey kdc_pub_tmp, kdi_pub_tmp;	/* not used lazy to create new api */ 
	bsafeinit_set_bsafebn_endian();

	err = Status_OK;
	/* setup the globals kdc_pub and kdi_pub */
	if ((kdp_getpubkeys(&kdc_pub_tmp, &kdi_pub_tmp)) !=  0) {
		PRINTF("emulator_newkeys: kdp_getpubkeys failed \n");
		err = Status_Failed;
	}

#ifndef DO_SW_ALSO
	goto emulator_newkeys_return;
#endif
	}
#endif

  /* Generate Kdi */
  em ( dsa = DSA_generate_parameters(1024, NULL, 0, NULL, NULL, NULL, NULL) );
#ifdef USE_BSAFE_FIXED_KEYS
	{
  	dsa->p = BSAFE_bin2bn(g_dsa_p,128, dsa->p );
  	dsa->g = BSAFE_bin2bn(g_dsa_g,128, dsa->g );
  	dsa->q = BSAFE_bin2bn(g_dsa_q,20, dsa->q );
  	dsa->priv_key = BSAFE_bin2bn(g_dsa_priv_key,20, dsa->priv_key );
	}
#endif
  if (!DSA_generate_key(dsa))
    eg ( NOMEM );


  nvram.data.nvram.kdi_priv.type = KDPKeyType_DSAPrivate;
  em ( nvram.data.nvram.kdi_priv.data.dsaprivate.p = BN_dup(dsa->p) );
  em ( nvram.data.nvram.kdi_priv.data.dsaprivate.q = BN_dup(dsa->q) );
  em ( nvram.data.nvram.kdi_priv.data.dsaprivate.g = BN_dup(dsa->g) );
  em ( nvram.data.nvram.kdi_priv.data.dsaprivate.x = BN_dup(dsa->priv_key) );

  nvram.data.nvram.kdi_pub.type = KDPKeyType_DSAPublic;
  em ( nvram.data.nvram.kdi_pub.data.dsapublic.p = BN_dup(dsa->p) );
  em ( nvram.data.nvram.kdi_pub.data.dsapublic.q = BN_dup(dsa->q) );
  em ( nvram.data.nvram.kdi_pub.data.dsapublic.g = BN_dup(dsa->g) );
  em ( nvram.data.nvram.kdi_pub.data.dsapublic.y = BN_dup(dsa->pub_key) );


  /* Generate Kdc */
  dh = DH_new();
  em ( dh->p = BN_dup(Poakley2048) );
  em ( dh->g = BN_dup(Goakley2048) );

#ifdef USE_BSAFE_FIXED_KEYS
	{
	dh->length = 32*8;		/* in bits */
  	dh->priv_key = BSAFE_bin2bn(g_dh_priv_key,32, dh->priv_key );
  	dh->p = BSAFE_bin2bn(g_dh_p,sizeof(g_dh_p), dh->p );	 /* 256 */
  	dh->g = BSAFE_bin2bn(g_dh_g,sizeof(g_dh_g), dh->g );	/* 4 */
	}
#endif
  if (!DH_generate_key(dh))
    eg ( NOMEM );

  nvram.data.nvram.kdc_priv.type = KDPKeyType_DHPrivate2048Oakley;
  em ( nvram.data.nvram.kdc_priv.data.dhprivate2048oakley.x =
       BN_dup(dh->priv_key) );

  nvram.data.nvram.kdc_pub.type = KDPKeyType_DHPublic2048Oakley;
  em ( nvram.data.nvram.kdc_pub.data.dhpublic2048oakley.gx =
       BN_dup(dh->pub_key) );

  KDP_MARSHAL(EmulatorFile, &nvram, buf, buflen, &ud);

  nvfile = fopen(filename, "wb");
  if (!nvfile)
    eg ( Status_OSErrorErrno );

  if (fwrite(buf, sizeof *buf, buflen, nvfile) < (unsigned) buflen)
    eg ( Status_OSErrorErrno );
  if (fclose(nvfile))
    eg ( Status_OSErrorErrno );
  nvfile = NULL;

  NF_Free(buf, &ud);
  buf = NULL;
  KDP_COPY(KDPKey, &kdc_priv, &nvram.data.nvram.kdc_priv, buf, &ud);
  KDP_COPY(KDPKey, &kdc_pub, &nvram.data.nvram.kdc_pub, buf, &ud);
  KDP_COPY(KDPKey, &kdi_priv, &nvram.data.nvram.kdi_priv, buf, &ud);
  KDP_COPY(KDPKey, &kdi_pub, &nvram.data.nvram.kdi_pub, buf, &ud);

x_err:
  if (dsa)
    DSA_free(dsa);
  if (dh)
    DH_free(dh);
  if (nvfile)
    fclose(nvfile);
  if (buf)
    NF_Free(buf, &ud);
  NF_FREE(EmulatorFile, &nvram, &ud);

emulator_newkeys_return:
	print_keys();
  return err;
}

static time_t earlier_timeout(time_t t1, time_t t2)
{
  if (t1 == 0)
    return t2;
  else if (t2 == 0)
    return t1;
  else if (t1 < t2)
    return t1;
  else
    return t2;
}

#define DBG_DUMPARRAY	ddumpArray
#define DBG_DUMPBN	bsafe_dumpbn
#define DBG_PRINTF	printf

M_Status kdp_start(const M_Nonce *n_s,
                    KDP_SessionID *sessionid, M_KDPKey *ksc_pub,
                    M_KDPBignum *sig_begin_r, M_KDPBignum *sig_begin_s)
{
  BN_CTX *bnctx;
  int receiver_id = -1;
  KDP_ReceiverState *receiver;
  M_KDPKey ksc_priv;
  M_DLIESMessage message;
  unsigned char *pt = NULL, *buf = NULL;
  int ptlen;
  M_Status err =Status_OK;





	PRINTF("kdp_start <<<<\n");


  FILLZERO(ksc_priv);
  FILLZERO(*ksc_pub);
  FILLZERO(*sig_begin_r);
  FILLZERO(*sig_begin_s);

#ifdef USE_BSAFE_UHSM
	{

	unsigned short sessionid_tmp = *sessionid;

	unsigned char ksc_pub_gx_tmp[256];
	BIGNUM * sig_begin_r_tmp_bn =NULL;
	BIGNUM * sig_begin_s_tmp_bn =NULL;
	int bsafe_fd =0;
	int bsafe_status =0;

	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("bsafe_uhsm_start: bsafe_open failed %d\n", bsafe_fd);
     		err= Status_Failed ;
		goto kdp_start_return;
	}



  	ksc_pub->type = KDPKeyType_DHPublic2048Oakley;
	sig_begin_r_tmp_bn =BN_create_opensslbn(20, sig_begin_r_tmp_bn);
	sig_begin_s_tmp_bn =BN_create_opensslbn(20, sig_begin_s_tmp_bn);

	if ((bsafe_status=bsafe_uhsm_start(bsafe_fd, (unsigned char *) n_s->bytes, &sessionid_tmp, 
			ksc_pub_gx_tmp, 
			(unsigned char *) sig_begin_r_tmp_bn->d, 
			(unsigned char *) sig_begin_s_tmp_bn->d)) != 0)  {
		PRINTF("bsafe_uhsm_start: failed %d\n", bsafe_status);
     		err= Status_Failed ;
	}
	bsafe_close(bsafe_fd);

	ksc_pub->data.dhpublic2048oakley.gx = BSAFE_bin2bn(ksc_pub_gx_tmp, 256, ksc_pub->data.dhpublic2048oakley.gx );
	BN_fix_opensslbn_top(sig_begin_r_tmp_bn);
	BN_fix_opensslbn_top(sig_begin_s_tmp_bn);


	/* set the return values */
	*sessionid = sessionid_tmp;
	*sig_begin_r = sig_begin_r_tmp_bn;
	*sig_begin_s = sig_begin_s_tmp_bn;


	DBG_DUMPARRAY("kdp_start hw:nonce", n_s->bytes,20);
	DBG_DUMPARRAY("kdp_start hw:ksc_pub->data.dhpublic2048oakley.gx->d", 
			ksc_pub->data.dhpublic2048oakley.gx->d, 256);
	DBG_DUMPARRAY("kdp_start hw:sig_begin_r->d", 
			(*sig_begin_r)->d, 20);
	DBG_DUMPARRAY("kdp_start hw:sig_begin_s->d", 
			(*sig_begin_s)->d, 20);
	DBG_PRINTF("kdp_start ret  sessionid %d >>>>\n",*sessionid);


#ifndef DO_SW_ALSO
	goto kdp_start_return;
#endif

	}
#endif /* USE_BSAFE_UHSM */


  /* The uHSM must:
   *
   *    1. Randomly choose a Bignum x such that (Goakley2K, Poakley2K, x)
   *       form a valid Key_DH_Priv Ksc_priv (2048-bit, type 0x00 0x3a).
   */
  em ( bnctx = BN_CTX_new() );

  ksc_priv.type = KDPKeyType_DHPrivate2048Oakley;
#ifdef USE_BSAFE_FIXED_KEYS
	{
  	ksc_priv.data.dhprivate2048oakley.x = BSAFE_bin2bn(g_kdp_start_priv_key,32, ksc_priv.data.dhprivate2048oakley.x );
	}
#else
  em ( ksc_priv.data.dhprivate2048oakley.x = BN_new() );
  if (!BN_rand(ksc_priv.data.dhprivate2048oakley.x, 2048, 1, 0))
    eg ( Status_Failed );
#endif

  /*    2. Create a new KDP Key Receiver State, with:
   *         * t_start = current time
   *         * t_timeout = none
   *         * Kek_priv = Ksc_priv
   *         * Policy for Kek_priv is of kind POLICY_KTK (0x02, Sec 6.3),
   *           with Intended Use KTK_USE_KEK_DECRYPT, and Sub-Algorithm
   *           KTK_SUBALG_DH_DLIESDES3.
   *
   *    3. Associate an identifier sessionid with this state, to identify it
   *       in later commands.
   */
  eg ( alloc_receiver_state(&receiver_id) );
  receiver = receiver_state[receiver_id];
  ASSERT(receiver);
  receiver->t_start = time(NULL);
  receiver->t_timeout = 0;
  KDP_COPY(KDPKey, &receiver->kek_priv, &ksc_priv, buf, &ud);
  receiver->kek_priv_policy.kind = KDPPolicyKind_KTK;
  receiver->kek_priv_policy.details.ktk.intendeduse = IntendedUse_KEKDecrypt;
  receiver->kek_priv_policy.details.ktk.subalgorithm =
    SubAlgorithm_DHDLIESDES3;
  receiver->kek_priv_policy.duration = 0;

  /*    4. Compute gx = (Goakley2K ^ x) mod Poakley2K so that (Goakley2K,
   *       Poakley2K, gx) form the corresponding Key_DH_Pub, Ksc_pub.
   */
  /* I'd use DH_compute_key, but its interface is silly. It's easier to do
   * the modexp myself.
   */
  ksc_pub->type = KDPKeyType_DHPublic2048Oakley;
  em ( ksc_pub->data.dhpublic2048oakley.gx = BN_new() );

	DBG_DUMPBN("sw: a",Goakley2048);
	DBG_DUMPBN("sw: p",ksc_priv.data.dhprivate2048oakley.x);
	DBG_DUMPBN("sw: m",Poakley2048);
  if (!BN_mod_exp(ksc_pub->data.dhpublic2048oakley.gx, Goakley2048,
                  ksc_priv.data.dhprivate2048oakley.x, Poakley2048, bnctx))
    eg ( Status_Failed );

  /*    5. Construct the "BeginAuth" message (see below), and sign it
   *       according to FIPS 186-2 with Kdi_priv.
   */
  message.type = DLIESMsgType_BeginAuth;
  message.data.beginauth.n_s = *n_s;
  KDP_COPY(KDPKey, &message.data.beginauth.ksc_pub, ksc_pub, buf, &ud);
  KDP_MARSHAL(DLIESMessage, &message, pt, ptlen, &ud);

	DBG_DUMPARRAY("sw: pt", pt, ptlen);
  eg ( sign_dsa(pt, (unsigned) ptlen, &kdi_priv, sig_begin_r, sig_begin_s) );

  /*    6. Return sessionid, Ksc_pub and the DSA signature from Kdi_priv.
   *       The DSA signature algorithm returns two bignums, r and s, less
   *       than 2^160. These values are returned in a format convenient for
   *       the implementation.
   */
  *sessionid = receiver_id;

x_err:
  if (err != Status_OK) {
    if (receiver_id != -1)
      emulator_free_receiver_state(receiver_id);
    NF_FREE(KDPKey, ksc_pub, &ud);
    NF_FREE(KDPBignum, sig_begin_r, &ud);
    NF_FREE(KDPBignum, sig_begin_s, &ud);
  }

  NF_FREE(KDPKey, &ksc_priv, &ud);
  NF_FREE(DLIESMessage, &message, &ud);
  if (pt)
    NF_Free(pt, &ud);
  if (buf)
    NF_Free(buf, &ud);
  if (bnctx)
    BN_CTX_free(bnctx);


kdp_start_return:
	DBG_DUMPARRAY("kdp_start sw:nonce", n_s->bytes,20);
	DBG_DUMPARRAY("kdp_start sw:ksc_pub->data.dhpublic2048oakley.gx->d", 
			ksc_pub->data.dhpublic2048oakley.gx->d, 256);
	DBG_DUMPARRAY("kdp_start sw:sig_begin_r->d", 
			(*sig_begin_r)->d, 20);
	DBG_DUMPARRAY("kdp_start sw:sig_begin_s->d", 
			(*sig_begin_s)->d, 20);
	PRINTF("kdp_start ret  sessionid %d >>>>\n",*sessionid);

  return err;
}

M_Status kdp_message(const KDP_SessionID *sessionids, int numsessionids,
                     const M_ByteBlock *message,
                     KDP_AppKeyID *appkeyid_r, KDP_SessionID *sessionid_r)
{
  /*    1. Start at the start of sessionid_array, and note the current time
   *       as the nominal start time t_nominalstart, and set
   *       t_nominaltimeout to none.
   */
  int i;
  time_t t_nominalstart = time(NULL), t_nominaltimeout = 0;
  M_ByteBlock ct, pt;
  M_DLIESMessage decoded;
  M_Status err;

	PRINTF("kdp_message <<<< numsessionids %d \t",numsessionids);
  		for (i = 0; i < numsessionids; ++i) {
			PRINTF("sessionids %d\t",sessionids[i]);
		}
	PRINTF("\nkdp_return ");


  FILLZERO(pt);
  FILLZERO(decoded);


#ifdef USE_BSAFE_UHSM
	{
	unsigned short sessionids_tmp[2];
	unsigned short appkeyid_r_tmp, sessionid_r_tmp;
	int bsafe_fd;
	int bsafe_status;
	err=  Status_OK;
	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("bsafe_uhsm_message: bsafe_open failed %d\n", bsafe_fd);
     		err = Status_Failed ;
		goto  kdp_message_return;
	}

	/* ncipher expects -1 as the KDC sessionid while bsafe has 1 */
  	for (i = 0; i < numsessionids; ++i) {
		sessionids_tmp[i] = ((sessionids[i]==-1)?BSAFE_KDC_S_LOC:sessionids[i]);
#ifdef DO_SW_ALSO
		if (sessionids[i] == 0)
			sessionids_tmp[i] = BSAFE_KSESSION_LOC;
		else if (sessionids[i] == 1)
			sessionids_tmp[i] = BSAFE_KEK3_LOC;
		else if (sessionids[i] == -1)
			sessionids_tmp[i] = BSAFE_KDC_S_LOC;
		else {
			sessionids_tmp[i]= sessionids[i];
			printf("UNKNOWN SESSION ID %d\n", sessionids[i]);
		}
#endif
	}
	PRINTF("kdp_message <<<< numsessionids_tmp %d \t",numsessionids);
  		for (i = 0; i < numsessionids; ++i) {
			PRINTF("sessionids %d\t",sessionids_tmp[i]);
		}

	ddumpArray("message->ptr", message->ptr, message->len);
	
	if ((bsafe_status=bsafe_uhsm_message(bsafe_fd, (unsigned short *) &sessionids_tmp, (unsigned short )numsessionids, 
			message->ptr , message->len, 
			((appkeyid_r == NULL)? NULL: &appkeyid_r_tmp),
			((sessionid_r == NULL)? NULL: &sessionid_r_tmp))) != 0) {
		PRINTF("bsafe_uhsm_message: failed %d\n", bsafe_status);
     		err= Status_Failed ;
	} else 

	bsafe_close(bsafe_fd);

	if (err != Status_Failed) {
		if (appkeyid_r != NULL)  {
			*appkeyid_r = (int) appkeyid_r_tmp;
			/* default status is Status_OK */
		}
		if (sessionid_r != NULL) {
			*sessionid_r = (int) sessionid_r_tmp;
			err = Status_Accepted;
			}
	}

#ifndef DO_SW_ALSO
	goto  kdp_message_return;
#endif

	}
#endif

  /*    2. Decode message as a DLIES ciphertext according to IEEE P1363 and
   *       Sec 8.2.
   */
  ct = *message;

  for (i = 0; i < numsessionids; ++i) {
    KDP_ReceiverState *receiver = 0;

    /*  3. Check the current entry in the sessionid_array: */
    if (sessionids[i] == -1) {
      /*   + If it is none, decrypt the DLIES ciphertext with Kdc_priv. This
       *     yields a plaintext byte string.
       */
	ddumpArray("message---DLIES_dec -1 ", ((M_ByteBlock *) &ct)->ptr, 
			((M_ByteBlock *)&ct)->len);
      eg ( DLIES_decrypt(&ai_3DES_SHA1, &kdc_priv, &ct, &pt) );
    }else {
      /*   + If it is not none:
       *     1. Verify that the current entry in sessionid_array refers to a
       *        valid KDP Key Receiver state. If not, return UnknownID.
       */
      if (sessionids[i] < 0 || sessionids[i] >= MAX_RECEIVER_STATES ||
          !receiver_state[sessionids[i]])
        eg ( Status_UnknownID );
      receiver = receiver_state[sessionids[i]];

      /*     2. Verify that Kek_priv is of type Key_DH_Priv. If not, return
       *        InvalidKeyType.
       */
      switch (receiver->kek_priv.type) {
        case KDPKeyType_DHPrivate2048:
        case KDPKeyType_DHPrivate1024Oakley:
        case KDPKeyType_DHPrivate2048Oakley:
          break;
        default:
          eg ( Status_InvalidKeyType );
      }

      /*     3. Verify that Kek_priv has policy kind POLICY_KTK with
       *        Intended Use KTK_USE_KEK_DECRYPT and Sub-Algorithm
       *        KTK_SUBALG_DH_DLIESDES3. If not, return AccessDenied.
       */
      if (receiver->kek_priv_policy.kind != KDPPolicyKind_KTK ||
          receiver->kek_priv_policy.details.ktk.intendeduse !=
              IntendedUse_KEKDecrypt ||
          receiver->kek_priv_policy.details.ktk.subalgorithm !=
              SubAlgorithm_DHDLIESDES3)
        eg ( Status_AccessDenied );

      /*     4. Verify that the timeout, if set, on Kek_priv has not
       *        expired. If it has, return TimeLimitExceeded.
       */
      if (receiver->t_timeout && receiver->t_timeout < time(NULL))
        eg ( Status_TimeLimitExceeded );

      /*     5. Update the nominal start time t_nominalstart to be the
       *        earlier of its previous value and the t_start from the Key
       *        Receiver State.
       */
      if (receiver->t_start < t_nominalstart)
        t_nominalstart = receiver->t_start;

      /*     6. Update t_nominaltimeout to be the earlier of its previous
       *        value and the t_timeout from the Key Receiver State state
       *        (see Timeouts, Sec 6.6).
       */
      t_nominaltimeout = earlier_timeout(t_nominaltimeout,
                                         receiver->t_timeout);

      /*     7. Decrypt the DLIES ciphertext with Kek_priv, yielding a
       *        plaintext byte string.
       */
	ddumpArray("message---DLIES_dec", ((M_ByteBlock *) &ct)->ptr, 
			((M_ByteBlock *)&ct)->len);
      eg ( DLIES_decrypt(&ai_3DES_SHA1, &receiver->kek_priv, &ct, &pt) );
    }

    NF_FREE(DLIESMessage, &decoded, &ud);
    KDP_UNMARSHAL(DLIESMessage, pt.ptr, pt.len, &decoded, &ud);

    /*  4. Check that the plaintext byte string is a valid "Key" or
     *     "CipherText" message. According to the plaintext, the uHSM acts
     *     as follows:
     */
    if (decoded.type == DLIESMsgType_Key) {
      M_Word t_new;

      if (i != numsessionids - 1)
        eg ( Status_UnexpectedlyShallowNesting );

      /*   + "Key":
       *     1. verify that the key type is recognised; if not return 
       *        InvalidKeyType.
       */
      if (NF_Lookup(decoded.data.key.key.type,
                    NF_KDPKeyType_enumtable) == NULL)
        eg ( Status_InvalidKeyType );

      /*     2. calculate a new timeout t_new which is the earlier of the
       *        timeout in the plaintext (measured from t_nominalstart) and
       *        t_nominaltimeout (see Timeouts, Sec 6.6).
       */
      if (decoded.data.key.policy.duration)
        t_new = earlier_timeout(
            t_nominalstart + decoded.data.key.policy.duration,
            t_nominaltimeout);
      else
        t_new = t_nominaltimeout;

      /*     3. Check the policy kind, which must be one of the following: */
      switch (decoded.data.key.policy.kind) {
        /*      # Host Application Key (POLICY_HOSTAPPKEY, Sec 6.2), or a
         *        recognised Vendor Defined policy kind (from the range 0xc0
         *        to 0xef, Sec 6.4):
         */
        /* no vendor-defined policy kinds supported here */
        case KDPPolicyKind_HostAppKey:
        {
          int appkey_id;
          KDP_AppKeyState *appkey;

          if (!appkeyid_r)
            eg ( Status_InvalidParameter );

          /*      1. For policy kind POLICY_KTK, or a Vendor Defined policy
           *         kind, verify that the policy details are valid for this
           *         policy kind; if not return InvalidMessage.
           */
          /* Since no vendor-defined policy kinds are supported here, this
           * is a no-op.
           */

          /*      2. create a new KDP Application Key:
           *         - policy kind and details copied from message plaintext
           *         - t_timeout set from t_new
           *         - Kapp copied from the plaintext
           */
          er ( alloc_appkey_state(&appkey_id) );
          appkey = appkey_state[appkey_id];
          ASSERT(appkey);
          STEAL(appkey->policy, decoded.data.key.policy);
          appkey->t_timeout = t_new;
          STEAL(appkey->kapp, decoded.data.key.key);

          /*      3. Return OK and an identifier referring
           *         to the new KDP Application Key.
           */
          *appkeyid_r = appkey_id;
		PRINTF("appkeyid %d ",*appkeyid_r);

          /* need to cleanup pt and decoded */
          err = Status_OK;
          goto x_err;
        }

        /*      # Key Transfer Key (POLICY_KTK, Sec 6.3): */
        case KDPPolicyKind_KTK:
        {
          int new_receiver_id;
          KDP_ReceiverState *new_receiver;

          if (!sessionid_r)
            eg ( Status_InvalidParameter );

          /*      1. create a new KDP Key Receiver State:
           *         - t_start set from the current value of t_nominalstart
           *         - t_timeout set from t_new
           *         - policy kind and details copied from the plaintext.
           *         - Kek_priv copied from Ktk_priv in the plaintext.
           */
          er ( alloc_receiver_state(&new_receiver_id) );
          new_receiver = receiver_state[new_receiver_id];
          ASSERT(new_receiver);
          new_receiver->t_start = t_nominalstart;
          new_receiver->t_timeout = t_new;
          STEAL(new_receiver->kek_priv_policy, decoded.data.key.policy);
          STEAL(new_receiver->kek_priv, decoded.data.key.key);

          /*      2. Return Accepted. */
          *sessionid_r = new_receiver_id;
		printf("sessionid %d ",*sessionid_r);

          /* need to cleanup pt and decoded */
          err = Status_Accepted;
          goto x_err;
        }

        /*      # Any other policy kind including POLICY_UNUSEABLE (0x00,
         *        Sec 6.1, or other policy not understood by the uHSM):
         */
        default:
          /*      1. Shouldn't be possible, KDP_Unmarshal_DLIESMessage()
           *         only returns non-zero for the above kinds. */
          eg ( Status_Failed );
      }
    } else if (decoded.type == DLIESMsgType_CipherText)
      /*   + "CipherText":
       *     1. Advance to the next entry in the sessionid_array. If there
       *        are no more entries, return UnexpectedlyDeepNesting.
       *
       *     2. Decode nested_ciphertext as a DLIES ciphertext according to
       *        P1363.
       *
       *     3. Select a key and decrypt the ciphertext according to the
       *        outer steps 3 and onwards, above.
       */
      ct = decoded.data.ciphertext.nested;
      /* continue round the loop if there are any remaining entries */
    else
      eg ( Status_UnknownParameter );

    NF_FREE(ByteBlock, &pt, &ud);
  }

  err = Status_UnexpectedlyDeepNesting;

x_err:
  NF_FREE(ByteBlock, &pt, &ud);
  NF_FREE(DLIESMessage, &decoded, &ud);

kdp_message_return:

	PRINTF(" >>>>\n");

  return err;
}

void ddumpArray(char * mesg, unsigned char * buf,int size) ;
void bsafe_dumpbn(char *mesg, BIGNUM * bn)
{
        {
        unsigned char tmp[1024];
        int size=0;
        size = BN_bn2bin( bn, tmp);
        ddumpArray(mesg,tmp,size);
        }
}

void ddumpArray(char * mesg, unsigned char * buf,int size) 
{
        int i=0;
        unsigned char * ptr;

        printf("\n****** %s %d bytes buf=%x ******\n",mesg,size, buf);
        #define MAX_DISPLAY        5000
        ptr = buf;
        for (i=0;i<((size<MAX_DISPLAY)?size:MAX_DISPLAY);i++) {
                printf("%02x ",(unsigned char) *ptr); ptr =ptr+1;
                if (!((i+1)%16))
                        printf("\n");
        }
        printf("\n");
}

void print_keys();

void print_keys()
{


#if 1
	{
	unsigned char buf[4028];
	int size=0;

#define BSAFE_bn2bin BN_bn2bin


#ifndef USE_BSAFE_UHSM
/* kdi_priv */	
	size = BSAFE_bn2bin( kdi_priv.data.dsaprivate.p, buf);
	dumpArray("kdi_priv.dsaprivate.p",buf,size);
	size = BSAFE_bn2bin( kdi_priv.data.dsaprivate.q, buf);
	dumpArray("kdi_priv.dsaprivate.q",buf,size);
	size = BSAFE_bn2bin( kdi_priv.data.dsaprivate.g,buf);
	dumpArray("kdi_priv.dsaprivate.g",buf,size);
	size = BSAFE_bn2bin( kdi_priv.data.dsaprivate.x,buf);
	dumpArray("kdi_priv.dsaprivate.x",buf,size);
#endif


/* kdi_pub */	
	size = BSAFE_bn2bin( kdi_pub.data.dsapublic.p, buf);
	dumpArray("kdi_pub.dsapublic.p",buf,size);
	size = BSAFE_bn2bin( kdi_pub.data.dsapublic.q, buf);
	dumpArray("kdi_pub.dsapublic.q",buf,size);
	size = BSAFE_bn2bin( kdi_pub.data.dsapublic.g, buf);
	dumpArray("kdi_pub.dsapublic.g",buf,size);
	size = BSAFE_bn2bin( kdi_pub.data.dsapublic.y, buf);
	dumpArray("kdi_pub.dsapublic.y",buf,size);

/* kdc_priv */	
#ifndef USE_BSAFE_UHSM
	size = BSAFE_bn2bin( kdc_priv.data.dhprivate2048oakley.x, buf);
	dumpArray("kdc_priv.dhprivate2048oakley.x",buf,size);
#endif
/* kdc_pub */	
	size = BSAFE_bn2bin( kdc_pub.data.dhpublic2048oakley.gx, buf);
	dumpArray("kdc_pub.dhpublic2048oakley.gx",buf,size);
	}
#endif
}

M_Status kdp_getpubkeys(M_KDPKey *kdc_pub_out, M_KDPKey *kdi_pub_out)
{
  /* The uHSM returns the values of its device public keys. This command is
   * useful for enrollment: the public keys can be extracted under
   * controlled conditions and installed in the Key Server, so that the Key
   * Server can identify the uHSM in the future.
   */
  unsigned char *buf = NULL;
  M_Status err = Status_OK;




#ifdef USE_BSAFE_UHSM
	{
	int bsafe_fd;
	int bsafe_status;
	
	bsafe_dsa_pub_t kdi_pub_struct_tmp;
	unsigned char p[128], g[128], q[20], y[128];
	unsigned char kdc_pub_gx_tmp[256];

	kdi_pub_struct_tmp.p = p;
	kdi_pub_struct_tmp.g = g;
	kdi_pub_struct_tmp.q = q;
	kdi_pub_struct_tmp.y = y;
	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("bsafe_uhsm_getpubkeys: bsafe_open failed %d\n", bsafe_fd);
		err = Status_Failed ;
		goto x_err;
	}

  	kdi_pub.type = KDPKeyType_DSAPublic;
  	//kdi_pub.data.dsapublic.p = BSAFE_bin2bn(g_dsa_p,sizeof(g_dsa_p), kdi_pub.data.dsapublic.p );
  	//kdi_pub.data.dsapublic.g = BSAFE_bin2bn(g_dsa_g,sizeof(g_dsa_g),kdi_pub.data.dsapublic.g );
  	//kdi_pub.data.dsapublic.q = BSAFE_bin2bn(g_dsa_q,sizeof(g_dsa_q), kdi_pub.data.dsapublic.q );

  	kdc_pub.type = KDPKeyType_DHPublic2048Oakley;
  	//kdc_pub.data.dhpublic2048oakley.p = BSAFE_bin2bn(g_dh_p,sizeof(g_dh_p), kdc_pub.data.dhpublic2048oakley.p );
  	//kdc_pub.data.dhpublic2048oakley.g = BSAFE_bin2bn(g_dh_g,sizeof(g_dh_g), kdc_pub.data.dhpublic2048oakley.g );


	if ((bsafe_status =bsafe_uhsm_getpubkeys(bsafe_fd, 
			kdc_pub_gx_tmp,
			&kdi_pub_struct_tmp)) != 0) {
		PRINTF("bsafe_uhsm_getpubkeys: failed %d\n", bsafe_status);
		err = Status_Failed;
	}

	kdi_pub.data.dsapublic.p = BSAFE_bin2bn(kdi_pub_struct_tmp.p, 128, kdi_pub.data.dsapublic.p);
	kdi_pub.data.dsapublic.g = BSAFE_bin2bn(kdi_pub_struct_tmp.g, 128, kdi_pub.data.dsapublic.g);
	kdi_pub.data.dsapublic.q = BSAFE_bin2bn(kdi_pub_struct_tmp.q, 20, kdi_pub.data.dsapublic.q);
	kdi_pub.data.dsapublic.y = BSAFE_bin2bn(kdi_pub_struct_tmp.y, 128, kdi_pub.data.dsapublic.y);
	kdc_pub.data.dhpublic2048oakley.gx = BSAFE_bin2bn(kdc_pub_gx_tmp, 256, kdc_pub.data.dhpublic2048oakley.gx);
	bsafe_close(bsafe_fd);

#ifndef DO_SW_ALSO
	if ( err == Status_Failed)
		goto x_err;
#endif
	
	}
#endif


  KDP_COPY(KDPKey, kdc_pub_out, &kdc_pub, buf, &ud);
  KDP_COPY(KDPKey, kdi_pub_out, &kdi_pub, buf, &ud);

x_err:
  if (buf)
    NF_Free(buf, &ud);
  return err;
}

M_Status kdp_userauth_sha1pp(const M_Nonce *n_s,
                             M_KDPBignum *sig_userauth_r,
                             M_KDPBignum *sig_userauth_s)
{
  char ppbuf[1024], *ptr;
  const char *pperr;
  M_DLIESMessage message;
  unsigned char *pt = NULL;
  int ptlen;
  M_Status err;

  FILLZERO(message);

#ifdef BSAFE_FIXED_KEYS
	memcpy(n_s->bytes,g_n_s,20);
#endif

#ifdef USE_BSAFE_UHSM
	{
	int bsafe_status;
	BIGNUM * r_bn = NULL;
	BIGNUM * s_bn = NULL;
	int bsafe_fd;
	err= Status_OK;
	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("bsafe_uhsm_userauth_sha1pp: bsafe_open failed %d\n", bsafe_fd);
     		err = Status_Failed ;
		goto kdp_userauth_sha1pp_return;
	}

	r_bn =BN_create_opensslbn(20, r_bn);
	s_bn =BN_create_opensslbn(20, s_bn);

/* input for this is given to the chip via the gpio pins */

	if ((bsafe_status =bsafe_uhsm_userauth_sha1pp(bsafe_fd, (unsigned char *) n_s->bytes, 
		(unsigned char *) r_bn->d,  
		(unsigned char *) s_bn->d)) != 0) {
		PRINTF("bsafe_uhsm_userauth_sha1pp: failed %d\n", bsafe_status);
     		err = Status_Failed ;
	}
	bsafe_close(bsafe_fd);

	BN_fix_opensslbn_top(r_bn);
	BN_fix_opensslbn_top(s_bn);

	*sig_userauth_r = r_bn;
	*sig_userauth_s= s_bn;

#ifndef DO_SW_ALSO
	goto kdp_userauth_sha1pp_return;
#endif

	}
#endif

  /* The uHSM must:
   *
   *    1. Request a passphrase from the user via a physically separate
   *       password/passphrase/PIN entry device.
   */
  /* Oh well, it's an emulator. */
#ifdef BSAFE_FIXED_KEYS
	memcpy(ppbuf,g_user_input,strlen(g_user_input)+1);
	printf("not using user input...\n");
#else
  pperr = nf_readpp("uHSM user passphrase: ", ppbuf, sizeof(ppbuf));
  if (pperr) {
    nf_report_moan("error reading passphrase: %s", pperr);
    eg ( Status_UserCancelled ); /* either accurate or close enough */
  }
#endif
  /* 
   * The emulator only accepts ASCII passphrases. If we were to accept 
   * general UTF-8 strings then we would need to ensure that ppbuf was 
   * encoded in the shortest-possible UTF-8 representation.
   */
  for (ptr = ppbuf; *ptr != 0; ptr++) {
    if ( !isascii((unsigned char)*ptr) )
      eg ( Status_MalformedPassphrase );
  }

  /*    2. Construct the "UserAuth" "SHA1-Passphrase" message (see below),
   *       and sign it according to FIPS 186-2 with Kdi_priv.
   */
  message.type = DLIESMsgType_UserAuth;
  message.data.userauth.n_a = *n_s;
  message.data.userauth.scheme = UserAuthScheme_SHA1Passphrase;


	dumpArray("userinput", ppbuf, strlen(ppbuf));
	dumpArray("nonce", n_s->bytes, 20);

  SHA1(ppbuf, strlen(ppbuf),
       message.data.userauth.cred.sha1passphrase.sha1passphrase.bytes);

	dumpArray("SHA(userinput)", message.data.userauth.cred.sha1passphrase.sha1passphrase.bytes, 20);

  KDP_MARSHAL(DLIESMessage, &message, pt, ptlen, &ud);
  eg ( sign_dsa(pt, (unsigned) ptlen, &kdi_priv,
                sig_userauth_r, sig_userauth_s) );

  /*    3. Return the DSA signature. */

x_err:
  if (err != Status_OK) {
    NF_FREE(KDPBignum, sig_userauth_r, &ud);
    NF_FREE(KDPBignum, sig_userauth_s, &ud);
  }

  NF_FREE(DLIESMessage, &message, &ud);
  if (pt)
    NF_Free(pt, &ud);

kdp_userauth_sha1pp_return:

  return Status_OK;
}

/* Simplistic key use commands. These do not define the AlgorithmDetails
 * structure mentioned in the specification; a complete implementation
 * should do this, and should have a single decrypt etc. command rather than
 * one for each mode.
 */

M_Status kdp_decrypt_rsaprivate(KDP_AppKeyID appkeyid,
                                const M_ByteBlock *ip, M_ByteBlock *op)
{
  KDP_AppKeyState *appkey;
  RSA *rsa = NULL;
  BN_CTX *bnctx = NULL;
  M_Status err = Status_OK;

	PRINTF("kdp_decrypt_rsaprivate appkeyid %d\n",appkeyid);
	ddumpArray("rsa_decrypt input ", ip->ptr, ip->len);
	
#ifdef USE_BSAFE_UHSM
	{
	int bsafe_status;
	int bsafe_fd;
	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("bsafe_uhsm_decrypt_rsaprivate: bsafe_open failed %d\n", bsafe_fd);
     		err =  Status_Failed ;
		goto x_err;
	}

  	em ( op->ptr = NF_Malloc(ip->len , &ud) );
  	op->len = ip->len;

	if ((bsafe_status =bsafe_uhsm_decrypt_rsaprivate(bsafe_fd, (unsigned short) appkeyid, 
			ip->ptr , ip->len, op->ptr )) != 0) {
		PRINTF("bsafe_uhsm_decrypt_rsaprivate: failed %d\n", bsafe_status);
		bsafe_close(bsafe_fd);
  		if (op->ptr) {
  			op->len = 0;
    			NF_Free(op->ptr, &ud);
		}
     		err = Status_Failed ;
	}
	bsafe_close(bsafe_fd);

	if (err == Status_OK) {
		ddumpArray("decrypt_rsaprivate output hw: ", op->ptr, op->len);
		bsafe_rfc2313_padding_remove(op->ptr, ip->len, &op->len);
	}

#ifdef DO_SW_ALSO
	ddumpArray("decrypt_rsaprivate output hw: ", op->ptr, op->len);
    	NF_Free(op->ptr, &ud);
#else
	goto x_err;
#endif
	}
#endif

  /* The uHSM must:
   *
   *    1. Verify that the appkey identifier corresponds to a currently
   *       loaded application key. If not, return the Invalid ID outcome.
   */
  if (appkeyid < 0 || appkeyid >= MAX_APPKEY_STATES || 
      !appkey_state[appkeyid])
    eg ( Status_UnknownID );
  appkey = appkey_state[appkeyid];

  /*    2. Verify that the associated policy for the key has kind Host
   *       Application Key (POLICY_HOSTAPPKEY, Sec 6.2), and that the
   *       details have ENCRYPT_DATA, DECRYPT_DATA, SIGN_DATA or VERIFY_DATA
   *       set (according to the command). If not, return the Access Denied
   *       outcome.
   */
  if (appkey->policy.kind != KDPPolicyKind_HostAppKey ||
      !(appkey->policy.details.hostappkey.perms &
        KDPPolicyPermissions_Decrypt))
    eg ( Status_AccessDenied );

  /*    3. Verify that the timeout, if set, on the application key has not
   *       expired. If it has, return the Timeout outcome.
   */
  if (appkey->t_timeout && appkey->t_timeout < time(NULL))
    eg ( Status_TimeLimitExceeded );

  /*    4. Verify that the algspec is appropriate for the key type and
   *       operation. If not, return the Invalid Key Type outcome.
   */
  switch (appkey->kapp.type) {
    case KDPKeyType_RSAPrivate1024:
    case KDPKeyType_RSAPrivate2048:
      break;
    default:
      eg ( Status_InvalidKeyType );
  }

  /*    5. Process the input data according to the key data and parameters
   *       specified in algspec.
   *
   *    6. If the processing is successful, return Success and the output
   *       data op.
   *
   *    7. Otherwise, return the Cryptographic Error outcome.
   */

  /* fill in basic RSA parameters */
  em ( rsa = RSA_new() );
  switch (appkey->kapp.type) {
    case KDPKeyType_RSAPrivate1024:
      em ( rsa->p = BN_dup(appkey->kapp.data.rsaprivate1024.p) );
      em ( rsa->q = BN_dup(appkey->kapp.data.rsaprivate1024.q) );
      em ( rsa->dmp1 = BN_dup(appkey->kapp.data.rsaprivate1024.dmp1) );
      em ( rsa->dmq1 = BN_dup(appkey->kapp.data.rsaprivate1024.dmq1) );
      em ( rsa->iqmp = BN_dup(appkey->kapp.data.rsaprivate1024.iqmp) );
      break;
    case KDPKeyType_RSAPrivate2048:
      em ( rsa->p = BN_dup(appkey->kapp.data.rsaprivate2048.p) );
      em ( rsa->q = BN_dup(appkey->kapp.data.rsaprivate2048.q) );
      em ( rsa->dmp1 = BN_dup(appkey->kapp.data.rsaprivate2048.dmp1) );
      em ( rsa->dmq1 = BN_dup(appkey->kapp.data.rsaprivate2048.dmq1) );
      em ( rsa->iqmp = BN_dup(appkey->kapp.data.rsaprivate2048.iqmp) );
      break;
    default:
      assert(!"bad key type should have been checked earlier");
  }

	bsafe_dumpbn("rsa->p", rsa->p);
	bsafe_dumpbn("rsa->q", rsa->q);
	bsafe_dumpbn("rsa->dmp1", rsa->dmp1);
	bsafe_dumpbn("rsa->dmq1", rsa->dmq1);
	bsafe_dumpbn("rsa->iqmp", rsa->iqmp);

  /* We don't have the RSA public exponent e to hand, which OpenSSL wants in
   * order to turn on RSA blinding. Since (a) its use of e appears to be
   * largely cosmetic and (b) this is an emulator running entirely on the
   * host anyway, get around this by turning off RSA blinding.
   *
   * Implementations in real hardware must enable RSA blinding.
   */
#if (OPENSSL_VERSION_NUMBER >= 0x009060afL && \
     OPENSSL_VERSION_NUMBER <= 0x00907000L) || \
    OPENSSL_VERSION_NUMBER >= 0x0090702L
  /* OpenSSL 0.9.6j and 0.9.7b introduced this flag. Previous versions left
   * it off by default.
   */
  rsa->flags |= RSA_FLAG_NO_BLINDING;
#endif /* OpenSSL >= 0.9.6j/0.9.7b */

  /* calculate n = p * q */
  em ( bnctx = BN_CTX_new() );
  em ( rsa->n = BN_new() );
  if (!BN_mul(rsa->n, rsa->p, rsa->q, bnctx))
    eg ( Status_DecryptFailed );

	bsafe_dumpbn("rsa->n", rsa->n);

  /* allocate enough space */
  op->len = RSA_size(rsa);
  em ( op->ptr = NF_Malloc(op->len, &ud) );

  /* do the actual decryption */
  if (RSA_private_decrypt(ip->len, ip->ptr, op->ptr,
                          rsa, RSA_PKCS1_PADDING) == -1)
    eg ( Status_DecryptFailed );


x_err:
	ddumpArray("rsa_decrypt output ", op->ptr, op->len);
  if (bnctx)
    BN_CTX_free(bnctx);
  if (rsa)
    RSA_free(rsa);
  if (err)
    NF_Free(op->ptr, &ud);

  return err;
}

/* Generic block cipher decryption function. */
static M_Status kdp_decrypt_genblock(M_KDPKeyType keytype,
                                     KDP_AppKeyID appkeyid,
                                     const unsigned char *iv,
                                     const M_ByteBlock *ip, M_ByteBlock *op)
{
  KDP_AppKeyState *appkey;
  const EVP_CIPHER *cipher = NULL;
  unsigned char *keybytes = NULL;
  int blocksize;
  EVP_CIPHER_CTX evpctx;
  int cleanupctx = 0;
  unsigned char *buf = NULL;
  int buflen;
  M_Status err = Status_OK;

	PRINTF("kdp_decrypt_genblock appkeyid %d\n",appkeyid);
	ddumpArray("genblock input ", ip->ptr, ip->len);

  FILLZERO(*op);
#ifdef USE_BSAFE_UHSM
  	if  (keytype !=  KDPKeyType_DES3)
		goto do_decrypt_genblock_sw;

	{
	int bsafe_status;
	int bsafe_fd;

  	FILLZERO(*op);
	if ((bsafe_fd = bsafe_open(BSAFE_DEVICE_NAME))< 0) {
		PRINTF("bsafe_uhsm_decrypt_des3: bsafe_open failed %d\n", bsafe_fd);
     		err = Status_Failed ;
		goto x_err;
	}

  	em ( op->ptr = NF_Malloc(ip->len , &ud) );
  	op->len = ip->len;

	if ((bsafe_status =bsafe_uhsm_decrypt_des3(bsafe_fd, (unsigned short) appkeyid, 
			(unsigned char *) iv,
			ip->ptr , ip->len, op->ptr  )) != 0) {
		PRINTF("bsafe_uhsm_decrypt_des3: failed %d\n", bsafe_status);
		bsafe_close(bsafe_fd);
  		if (op->ptr) {
  			op->len = 0;
    			NF_Free(op->ptr, &ud);
		}
     		err = Status_Failed ;
	}
	bsafe_close(bsafe_fd);

do_decrypt_genblock_sw:	
#ifdef DO_SW_ALSO
	ddumpArray("genblock output hw: ", op->ptr, op->len);
    	NF_Free(op->ptr, &ud);
#else
	goto x_err;
#endif

	}
#endif

  /* The uHSM must:
   *
   *    1. Verify that the appkey identifier corresponds to a currently
   *       loaded application key. If not, return the Invalid ID outcome.
   */
  if (appkeyid < 0 || appkeyid >= MAX_APPKEY_STATES || 
      !appkey_state[appkeyid])
    eg ( Status_UnknownID );
  appkey = appkey_state[appkeyid];

  /*    2. Verify that the associated policy for the key has kind Host
   *       Application Key (POLICY_HOSTAPPKEY, Sec 6.2), and that the
   *       details have ENCRYPT_DATA, DECRYPT_DATA, SIGN_DATA or VERIFY_DATA
   *       set (according to the command). If not, return the Access Denied
   *       outcome.
   */
  if (appkey->policy.kind != KDPPolicyKind_HostAppKey ||
      !(appkey->policy.details.hostappkey.perms &
        KDPPolicyPermissions_Decrypt))
    eg ( Status_AccessDenied );

  /*    3. Verify that the timeout, if set, on the application key has not
   *       expired. If it has, return the Timeout outcome.
   */
  if (appkey->t_timeout && appkey->t_timeout < time(NULL))
    eg ( Status_TimeLimitExceeded );

  /*    4. Verify that the algspec is appropriate for the key type and
   *       operation. If not, return the Invalid Key Type outcome.
   */
  if (appkey->kapp.type != keytype)
    eg ( Status_InvalidKeyType );

  /*    5. Process the input data according to the key data and parameters
   *       specified in algspec.
   *
   *    6. If the processing is successful, return Success and the output
   *       data op.
   *
   *    7. Otherwise, return the Cryptographic Error outcome.
   */

  switch (keytype) {
    case KDPKeyType_DES3:
	printf("cipher ...EVP_des_ede3_cbc()\n");
      cipher = EVP_des_ede3_cbc();
      keybytes = appkey->kapp.data.des3.k.bytes;
      break;
    case KDPKeyType_AES128:
	printf("cipher ...EVP_aes_128_cbc()\n");
      cipher = EVP_aes_128_cbc();
      keybytes = appkey->kapp.data.aes128.k.bytes;
      break;
    default:
      /* static function, only called with known key types */
      assert(!"unknown key type in kdp_decrypt_genblock");
  }

  blocksize = EVP_CIPHER_block_size(cipher);


  em ( buf = NF_Malloc(ip->len + blocksize, &ud) );

  EVP_CIPHER_CTX_init(&evpctx);
  cleanupctx = 1;
	ddumpArray("genblock input keybytes[buf] ", keybytes, 24);
	ddumpArray("genblock input iv[buf] ", iv, 8);
  if (!EVP_DecryptInit_ex(&evpctx, cipher, NULL, keybytes, iv))
    eg ( Status_DecryptFailed );
	ddumpArray("genblock input[buf] ", ip->ptr, ip->len);
  if (!EVP_DecryptUpdate(&evpctx, buf, &buflen, ip->ptr, ip->len))
    eg ( Status_DecryptFailed );
  em ( op->ptr = NF_Malloc(buflen + blocksize, &ud) );
  memcpy(op->ptr, buf, buflen);
	ddumpArray("genblock output[buf] ", buf, buflen);
  op->len = buflen;
  if (!EVP_DecryptFinal(&evpctx, buf, &buflen))
    eg ( Status_DecryptFailed );
  memcpy(op->ptr + op->len, buf, buflen);
	ddumpArray("genblock output[buf] ", buf, buflen);
  op->len += buflen;
x_err:
	ddumpArray("genblock output ", op->ptr, op->len);
  if (cleanupctx)
    EVP_CIPHER_CTX_cleanup(&evpctx);
  if (buf)
    NF_Free(buf, &ud);
  if (err)
    NF_FREE(ByteBlock, op, &ud);

  return err;
}

M_Status kdp_decrypt_des3(KDP_AppKeyID appkeyid, const unsigned char *iv,
                          const M_ByteBlock *ip, M_ByteBlock *op)
{
  return kdp_decrypt_genblock(KDPKeyType_DES3, appkeyid, iv, ip, op);
}

M_Status kdp_decrypt_aes128(KDP_AppKeyID appkeyid, const unsigned char *iv,
                            const M_ByteBlock *ip, M_ByteBlock *op)
{
  return kdp_decrypt_genblock(KDPKeyType_AES128, appkeyid, iv, ip, op);
}
