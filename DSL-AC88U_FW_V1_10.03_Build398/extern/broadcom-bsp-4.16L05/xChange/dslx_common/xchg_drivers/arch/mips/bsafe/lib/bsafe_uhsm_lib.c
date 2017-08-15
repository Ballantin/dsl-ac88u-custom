/*
 * Bsafe uHSM library function .
 *
 * $Copyright Open Broadcom Corporation$      
 * $Id: bsafe_uhsm_lib.c,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <bsafe_osl.h>
#include <bsafe_io.h>

#include <bsafe_uhsm_lib.h>

#ifdef BSAFE_SOCKET_SIM

#undef open
#undef close
#undef ioctl
#define open(devname,flags,mode)	bsafe_sock_open(devname)
#define close	bsafe_sock_close
#define ioctl	bsafe_sock_ioctl

#else

int ioctl( int fd, int request, void *arg );

#endif	/* BSAFE_SOCKET_SIM */

#define RETURN(cmd) 	if  (cmd.param.returnCode != UHSM_SUCCESS) 	\
				return cmd.param.returnCode;		\
			return cmd.param.returnStatus;
	



int bsafe_open(char * devname) 
{
	return open(devname,O_RDWR,0);
}
int bsafe_close(int fd) 
{
	return close(fd);
}




/* 	API - assumptions 
 * 	- Expecting the caller to check the maximum limits, pass in the required  size buffers etc 
 * 	 (ie) bsafe api _DOES_NOT_ do any validations.
 */


/* --- uHSM Key Delivery Protocol implementation functions --- */


/* 	
 *	bsafe_uhsm_start 
 * 	Input:	challenge 
 * 	Output:	session id, ksc_pub, sig_begin_r, sig_begin_s
 */
int bsafe_uhsm_start(  int fd,
				unsigned char *challenge /* 20 bytes */, 
				unsigned short *sessionid,
				unsigned char *ksc_pub /* 256 bytes */ ,
				BSAFE_BIGNUM *sig_begin_r /* 20 bytes */, 
				BSAFE_BIGNUM *sig_begin_s /* 20 bytes */) 
{

	bsafe_dlies_gen_ks_io_t cmd;

	/* input */	
	cmd.challenge = challenge;

	cmd.kekLoc = BSAFE_KSESSION_LOC;

	/* output ptrs */	
	cmd.ks_cp = ksc_pub;
	cmd.msgSig_r = sig_begin_r;
	cmd.msgSig_s = sig_begin_s;


	ioctl(fd,BSAFE_UHSM_DLIES_GEN_KS_IOCTL,&cmd);

	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*sessionid = BSAFE_KSESSION_LOC;	/* location is used as id's */
	}

	RETURN(cmd);

}


/* 
 *	bsafe_uhsm_message.
 *	Input: sessionids, numsessionids, message, message_len
 * 	Ouput:	appkeyid_r, sessionid_r [ pass in NULL for the key which is not required ]
 *
 */
int bsafe_uhsm_message( int fd,
			unsigned short *sessionids  /* no of array elements == numsessionids; ie sessionids[numsessionids] */,
			unsigned short numsessionids /* 1 <= numsessionid <= 2 */,
			unsigned char *message,
			unsigned int message_len,
			unsigned short *appkeyid_r,
			unsigned short *sessionid_r)
{
	bsafe_dlies_msg_dec_io_t cmd;
	
	cmd.nestLevel =  numsessionids;
	cmd.decLocArray = sessionids;

	
	cmd.dlies_msg = message;
	cmd.dlies_msg_len = message_len;	

	if (appkeyid_r == NULL)
		cmd.akeyLoc = BSAFE_INVALID_LOC;
	if (sessionid_r == NULL)
		cmd.kekLoc = BSAFE_INVALID_LOC;
	
	/* setup flags so the driver gets free key locations */
	cmd.flags = BSAFE_UHSM_DRIVER_GET_KEY_LOC;

	ioctl(fd,BSAFE_UHSM_DLIES_MSG_DEC_IOCTL,&cmd);

	
	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		if (appkeyid_r != NULL)
			*appkeyid_r = cmd.akeyLoc;
		if (sessionid_r != NULL)
			*sessionid_r = cmd.kekLoc;
	}
	

	RETURN(cmd);
}


/* 	
 * 	bsafe_uhsm_getpubkeys
 * 	Output: kdc_pub_out, kdi_pub_out
 *
 */
