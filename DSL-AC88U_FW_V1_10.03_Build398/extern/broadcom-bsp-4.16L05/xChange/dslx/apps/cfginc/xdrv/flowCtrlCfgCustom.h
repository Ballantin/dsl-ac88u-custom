/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
#if !defined( FLOW_CTRL_CFG_CUSTOM_H )
#define FLOW_CTRL_CFG_CUSTOM_H

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** Largest packet size is G.711, 30ms  = 120 words
** xChange header                      =   5 words
** HPI header                          =   9 words
** HAL header                          =   1 words
**                                       ---------
**                                       135 words
*/
#define FLOWCTRL_CFG_BUFFER_SIZE_WORDS    255

/*
** Assume worst case 300ms jitter, and 10ms packets. 300/10 = 30 packets.
*/
#define FLOWCTRL_CFG_NUM_BUFFER           60


#endif /* FLOW_CTRL_CFG_CUSTOM_H */
