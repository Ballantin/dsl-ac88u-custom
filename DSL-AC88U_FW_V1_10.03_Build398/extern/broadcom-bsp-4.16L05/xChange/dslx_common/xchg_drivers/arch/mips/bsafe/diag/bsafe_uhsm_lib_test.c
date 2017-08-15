#include <stdio.h>
#include <bsafe_uhsm_lib.h>

int fd=0;

void dumpArray(char * mesg, unsigned char * buf,int size) ;
void dumpArray(char * mesg, unsigned char * buf,int size)
{
        int i=0;
        unsigned char * ptr;

        printf("\n****** %s %d bytes buf=%x ******\n",mesg,size, buf);
        #define MAX_DISPLAY        500
        ptr = buf;
        for (i=0;i<((size<MAX_DISPLAY)?size:MAX_DISPLAY);i++) {
                printf("%02x ",(unsigned char) *ptr); ptr =ptr+1;
                if (!((i+1)%16))
                        printf("\n");
        }
        printf("\n");
}



int test_bsafe_uhsm_start()
{
	unsigned char challenge[20]={"12345678901234567890"};
	unsigned short sessionid;
	unsigned char ksc_pub[256];
	unsigned char sig_begin_r[20];
	unsigned char sig_begin_s[20];

	int ret=0;

	printf("CALLING: bsafe_start \n");
	ret = bsafe_uhsm_start(fd, &challenge, 
			&sessionid,
			&ksc_pub,
			&sig_begin_r,
			&sig_begin_s);
	printf("bsafe_start %d\n", ret);

	dumpArray("ksc_pub", ksc_pub, 256);
	dumpArray("sig_begin_r", sig_begin_r, 20);
	dumpArray("sig_begin_s", sig_begin_s, 20);

}

int test_bsafe_uhsm_message(int nestLevel, unsigned short appkeyid, unsigned short sessionid)
{
	unsigned short dec_loc_array[2]={2,1};
	unsigned char message[24]={"123456781234567812345678"};
	
	
	printf("CALLING: bsafe_uhsm_message %d\n",nestLevel);
	
	if (appkeyid ==NULL)
		bsafe_uhsm_message(fd, &dec_loc_array,nestLevel, message, 24, NULL, &sessionid);
	else if (sessionid ==NULL)
		bsafe_uhsm_message(fd, &dec_loc_array,nestLevel, message, 24, &appkeyid, NULL);


}

int test_bsafe_uhsm_getpubkeys()
{
	bsafe_dsa_pub_t kdi_pub_out;
	unsigned char kdc_pub_out[256];
	unsigned char p[128], g[128], q[20], y[128];

	kdi_pub_out.p = p;
	kdi_pub_out.g = g;
	kdi_pub_out.q = q;
	kdi_pub_out.y = y;

	printf("CALLING: test_uhsm_getpubkeys \n");
	bsafe_uhsm_getpubkeys(fd, &kdc_pub_out, &kdi_pub_out);

	dumpArray("kdi_pub.p", kdi_pub_out.p, 128);
	dumpArray("kdi_pub.g", kdi_pub_out.g, 128);
	dumpArray("kdi_pub.q", kdi_pub_out.q, 20);
	dumpArray("kdi_pub.y", kdi_pub_out.y, 128);
	dumpArray("kdc_pub", kdc_pub_out, 256);

}


int test_bsafe_uhsm_userauth_sha1pp()
{
	unsigned char challenge[20]={"12345678901234567890"};
	unsigned char sig_begin_r[20];
	unsigned char sig_begin_s[20];

	printf("CALLING: test_bsafe_uhsm_userauth_sha1pp \n");
	bsafe_uhsm_userauth_sha1pp(fd, &challenge, 
			&sig_begin_r,
			&sig_begin_s);

}

int test_bsafe_uhsm_decrypt_rsaprivate()
{
	unsigned short appkeyid=5;

	unsigned char ip[24]={"123456781234567812345678"};
	unsigned char op[24];

	printf("CALLING: test_bsafe_uhsm_decrypt_rsaprivate \n");
	bsafe_uhsm_decrypt_rsaprivate(fd, appkeyid,
				ip , 24, op);
}

int test_bsafe_uhsm_decrypt_des3()
{
	unsigned short appkeyid=5;

	unsigned char iv[8]={"cafebabe"};
	unsigned char ip[24]={"123456781234567812345678"};
	unsigned char op[24];
	

	printf("CALLING: bsafe_uhsm_decrypt_des3 \n");


	bsafe_uhsm_decrypt_des3(fd, appkeyid, iv,
				ip , 24, op);
}


int test_bsafe_init_devicekey()
{
	unsigned char auth_digest[20]={"12345678901234567890"};
	unsigned int uhsm_config = DC_ENABLE_USERKEY |  DC_AUTH_VALID | SC_EN_LOWFREQ |SC_EN_RSTMON|SC_EN_JMPMON|SC_EN_EXCPTMON;
	unsigned int prod_config = 0x9c6eb875;
	unsigned short num_row_retry =	3;
	unsigned short num_col_retry =	3;
	unsigned char statistics[4];

	printf("CALLING: bsafe_uhsm_init_devicekey \n");


	bsafe_init_devicekey(fd, uhsm_config, (unsigned char *) &prod_config, auth_digest, num_row_retry, num_col_retry, & statistics);
}