int bsafe_uhsm_getpubkeys( int fd,
				unsigned char *kdc_pub_out /* 256 bytes */, 
				bsafe_dsa_pub_t *kdi_pub_out /* 404 bytes  */)
{
	bsafe_dev_kdc_public_io_t kdc_cmd;
	bsafe_dev_kdi_public_io_t kdi_cmd;

	/* tmp values not returned by kdc api */
	BSAFE_BIGNUM msgSig_r[20];
	BSAFE_BIGNUM msgSig_s[20];
	BYTE configuration[8];
	BYTE authorization[20];

	kdc_cmd.options = NONE_ON_GPIO;
	kdc_cmd.msgSig_r = msgSig_r;
	kdc_cmd.msgSig_s = msgSig_s;
	kdi_cmd.options = NONE_ON_GPIO;
	kdi_cmd.configuration = configuration;
	kdi_cmd.authorization = authorization;
	
	kdi_cmd.kdi_p.p = kdi_pub_out->p;
	kdi_cmd.kdi_p.g = kdi_pub_out->g;
	kdi_cmd.kdi_p.q = kdi_pub_out->q;
	kdi_cmd.kdi_p.y = kdi_pub_out->y;
	ioctl(fd,BSAFE_UHSM_DEV_KDI_PUBLIC_IOCTL,&kdi_cmd);

	kdc_cmd.kdc_cp = kdc_pub_out;
	ioctl(fd,BSAFE_UHSM_DEV_KDC_PUBLIC_IOCTL,&kdc_cmd);
	
	return kdc_cmd.param.returnStatus ;
}



/* 
 *	bsafe_uhsm_userauth_sha1pp
 *	Input: challenge
 *	Output: sig_userauth_r, sig_userauth_s
 */
int bsafe_uhsm_userauth_sha1pp( int fd,
				unsigned char *challenge /* 20 bytes */,
				BSAFE_BIGNUM *sig_userauth_r /* 20 bytes */, 
				BSAFE_BIGNUM *sig_userauth_s /* 20 bytes */)
{
	bsafe_userauth_io_t cmd;
	
	cmd.challenge = challenge;

	cmd.msgSig_r = sig_userauth_r;
	cmd.msgSig_s = sig_userauth_s;

	ioctl(fd,BSAFE_UHSM_USERAUTH_IOCTL,&cmd);

	RETURN(cmd);

}



/* 
 *	bsafe_uhsm_decrypt_rsaprivate
 *	Input: appkeyid, ip, p_len
 * 	Output: op
 */
int bsafe_uhsm_decrypt_rsaprivate( int fd,
				unsigned short appkeyid,
				unsigned char *ip , 
				unsigned int p_len,
				unsigned char *op /* len of op == p_len */ )
{
	bsafe_user_pke_io_t cmd;

	cmd.akekLoc = appkeyid;
	cmd.pkeMsgIn = ip;
	cmd.pkeMsg_len = p_len;
	cmd.pkeMsgOut = op;
	cmd.opCode = BSAFE_RSA_PRIVATE_KEY_CRT;
	
	ioctl(fd,BSAFE_UHSM_USER_PKE_IOCTL,&cmd);

	RETURN(cmd);
}


/* 
 *	bsafe_uhsm_decrypt_des3
 *	Input: appkeyid, iv,ip, p_len
 *	Output: op
 */
int bsafe_uhsm_decrypt_des3( int fd,
				unsigned short appkeyid,
				unsigned char *iv /* 8 bytes */,
				unsigned char *ip , 
				unsigned int p_len,
				unsigned char *op /* len of op == p_len */ )

{
	bsafe_user_3des_hmacsha1_io_t cmd;
	
	/* value not used returned by the kdp api */
	BYTE outDigest[20];
	cmd.outDigest = (BYTE *) outDigest;

	cmd.mode = BSAFE_MODE_DECRPT | BSAFE_MODE_HMAC_NULL | BSAFE_MODE_DES_CBC;

	cmd.akey3DesLoc = appkeyid;
	cmd.iv = iv;
	cmd.dataIn = ip;
	cmd.data_len = p_len;
	cmd.dataOut = op;

	ioctl(fd,BSAFE_UHSM_USER_3DES_HMACSHA1_IOCTL,&cmd);
	

	RETURN(cmd);
}


/* misc routines */

/* 
 *	bsafe_init_devicekey
 *	Input: uhsm_config, prod_config,auth_digest, num_row_retry, num_col_retry
 *	Output: 
 */
int bsafe_init_devicekey( int fd,
				unsigned int uhsm_config,
				unsigned char *prod_config,/* 4 bytes */
				unsigned char *auth_digest /* 20 bytes */,
				unsigned short num_row_retry , 
				unsigned short num_col_retry,
				unsigned char *statistics /* 16 bytes */
				)

