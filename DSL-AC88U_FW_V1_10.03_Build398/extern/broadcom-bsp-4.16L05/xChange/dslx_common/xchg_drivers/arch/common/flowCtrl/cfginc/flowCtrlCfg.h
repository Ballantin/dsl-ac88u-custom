/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: flowCtrlMemCfg.h
*
****************************************************************************
*    Description:
*
*     Build configuration options for flow control module.
*
****************************************************************************/

#ifndef FLOW_CTRL_CFG_H
#define FLOW_CTRL_CFG_H

#ifdef __cplusplus
extern "C" {
#endif


/* ---- Include Files ---------------------------------------------------- */

#include <flowCtrlCfgCustom.h>


/* ---- Constants and Types ---------------------------------------------- */


#ifndef FLOWCTRL_CFG_BUFFER_SIZE_WORDS
#define FLOWCTRL_CFG_BUFFER_SIZE_WORDS       225
#endif

#ifndef FLOWCTRL_CFG_NUM_BUFFER
#define FLOWCTRL_CFG_NUM_BUFFER              30
#endif


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* FLOW_CTRL_CFG_H  */
