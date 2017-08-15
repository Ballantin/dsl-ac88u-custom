/*
 * $Copyright Open Broadcom$
 *
 * $Id: bsafe_version.h.in,v 1.1.1.1 2004/03/17 01:07:36 gigis Exp $
 *
*/

#ifndef _bsafe_version_h_
#define _bsafe_version_h_


/* Vendor Name, ASCII, 32 chars max */
#ifdef COMPANYNAME
#define	BSAFE_VENDOR 		COMPANYNAME
#else
#define	BSAFE_VENDOR 		Broadcom"
#endif

/* Driver Date, ASCII, 32 chars max */
#define BSAFE_DRV_BUILD_DATE	__DATE__

/* Hardware Manufacture Date, ASCII, 32 chars max */
#define BSAFE_HW_MFG_DATE	"Not Specified"



#define	BSAFE_MAJOR_VERSION	1

#define	BSAFE_MINOR_VERSION	0

#define	BSAFE_RC_NUMBER		0

#define	BSAFE_INCREMENTAL_NUMBER	0

#define	BSAFE_BUILD_NUMBER	0

#define	BSAFE_VERSION		1,0,0,0

#define	BSAFE_VERSION_NUM		0x01000000

/* Driver Version String, ASCII, 32 chars max */
#ifdef BCMINTERNAL
#define	BSAFE_VERSION_STR		"1.0.0.0 (BROADCOM INTERNAL)"
#define	BSAFE_ROUTER_VERSION_STR	"1.1.0.0 (BROADCOM INTERNAL)"
#else
#define	BSAFE_VERSION_STR		"1.0.0.0"
#define	BSAFE_ROUTER_VERSION_STR	"1.1.0.0"
#endif

#endif /* _bsafe_version_h_ */