int test_bsafe_selftest()
{
	unsigned int runtest=0x0;
	unsigned int result;

	runtest = RAM_BIST_ROM_BIST_TEST | NVM_CHECKSUM_VERIFY_TEST | HMAC_SHA1_ENGINE_TEST | DES3_ENGINE_TEST | FIPS186_2_PSEUDO_RANDOM_TEST | DSA_SIGN_VERIFY_TEST | DH_TEST | RSA_ENC_DEC_TEST | DSA_SIGN_VERFIY_KDI_TEST| FIPS140_2_RANDOM_NUM_TEST | RANDOM_NUM_STAT_TEST ;

	bsafe_selftest(fd,runtest, &result);
}


int test_bsafe_kek_akey_status()
{
	unsigned short location =3;
	unsigned char keypolicy[6];
	unsigned int ksize;
	unsigned short keytype;


	bsafe_kek_status(fd, location, &keypolicy, &keytype, &ksize);
	bsafe_akey_status(fd, location, &keypolicy, &keytype, &ksize);
}

int test_bsafe_clr_key()
{
	unsigned short location =3;
	unsigned short cachetype =BSAFE_KEK_CACHE;
	unsigned int ksize;


	bsafe_clr_key(fd, location, cachetype, &ksize);
	cachetype =BSAFE_AKEY_CACHE;
	bsafe_clr_key(fd, location, cachetype, &ksize);

}

int test_bsafe_ld_userkey()
{
	unsigned short akeylocation =3;
	unsigned int ksize;
	unsigned char keydata[32];


	bsafe_ld_userkey(fd, akeylocation, keydata, 32, &ksize);
	
}

int test_bsafe_ld_cfg()
{
	unsigned char keypolicy[6]={0x1,0x2,0x3,0x4,0x5,0x6};
	unsigned short keyType = 0x16;
	bsafe_dsa_pub_t km_pub;
	unsigned char dlies_msg[24]={"123456789012345678901234"};
	unsigned char authsig_r[20]={"12345123451234512345"};
	unsigned char authsig_s[20]={"67890678906789067890"};

	bsafe_ld_cfg(fd,
				keypolicy,
				keyType,
				&km_pub,	/* 404 bytes p g q y */
				dlies_msg,
				24,
				authsig_r,
				authsig_s
				);
				
}


int test_bsafe_fips_clr()
{
	bsafe_fips_clr(fd, BSAFE_FIPS_CLR_VECTOR, 24);
}

int test_bsafe_user_status()
{
	unsigned int SALeventstatus;

	bsafe_user_status(fd, &SALeventstatus);
}

int test_bsafe_user_nop()
{

	unsigned char datain[24] = {"123456789012345678901234"};
	unsigned char dataout[24];

	bsafe_user_nop(fd , datain, 24, dataout );

}

int test_bsafe_user_random()
{
	unsigned int rngtype = BSAFE_USER_RANDOM_RNGTYPE_K;
	unsigned char random[20];

	bsafe_user_random( fd , rngtype, random );
}


int test_bsafe_sha1()
{

	unsigned short prevlen=10;
	unsigned short 	mode = BSAFE_MODE_SHA1_FINISH;
	unsigned char indigest[20] = {"12345678901234567890"};
	unsigned char outdigest[20];
	unsigned char data[24]={"123456789012345678901234"};
		
	bsafe_user_sha1(fd,
				prevlen,
				mode, 
				indigest /* 20 bytes */,	
				data,
				24,
				outdigest);
}

#if 1
int bsafe_le_test()
{
#if 1
	unsigned char one[8] ={0x1,0x2,0x3,0x4, 0x5, 0x6, 0x7, 0x8 };	/* for le */
#else
	unsigned char one[8] ={0x8,0x7,0x6,0x5, 0x4, 0x3, 0x2, 0x1 };	/* for bn */
#endif
	bsafe_test_set_bsafe_bignum_endianess(fd,
			 &one);
}
#endif

int main (int argc, char *argv[]) 
{
	if (argc < 2) {
		printf("%s: <ipaddr> <1-if init_devicekey is req>\n",argv[0]);
		return 0;
	}

	fd= bsafe_open(argv[1]);

	printf("fd %d\n",fd);

#if 0
	bsafe_le_test();
#endif

	if (argc == 3)
		test_bsafe_init_devicekey();

	test_bsafe_uhsm_getpubkeys();

	test_bsafe_uhsm_start();	
#if 0
	
	/* 10 is just junk so that it is not null */
	test_bsafe_uhsm_message(2,NULL,10);
	test_bsafe_uhsm_message(2,10,NULL);
	test_bsafe_uhsm_message(1,10,NULL);
#endif
#if 0
	

	test_bsafe_uhsm_userauth_sha1pp();

	test_bsafe_uhsm_decrypt_rsaprivate();


	test_bsafe_uhsm_decrypt_des3();



	test_bsafe_kek_akey_status();


	test_bsafe_selftest();

	test_bsafe_clr_key();


	test_bsafe_ld_userkey();


	test_bsafe_ld_cfg();


	test_bsafe_fips_clr();

	test_bsafe_user_status();

	test_bsafe_user_nop();

	test_bsafe_user_random();

	test_bsafe_sha1();
#endif

	bsafe_close(fd);

}
