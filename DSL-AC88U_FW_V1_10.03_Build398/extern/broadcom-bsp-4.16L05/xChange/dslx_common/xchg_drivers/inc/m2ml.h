/*****************************************************************************
*
*  Copyright (c) 2004 Broadcom, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    m2ml.h
*
*  @brief   Lower m2m dma device driver header file
*
*  This file contains the lower device driver header information for the
*  m2m dma module.
*/
/****************************************************************************/

#ifndef M2ML_H
#define M2ML_H


/* ---- Include Files ----------------------------------------------------- */


/* ---- Public Constants and Types ---------------------------------------- */


typedef struct
{

   unsigned int (*m2mlRxSrcAddrGetp)( void );
   int (*m2mlRxSrcAddrSetp)( unsigned int srcAddr );
   unsigned int (*m2mlRxDstAddrGetp)( void );
   int (*m2mlRxDstAddrSetp)( unsigned int dstAddr );
   unsigned int (*m2mlRxSizeGetp)( void );
   int (*m2mlRxSizeSetp)( unsigned int length );
   int (*m2mlRxByteSwapEnablep)( void );
   int (*m2mlRxByteSwapDisablep)( void );
   int (*m2mlRxByteSwapIsEnabledp)( void );
   int (*m2mlRxEndianEnablep)( void );
   int (*m2mlRxEndianDisablep)( void );
   int (*m2mlRxEndianIsEnabledp)( void );
   int (*m2mlRxPcmciaSwapEnablep)( void );
   int (*m2mlRxPcmciaSwapDisablep)( void );
   int (*m2mlRxPcmciaSwapIsEnabledp)( void );
   int (*m2mlRxNoIncrEnablep)( void );
   int (*m2mlRxNoIncrDisablep)( void );
   int (*m2mlRxNoIncrIsEnabledp)( void );
   int (*m2mlRxDoneFlagClrp)( void );
   int (*m2mlRxErrFlagClrp)( void );
   int (*m2mlRxDoneFlagGetp)( void );
   int (*m2mlRxErrFlagGetp)( void );
   int (*m2mlRxIrqEnablep)( void );
   int (*m2mlRxIrqDisablep)( void );
   int (*m2mlRxIrqIsEnabledp)( void );
   int (*m2mlRxEnablep)( void );
   int (*m2mlRxDisablep)( void );
   int (*m2mlRxIsEnabledp)( void );
   int (*m2mlRxSrcPortIdSetp)( unsigned int portId );
   unsigned int (*m2mlRxSrcPortIdGetp)( void );
   int (*m2mlRxDstPortIdSetp)( unsigned int portId );
   unsigned int (*m2mlRxDstPortIdGetp)( void );

   unsigned int (*m2mlTxSrcAddrGetp)( void );
   int (*m2mlTxSrcAddrSetp)( unsigned int srcAddr );
   unsigned int (*m2mlTxDstAddrGetp)( void );
   int (*m2mlTxDstAddrSetp)( unsigned int dstAddr );
   unsigned int (*m2mlTxSizeGetp)( void );
   int (*m2mlTxSizeSetp)( unsigned int length );
   int (*m2mlTxByteSwapEnablep)( void );
   int (*m2mlTxByteSwapDisablep)( void );
   int (*m2mlTxByteSwapIsEnabledp)( void );
   int (*m2mlTxEndianEnablep)( void );
   int (*m2mlTxEndianDisablep)( void );
   int (*m2mlTxEndianIsEnabledp)( void );
   int (*m2mlTxPcmciaSwapEnablep)( void );
   int (*m2mlTxPcmciaSwapDisablep)( void );
   int (*m2mlTxPcmciaSwapIsEnabledp)( void );
   int (*m2mlTxNoIncrEnablep)( void );
   int (*m2mlTxNoIncrDisablep)( void );
   int (*m2mlTxNoIncrIsEnabledp)( void );
   int (*m2mlTxDoneFlagClrp)( void );
   int (*m2mlTxErrFlagClrp)( void );
   int (*m2mlTxDoneFlagGetp)( void );
   int (*m2mlTxErrFlagGetp)( void );
   int (*m2mlTxIrqEnablep)( void );
   int (*m2mlTxIrqDisablep)( void );
   int (*m2mlTxIrqIsEnabledp)( void );
   int (*m2mlTxEnablep)( void );
   int (*m2mlTxDisablep)( void );
   int (*m2mlTxIsEnabledp)( void );
   int (*m2mlTxSrcPortIdSetp)( unsigned int portId );
   unsigned int (*m2mlTxSrcPortIdGetp)( void );
   int (*m2mlTxDstPortIdSetp)( unsigned int portId );
   unsigned int (*m2mlTxDstPortIdGetp)( void );
} M2ML_FUNCS;

