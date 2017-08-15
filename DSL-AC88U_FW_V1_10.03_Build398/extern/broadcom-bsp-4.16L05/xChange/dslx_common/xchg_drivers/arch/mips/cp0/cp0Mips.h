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
*    Filename: cp0Mips.h
*
****************************************************************************
*    Description:
*
*     Interface for MIPS-specific get and set functions for co-processor 0
*     (CP0) registers.
*
****************************************************************************/

#ifndef CP0_MIPS_H
#define CP0_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <mipsDefines.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** These are aliases for the set of common CP0 control registers that should
** be found on every MIPS processor.
**
** In addition, each processor will have chip-specific control registers.
** These can be accessed by calling cp0GetRegMips() and cp0SetRegMips() directly.
*/
#define cp0GetIndexReg()         cp0GetRegMips( C0_INDEX_NUM,     0 )
#define cp0GetRandomReg()        cp0GetRegMips( C0_RANDOM_NUM,    0 )
#define cp0GetEntryLo0Reg()      cp0GetRegMips( C0_ENTRYLO_0_NUM, 0 )
#define cp0GetEntryLo1Reg()      cp0GetRegMips( C0_ENTRYLO_1_NUM, 0 )
#define cp0GetContextReg()       cp0GetRegMips( C0_CONTEXT_NUM,   0 )
#define cp0GetPageMaskReg()      cp0GetRegMips( C0_PAGEMASK_NUM,  0 )
#define cp0GetWiredReg()         cp0GetRegMips( C0_WIRED_NUM,     0 )
#define cp0GetBadVAddrReg()      cp0GetRegMips( C0_BADVADDR_NUM,  0 )
#define cp0GetCountReg()         cp0GetRegMips( C0_COUNT_NUM,     0 )
#define cp0GetEntryHiReg()       cp0GetRegMips( C0_ENTRYHI_NUM,   0 )
#define cp0GetCompareReg()       cp0GetRegMips( C0_COMPARE_NUM,   0 )
#define cp0GetStatusReg()        cp0GetRegMips( C0_STATUS_NUM,    0 )
#define cp0GetCauseReg()         cp0GetRegMips( C0_CAUSE_NUM,     0 )
#define cp0GetEpcReg()           cp0GetRegMips( C0_EPC_NUM,       0 )
#define cp0GetPridReg()          cp0GetRegMips( C0_PRID_NUM,      0 )
#define cp0GetConfigReg()        cp0GetRegMips( C0_CONFIG_NUM,    0 )
#define cp0GetLLAddrReg()        cp0GetRegMips( C0_LLADDR_NUM,    0 )
#define cp0GetTagLoReg()         cp0GetRegMips( C0_TAGLO_NUM,     0 )
#define cp0GetTagHiReg()         cp0GetRegMips( C0_TAGHI_NUM,     0 )

#define cp0SetIndexReg(val)      cp0SetRegMips( C0_INDEX_NUM,     0, (val) )
#define cp0SetRandomReg(val)     cp0SetRegMips( C0_RANDOM_NUM,    0, (val) )
#define cp0SetEntryLo0Reg(val)   cp0SetRegMips( C0_ENTRYLO_0_NUM, 0, (val) )
#define cp0SetEntryLo1Reg(val)   cp0SetRegMips( C0_ENTRYLO_1_NUM, 0, (val) )
#define cp0SetContextReg(val)    cp0SetRegMips( C0_CONTEXT_NUM,   0, (val) )
#define cp0SetPageMaskReg(val)   cp0SetRegMips( C0_PAGEMASK_NUM,  0, (val) )
#define cp0SetWiredReg(val)      cp0SetRegMips( C0_WIRED_NUM,     0, (val) )
#define cp0SetBadVAddrReg(val)   cp0SetRegMips( C0_BADVADDR_NUM,  0, (val) )
#define cp0SetCountReg(val)      cp0SetRegMips( C0_COUNT_NUM,     0, (val) )
#define cp0SetEntryHiReg(val)    cp0SetRegMips( C0_ENTRYHI_NUM,   0, (val) )
#define cp0SetCompareReg(val)    cp0SetRegMips( C0_COMPARE_NUM,   0, (val) )
#define cp0SetStatusReg(val)     cp0SetRegMips( C0_STATUS_NUM,    0, (val) )
#define cp0SetCauseReg(val)      cp0SetRegMips( C0_CAUSE_NUM,     0, (val) )
#define cp0SetEpcReg(val)        cp0SetRegMips( C0_EPC_NUM,       0, (val) )
#define cp0SetPridReg(val)       cp0SetRegMips( C0_PRID_NUM,      0, (val) )
#define cp0SetConfigReg(val)     cp0SetRegMips( C0_CONFIG_NUM,    0, (val) )
#define cp0SetLLAddrReg(val)     cp0SetRegMips( C0_LLADDR_NUM,    0, (val) )
#define cp0SetTagLoReg(val)      cp0SetRegMips( C0_TAGLO_NUM,     0, (val) )
#define cp0SetTagHiReg(val)      cp0SetRegMips( C0_TAGHI_NUM,     0, (val) )


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   cp0GetRegMips
**
** PURPOSE:    Get specified MIPS co-processor 0 (CP0) register.
**
** PARAMETERS: reg   (in)  CP0 register to get.
**             sel   (in)  Register select number (default is 0).
**
** RETURNS:    Register value.
**
** NOTE:
*****************************************************************************
*/
unsigned int cp0GetRegMips( unsigned int reg, unsigned int sel );


/*
*****************************************************************************
** FUNCTION:   cp0GetRegMips
**
** PURPOSE:    Set value of specified MIPS co-processor 0 (CP0) register.
**
** PARAMETERS: reg   (in)  CP0 register to set.
**             sel   (in)  Register select number (default is 0).
**             value (in)  New register value.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void cp0SetRegMips( unsigned int reg, unsigned int sel, unsigned int value );



#ifdef __cplusplus
    }
#endif

#endif  /* CP0_MIPS_H  */
