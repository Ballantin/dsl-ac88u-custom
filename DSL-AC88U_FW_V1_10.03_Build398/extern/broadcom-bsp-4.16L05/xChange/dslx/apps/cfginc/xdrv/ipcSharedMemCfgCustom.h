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
#if !defined( IPC_SHARED_MEM_CFG_CUSTOM_H )
#define IPC_SHARED_MEM_CFG_CUSTOM_H

/* ---- Include Files ---------------------------------------------------- */

#include <globalCfg.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** One IPC channel per VHD + one for the resource manager.
** Here we set a global configuration which is applied for all DSLX apps. It has been
** sized to accomodate the worst case app type.
*/
#define MAXIPC_PER_DSP 18
#define IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS (MAXIPC_PER_DSP + 1)*VRG_GLOBAL_CFG_MAX_NUM_DSP

#endif /* IPC_SHARED_MEM_CFG_CUSTOM_H */
