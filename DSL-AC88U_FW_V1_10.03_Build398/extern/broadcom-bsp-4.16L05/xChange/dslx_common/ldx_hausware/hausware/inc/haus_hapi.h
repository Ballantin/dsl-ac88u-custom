/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  haus_hapi.h
*
*  PURPOSE:
*
*     This file contains global definitions for
*
*  NOTES:
*
****************************************************************************/

#if !defined( HAUS_HAPI_H )
#define HAUS_HAPI_H

/* ---- Include Files ---------------------------------------------------- */

/* ---- Constants and Types ---------------------------------------------- */


/* HAL identifiers */
enum
{
   HAPIHAL0 = 0,
   HAPIHAL1,
   HAPIHAL2,
   HAPIHAL3,
   HAPIHAL4,
   HAPIHAL5,
   HAPIHAL6,
   HAPIHALMAX,
};



/* VHD identifiers */
enum
{
   HAPIVHD0 = 0x0050,
   HAPIVHD1,
   HAPIVHD2,
   HAPIVHD3,
   HAPIVHD4,
   HAPIVHD5,
   HAPIVHD6,
   HAPIVHD7,
   HAPIVHD8,
   HAPIVHD9,
   HAPIVHD10,
   HAPIVHD_HOST  = 0x00FE,  /* VHD handle reserved for the host application */
   HAPIVHD_INVALID = 0x00FF
};

/* In order to send a command to the host, it needs a reserved DSP number 
 * that is unique from the DSP. A DSP number of 0xFF will be reserved 
 * for the host since it is unlikely we shall have 256 DSPs talking to one 
 * host. If we do, we would have to reserve more than 8 bits in the handle
 * for the DSP number  */

#define HAPI_HOST_DSPNUM     0xFF

/*
** Note: HAPI_VhdHandleResult should be a signed type so that it can be
**       compared to HAPIFAILURE (which is a -ve number).
*/
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
*
****************************************************************************/

#endif /* HAUS_HAPI_H */

