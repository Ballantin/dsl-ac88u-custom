/****************************************************************************
*
*  dcm.h - Directed Cache Manager Definitions
*
*  PURPOSE:
*     This file contains definitions used by the assembly portion of DCM.
*
*  NOTES:
*
*              (c) Copyright 2000 Broadcom Canada Limited
*                          All Rights Reserved
*
****************************************************************************/

// Note that if you change these, then the code in dcmnest.s and dcmfault.s
// will need to be changed as well. Currently, the registers represented by
// REG_PARAM1 and REG_PARAM2 are not saved/restored.
//
// If these are changed then the current code which saves/restores the "new"
// registers would need to be removed, and the code which saves/restore the 
// "old" registers would need to be added.

#define  REG_PARAM1  %cb1_beg
#define  REG_PARAM2  %cb1_end