#define m2mlRxSrcAddrGet( drvp )             (drvp)->funcp->m2mlRxSrcAddrGetp()
#define m2mlRxSrcAddrSet( drvp, srcAddr )    (drvp)->funcp->m2mlRxSrcAddrSetp( srcAddr )
#define m2mlRxDstAddrGet( drvp )             (drvp)->funcp->m2mlRxDstAddrGetp()
#define m2mlRxDstAddrSet( drvp, dstAddr )    (drvp)->funcp->m2mlRxDstAddrSetp( dstAddr )
#define m2mlRxSizeGet( drvp )                (drvp)->funcp->m2mlRxSizeGetp()
#define m2mlRxSizeSet( drvp, length )        (drvp)->funcp->m2mlRxSizeSetp( length )
#define m2mlRxByteSwapEnable( drvp )         (drvp)->funcp->m2mlRxByteSwapEnablep()
#define m2mlRxByteSwapDisable( drvp )        (drvp)->funcp->m2mlRxByteSwapDisablep()
#define m2mlRxByteSwapIsEnabled( drvp )      (drvp)->funcp->m2mlRxByteSwapIsEnabledp()
#define m2mlRxEndianEnable( drvp )           (drvp)->funcp->m2mlRxEndianEnablep()
#define m2mlRxEndianDisable( drvp )          (drvp)->funcp->m2mlRxEndianDisablep()
#define m2mlRxEndianIsEnabled( drvp )        (drvp)->funcp->m2mlRxEndianIsEnabledp()
#define m2mlRxPcmciaSwapEnable( drvp )       (drvp)->funcp->m2mlRxPcmciaSwapEnablep()
#define m2mlRxPcmciaSwapDisable( drvp )      (drvp)->funcp->m2mlRxPcmciaSwapDisablep()
#define m2mlRxPcmciaSwapIsEnabled( drvp )    (drvp)->funcp->m2mlRxPcmciaSwapIsEnabledp()
#define m2mlRxNoIncrEnable( drvp )           (drvp)->funcp->m2mlRxNoIncrEnablep()
#define m2mlRxNoIncrDisable( drvp )          (drvp)->funcp->m2mlRxNoIncrDisablep()
#define m2mlRxNoIncrIsEnabled( drvp )        (drvp)->funcp->m2mlRxNoIncrIsEnabledp()
#define m2mlRxDoneFlagClr( drvp )            (drvp)->funcp->m2mlRxDoneFlagClrp()
#define m2mlRxErrFlagClr( drvp )             (drvp)->funcp->m2mlRxErrFlagClrp()
#define m2mlRxDoneFlagGet( drvp )            (drvp)->funcp->m2mlRxDoneFlagGetp()
#define m2mlRxErrFlagGet( drvp )             (drvp)->funcp->m2mlRxErrFlagGetp()
#define m2mlRxIrqEnable( drvp )              (drvp)->funcp->m2mlRxIrqEnablep()
#define m2mlRxIrqDisable( drvp )             (drvp)->funcp->m2mlRxIrqDisablep()
#define m2mlRxIrqIsEnabled( drvp )           (drvp)->funcp->m2mlRxIrqIsEnabledp()
#define m2mlRxEnable( drvp )                 (drvp)->funcp->m2mlRxEnablep()
#define m2mlRxDisable( drvp )                (drvp)->funcp->m2mlRxDisablep()
#define m2mlRxIsEnabled( drvp )              (drvp)->funcp->m2mlRxIsEnabledp()
#define m2mlRxSrcPortIdSet( drvp, portId )   (drvp)->funcp->m2mlRxSrcPortIdSetp( portId )
#define m2mlRxSrcPortIdGet( drvp )           (drvp)->funcp->m2mlRxSrcPortIdGetp()
#define m2mlRxDstPortIdSet( drvp, portId )   (drvp)->funcp->m2mlRxDstPortIdSetp( portId )
#define m2mlRxDstPortIdGet( drvp )           (drvp)->funcp->m2mlRxDstPortIdGetp()