{
	bsafe_init_devicekey_io_t cmd;

	

	cmd.uhsmConfig = uhsm_config;
	cmd.prodConfig = prod_config;
	cmd.authorization = auth_digest;
	cmd.numRowRetry = num_row_retry;
	cmd.numColRetry = num_col_retry;
	cmd.statistics = statistics;

	ioctl(fd,BSAFE_UHSM_INIT_DEVICEKEY_IOCTL,&cmd);

	RETURN(cmd);
}

/* 
 *	bsafe_init_devicekey
 *	Input: runtest
 *	Output: testresult
 */
int bsafe_selftest( int fd,
				unsigned int runtest,
				unsigned int *testresult 
				)

{
	bsafe_selftest_io_t cmd;
	

	cmd.runtest = runtest;
	
	ioctl(fd,BSAFE_UHSM_SELFTEST_IOCTL,&cmd);

//	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*testresult = cmd.result;
//	}

	RETURN(cmd);
}



/* 
 *	bsafe_kek_status
 *	Input: kekloc
 *	Output: keypolicy, keytype, ksize
 */
int bsafe_kek_status( int fd,
				unsigned short kekloc,
				unsigned char * keypolicy, /* 6 bytes */
				unsigned short *keytype,
				unsigned int *ksize
				)

{
	bsafe_kek_akey_status_io_t cmd;
	

	cmd.location = kekloc;
	cmd.keyPolicy = keypolicy;
	
	ioctl(fd,BSAFE_UHSM_KEK_STATUS_IOCTL,&cmd);

	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*keytype = cmd.keyType;
		*ksize = cmd.kSize;
	}


	RETURN(cmd);
}

/* 
 *	bsafe_akey_status
 *	Input: akeyloc
 *	Output: keypolicy, keytype, ksize
 */
int bsafe_akey_status( int fd,
				unsigned short akeyloc,
				unsigned char * keypolicy, /* 6 bytes */
				unsigned short *keytype,
				unsigned int *ksize
				)

{
	bsafe_kek_akey_status_io_t cmd;
	

	cmd.location = akeyloc;
	cmd.keyPolicy = keypolicy;
	
	ioctl(fd,BSAFE_UHSM_AKEY_STATUS_IOCTL,&cmd);

	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*keytype = cmd.keyType;
		*ksize = cmd.kSize;
	}


	RETURN(cmd);
}



/* 
 *	bsafe_clr_key
 *	Input: location, cache
 *	Output: ksize
 */
int bsafe_clr_key( int fd,
				unsigned short location,
				unsigned short cache,
				unsigned int *ksize
				)

{
	bsafe_clr_key_io_t cmd;
	

	cmd.location = location;
	cmd.cache = cache;
		
	ioctl(fd,BSAFE_UHSM_CLR_KEY_IOCTL,&cmd);

	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*ksize = cmd.kSize;
	}


	RETURN(cmd);
}


/* 
 *	bsafe_ld_userkey [akey]
 *	Input: akeylocation, keydata, keydata_len
 *	Output: ksize
 */
int bsafe_ld_userkey( int fd,
				unsigned short akeylocation,
				unsigned char * keydata,
				unsigned int keydata_len,
				unsigned int *ksize
				)

{
	bsafe_ld_userkey_io_t cmd;
	

	cmd.akeyLoc = akeylocation;
	cmd.keyData = keydata;
	cmd.keyData_len = keydata_len;
			
	ioctl(fd,BSAFE_UHSM_LD_USERKEY_IOCTL,&cmd);

	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*ksize = cmd.kSize;
	}

	RETURN(cmd);
}


/* 
 *	bsafe_ld_cfg 
 *	Input: keypolicy, keytype, km_pub, 
 *		dlies_msg, dlies_msg_len , authsig_r, authsig_s
 *	Output: 
 */
int bsafe_ld_cfg( int fd,
				unsigned char *keypolicy,
				unsigned short keytype,
				bsafe_dsa_pub_t *km_pub,	/* 404 bytes p g q y */
				unsigned char *dlies_msg,
				unsigned int dlies_msg_len,
				BSAFE_BIGNUM *authsig_r,
				BSAFE_BIGNUM *authsig_s
				)

{
	bsafe_ld_cfg_io_t cmd;
	

	cmd.keyPolicy = keypolicy;
	cmd.keyType = keytype;
	cmd.km_pub = km_pub;
	cmd.dlies_msg = dlies_msg;
	cmd.dlies_msg_len = dlies_msg_len;
	cmd.authSig_r = authsig_r;
	cmd.authSig_s = authsig_s;
			
	ioctl(fd,BSAFE_UHSM_LD_CFG_IOCTL,&cmd);

	RETURN(cmd);
}


/* 
 *	bsafe_fips_clr
 *	Input: 	clr_vector  , num_retry
 *	Output: 
 */
int bsafe_fips_clr( int fd , unsigned int clr_vector , unsigned short num_retry )

{
	bsafe_fips_clr_io_t cmd;
	
	cmd.clrVector = clr_vector;
	cmd.numRetry = num_retry;

	ioctl(fd,BSAFE_UHSM_FIPS_CLR_IOCTL,&cmd);

	RETURN(cmd);
}

/* 
 *	bsafe_user_status
 *	Input: 	
 *	Output: SALeventstatus
 */
int bsafe_user_status( int fd , unsigned int *SALeventstatus )
{
	bsafe_user_status_io_t cmd;
	

	ioctl(fd,BSAFE_UHSM_USER_STATUS_IOCTL,&cmd);

	if (cmd.param.returnStatus == BSAFE_SUCCESS ) {
		*SALeventstatus = cmd.salEventStatus;
	}

	RETURN(cmd);
}

/* 
 *	bsafe_user_nop
 *	Input: 	datain, data_len
 *	Output: dataout
 */
int bsafe_user_nop( int fd , unsigned char *datain,unsigned int data_len, unsigned char * dataout )
{
	bsafe_user_nop_io_t cmd;

	cmd.dataIn = datain;
	cmd.data_len = data_len;
	cmd.dataOut = dataout;
	
	ioctl(fd,BSAFE_UHSM_USER_NOP_IOCTL,&cmd);


	RETURN(cmd);
}


/* 
 *	bsafe_user_random
 *	Input: 	rngtype
 *	Output: random
 */
int bsafe_user_random( int fd , unsigned int rngtype, unsigned char * random /* 20 bytes */ )
{
	bsafe_user_random_io_t cmd;

	cmd.rngType = rngtype;
	cmd.random = random;
		
	ioctl(fd,BSAFE_UHSM_USER_RANDOM_IOCTL,&cmd);


	RETURN(cmd);
}


/* 
 *	bsafe_user_sha1
 *	Input: prevlen,mode, indigest, data, data_len
 *	Output: outdigest
 */
int bsafe_user_sha1( int fd,
				unsigned short prevlen,
				unsigned short mode, 
				unsigned char *indigest /* 20 bytes */,	
				unsigned char *data,
				unsigned int data_len,
				unsigned char *outdigest /* 20 bytes */)

{
	bsafe_user_sha1_io_t cmd;
	
	
	cmd.prevLen = prevlen;
	cmd.mode = mode;
	cmd.inDigest = (BYTE *) indigest;

	cmd.data = data;
	cmd.data_len = data_len;
	
	cmd.outDigest = (BYTE *) outdigest;


	ioctl(fd,BSAFE_UHSM_USER_SHA1_IOCTL,&cmd);
	

	RETURN(cmd);
}



/* le test set */
int bsafe_test_set_bsafe_bignum_endianess(int fd,
				BSAFE_BIGNUM * one	/* 8 bytes */)
{
	bsafe_bignum_prop_io_t cmd;

	if (one[0] == 0x01) /* huh  0x01020304 0x05060708 */
		ioctl(fd,BSAFE_LE_BIGNUM_IOCTL,&cmd);
	else if (one[7] == 0x01) /* huh 0x0807060504030201 */
		ioctl(fd,BSAFE_BN_2_BIGNUM_IOCTL,&cmd);
	else if (one[3] == 0x01) /* huh 0x04030201 0x05060708 */
		return BSAFE_SUCCESS;
	else 
		return -1;	/* basically unknown */

	return cmd.param.returnStatus;	/* no ocmd so return ioctl return */
}

/* 
 *	bsafe_rfc2313_padding_remove
 *	Input: buf, len, 
 *	Output:  out_len
 */
int bsafe_rfc2313_padding_remove( unsigned char * buf,
				unsigned int len,
				unsigned int *out_len)
{
	unsigned int i=0;

	
	if ( (buf[0]== 0x00) && (buf[1] == 0x02) ) {
		for(i=2; i < len; i++ ) {
			if (buf[i] == 0x00) {
				i++;	/* skip the 0x00 */
				*out_len = len - i;
				memmove(buf, buf+i, *out_len);
				return  *out_len;
				break;
			}
		}   
	}
	return -1;

}