#define m2mlTxSrcAddrGet( drvp )             (drvp)->funcp->m2mlTxSrcAddrGetp()
#define m2mlTxSrcAddrSet( drvp, srcAddr )    (drvp)->funcp->m2mlTxSrcAddrSetp( srcAddr )
#define m2mlTxDstAddrGet( drvp )             (drvp)->funcp->m2mlTxDstAddrGetp()
#define m2mlTxDstAddrSet( drvp, dstAddr )    (drvp)->funcp->m2mlTxDstAddrSetp( dstAddr )
#define m2mlTxSizeGet( drvp )                (drvp)->funcp->m2mlTxSizeGetp()
#define m2mlTxSizeSet( drvp, length )        (drvp)->funcp->m2mlTxSizeSetp( length )
#define m2mlTxByteSwapEnable( drvp )         (drvp)->funcp->m2mlTxByteSwapEnablep()
#define m2mlTxByteSwapDisable( drvp )        (drvp)->funcp->m2mlTxByteSwapDisablep()
#define m2mlTxByteSwapIsEnabled( drvp )      (drvp)->funcp->m2mlTxByteSwapIsEnabledp()
#define m2mlTxEndianEnable( drvp )           (drvp)->funcp->m2mlTxEndianEnablep()
#define m2mlTxEndianDisable( drvp )          (drvp)->funcp->m2mlTxEndianDisablep()
#define m2mlTxEndianIsEnabled( drvp )        (drvp)->funcp->m2mlTxEndianIsEnabledp()
#define m2mlTxPcmciaSwapEnable( drvp )       (drvp)->funcp->m2mlTxPcmciaSwapEnablep()
#define m2mlTxPcmciaSwapDisable( drvp )      (drvp)->funcp->m2mlTxPcmciaSwapDisablep()
#define m2mlTxPcmciaSwapIsEnabled( drvp )    (drvp)->funcp->m2mlTxPcmciaSwapIsEnabledp()
#define m2mlTxNoIncrEnable( drvp )           (drvp)->funcp->m2mlTxNoIncrEnablep()
#define m2mlTxNoIncrDisable( drvp )          (drvp)->funcp->m2mlTxNoIncrDisablep()
#define m2mlTxNoIncrIsEnabled( drvp )        (drvp)->funcp->m2mlTxNoIncrIsEnabledp()
#define m2mlTxDoneFlagClr( drvp )            (drvp)->funcp->m2mlTxDoneFlagClrp()
#define m2mlTxErrFlagClr( drvp )             (drvp)->funcp->m2mlTxErrFlagClrp()
#define m2mlTxDoneFlagGet( drvp )            (drvp)->funcp->m2mlTxDoneFlagGetp()
#define m2mlTxErrFlagGet( drvp )             (drvp)->funcp->m2mlTxErrFlagGetp()
#define m2mlTxIrqEnable( drvp )              (drvp)->funcp->m2mlTxIrqEnablep()
#define m2mlTxIrqDisable( drvp )             (drvp)->funcp->m2mlTxIrqDisablep()
#define m2mlTxIrqIsEnabled( drvp )           (drvp)->funcp->m2mlTxIrqIsEnabledp()
#define m2mlTxEnable( drvp )                 (drvp)->funcp->m2mlTxEnablep()
#define m2mlTxDisable( drvp )                (drvp)->funcp->m2mlTxDisablep()
#define m2mlTxIsEnabled( drvp )              (drvp)->funcp->m2mlTxIsEnabledp()
#define m2mlTxSrcPortIdSet( drvp, portId )   (drvp)->funcp->m2mlTxSrcPortIdSetp( portId )
#define m2mlTxSrcPortIdGet( drvp )           (drvp)->funcp->m2mlTxSrcPortIdGetp()
#define m2mlTxDstPortIdSet( drvp, portId )   (drvp)->funcp->m2mlTxDstPortIdSetp( portId )
#define m2mlTxDstPortIdGet( drvp )           (drvp)->funcp->m2mlTxDstPortIdGetp()

typedef struct
{
   /* This must be the first structure member. */
   const M2ML_FUNCS *funcp;

} M2ML_DRV;


#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Variables -------------------------------------------------- */


/* ---- Public Function Prototypes ---------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif
