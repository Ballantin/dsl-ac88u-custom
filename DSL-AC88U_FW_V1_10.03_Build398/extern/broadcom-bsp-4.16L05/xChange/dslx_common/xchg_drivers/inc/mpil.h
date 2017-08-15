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
*  @file    mpil.h
*
*  @brief   Lower MPI device driver header file
*
*  This file contains the lower device driver header information for the
*  MPI module.
*/
/****************************************************************************/

#ifndef MPIL_H
#define MPIL_H


/* ---- Include Files ----------------------------------------------------- */


/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @brief   MPI Card Type
*
*  The MPI Card Type enumeration is used to specify the card type that is 
*  connected to the MPI slot.
*     @li None - no card detected
*     @li PCMCIA - PCMCIA Card detected
*     @li CardBus - CardBus Card detected
*/
typedef enum
{
   MPIL_CARDTYPE_NONE = 0,       /**< no card detected */
   MPIL_CARDTYPE_PCMCIA,         /**< PCMCIA card detected */
   MPIL_CARDTYPE_CARDBUS,        /**< CardBus card detected */
} MPIL_CARDTYPE;

/**
*  @brief   MPI Card Vcc
*
*  The MPI Card Vcc enumeration is used to specify the card Vcc that is 
*  detected from the MPI slot.
*     @li 00 - 0 V
*     @li 3.3 - 3.3 V
*     @li 5.0 - 5.0 V
*/
typedef enum
{
   MPIL_CARDVCC_00 = 0,          /**< Vcc = 0 V */
   MPIL_CARDVCC_33,              /**< Vcc = 3.3 V */
   MPIL_CARDVCC_50,              /**< Vcc = 5.0 V */
} MPIL_CARDVCC;

/**
*  @brief   MPI Bus mapping index
*
*  The MPI mapping index enumeration is used to index the three sets of system
*  bus to pci bus mapping registers, and first two sets of pci to system bus
*  maping reigister
*     @li Mem1 - Memory Space 1
*     @li Mem2 - Memory Space 2 
*     @li IO - IO Space
*/
typedef enum
{
   MPIL_MEM1 = 0,                /**< Memory Space 1 */
   MPIL_MEM2,                    /**< Memory Space 2 */
   MPIL_IO,                      /**< System to PCI IO Space */
} MPIL_ENUM;

/**
*  @brief   PCMCIA Bus mapping index
*
*  The PCMCIA mapping index enumeration is used to index the three sets of 
*  PCMCIA bus mapping registers
*
*     @li Common - Common Memory Space
*     @li Attribute - Attribute Memory Space
*     @li IO - IO Memory Space
*/
typedef enum
{
   PCMCIA_COMMON = 0,            /**< Commone Memory Space */
   PCMCIA_ATTRIBUTE,             /**< Attribute Memory Space */
   PCMCIA_IO,                    /**< IO Memory Space */
} PCMCIA_ENUM;

typedef struct
{
   int (*mpilPcmciaBaseSizeGetp)( int ch, unsigned int *basep, unsigned int *sizep );
   unsigned int (*mpilPcmciaBaseSizeSetp)( int ch, unsigned int *basep, unsigned int *sizep );
   int (*mpilPcmciaEnablep)( int ch );
   int (*mpilPcmciaDisablep)( int ch );
   int (*mpilPcmciaIsEnabledp)( int ch );
   int (*mpilPcmciaCardTypeVccDetectp)( MPIL_CARDTYPE* cardTypep, MPIL_CARDVCC* cardVccp );
   int (*mpilPcmciaResetSetp)( void );
   int (*mpilPcmciaResetClrp)( void );
   int (*mpilPcmciaResetIsSetp)( void );
   int (*mpilPcmciaCountSetp)( int *rwActiveCountp, int *inactiveCountp, int *ceSetupCountp, int *ceHoldCountp );
   int (*mpilPcmciaCountGetp)( int *rwActiveCountp, int *inactiveCountp, int *ceSetupCountp, int *ceHoldCountp );
   int (*mpilPcmcia16BitEnablep)( void );
   int (*mpilPcmcia16BitDisablep)( void );
   int (*mpilPcmcia16BitIsEnabledp)( void );
   int (*mpilPcmciaNoByteSwapEnablep)( void );
   int (*mpilPcmciaNoByteSwapDisablep)( void );
   int (*mpilPcmciaNoByteSwapIsEnabledp)( void );
   int (*mpilCardBusResetSetp)( void );
   int (*mpilCardBusResetClrp)( void );
   int (*mpilCardBusResetIsSetp)( void );
   int (*mpilCardBusIdSetp)( int id );
   int (*mpilCardBusIdGetp)( void );

   int (*mpilPciToSysAddrSizeRemapGetp)( int ch, unsigned int *sizep, unsigned int *remapp );
   unsigned int (*mpilPciToSysAddrSizeRemapSetp)( int ch, unsigned int *sizep, unsigned int *remapp );
   int (*mpilSysToPciSizeBaseRemapGetp)( int ch, unsigned int *sizep, unsigned int *basep, unsigned int *remapp );
   unsigned int (*mpilSysToPciSizeBaseRemapSetp)( int ch, unsigned int *sizep, unsigned int *basep, unsigned int *remapp );
   int (*mpilSysToPciEnablep)( int ch );
   int (*mpilSysToPciDisablep)( int ch );
   int (*mpilSysToPciIsEnabledp)( int ch );
   int (*mpilSysToPciCardBusEnablep)( int ch );
   int (*mpilSysToPciCardBusDisablep)( int ch );
   int (*mpilSysToPciCardBusIsEnabledp)( int ch );

   int (*mpilPciCtrlBar2NoswapEnablep)( void );
   int (*mpilPciCtrlBar2NoswapDisablep)( void );
   int (*mpilPciCtrlBar2NoswapIsEnabledp)( void );
   int (*mpilPciCtrlBar1NoswapEnablep)( void );
   int (*mpilPciCtrlBar1NoswapDisablep)( void );
   int (*mpilPciCtrlBar1NoswapIsEnabledp)( void );
   int (*mpilPciCtrlReadPrefetchSetp)( int prefetch );
   int (*mpilPciCtrlReadPrefetchGetp)( void );
   int (*mpilSysCtrlNoswapEnablep)( void );
   int (*mpilSysCtrlNoswapDisablep)( void );
   int (*mpilSysCtrlNoswapIsEnabledp)( void );
   int (*mpilSysCtrlGpioEnablep)( void );
   int (*mpilSysCtrlGpioDisablep)( void );
   int (*mpilSysCtrlGpioIsEnabledp)( void );
   int (*mpilSysCtrlResetPciBusEnablep)( void );
   int (*mpilSysCtrlResetPciBusDisablep)( void );
   int (*mpilSysCtrlResetPciBusIsEnabledp)( void );

   int (*mpilSysPciCfgReadp)( unsigned int bus, unsigned int dev, unsigned int func, unsigned int offset, void *datap, int len );
   int (*mpilSysPciCfgWritep)( unsigned int bus, unsigned int dev, unsigned int func, unsigned int offset, void *datap, int len );
   unsigned int (*mpilLocPciCfgReadp)( unsigned int reg );
   unsigned int (*mpilLocPciCfgWritep)( unsigned int reg, unsigned int value );

   int (*mpilSysIrqEnablep)( int irq );
   int (*mpilSysIrqDisablep)( int irq );
   int (*mpilSysIrqIsEnabledp)( int irq );
   int (*mpilSysIrqGetp)( int irq );
   int (*mpilSysIrqClrp)( int irq );
   int (*mpilSysIrqLowerToUpperp)( int irq );
   int (*mpilSysIrqUpperToLowerp)( int irq );

#if 0
   int (*mpilPciToSysDmaEnablep)( void );
   int (*mpilPciToSysDmaDisablep)( void );
   int (*mpilPciToSysDmaIsEnabledp)( void );
   int (*mpilPciToSysDmaIrqDescDoneEnablep)( void );
   int (*mpilPciToSysDmaIrqDescDoneDisablep)( void );
   int (*mpilPciToSysDmaIrqDescDoneIsEnabledp)( void );
   int (*mpilPciToSysDmaIrqDescDoneGetp)( void );
   int (*mpilPciToSysDmaIrqDescDoneClrp)( void );
   int (*mpilPciToSysDmaDescAddrSetp)( unsigned int addr );
   unsigned int (*mpilPciToSysDmaDescAddrGetp)( void );

   int (*mpilSysToPciDmaEnablep)( void );
   int (*mpilSysToPciDmaDisablep)( void );
   int (*mpilSysToPciDmaIsEnabledp)( void );
   int (*mpilSysToPciDmaIrqDescDoneEnablep)( void );
   int (*mpilSysToPciDmaIrqDescDoneDisablep)( void );
   int (*mpilSysToPciDmaIrqDescDoneIsEnabledp)( void );
   int (*mpilSysToPciDmaIrqDescDoneGetp)( void );
   int (*mpilSysToPciDmaIrqDescDoneClrp)( void );
   int (*mpilSysToPciDmaDescAddrSetp)( unsigned int addr );
   unsigned int (*mpilSysToPciDmaDescAddrGetp)( void );
#endif
} MPIL_FUNCS;

#define mpilPcmciaBaseSizeGet( drvp, ch, basep, sizep ) \
                                             (drvp)->funcp->mpilPcmciaBaseSizeGetp( ch, basep, sizep )
#define mpilPcmciaBaseSizeSet( drvp, ch, basep, sizep ) \
                                             (drvp)->funcp->mpilPcmciaBaseSizeSetp( ch, basep, sizep )
#define mpilPcmciaEnable( drvp, ch )         (drvp)->funcp->mpilPcmciaEnablep( ch )
#define mpilPcmciaDisable( drvp, ch )        (drvp)->funcp->mpilPcmciaDisablep( ch )
#define mpilPcmciaIsEnabled( drvp, ch )      (drvp)->funcp->mpilPcmciaIsEnabledp( ch )
#define mpilPcmciaCardTypeVccDetect( drvp, cardTypep, cardVccp ) \
                                             (drvp)->funcp->mpilPcmciaCardTypeVccDetectp( cardTypep, cardVccp )
#define mpilPcmciaResetSet( drvp )           (drvp)->funcp->mpilPcmciaResetSetp()
#define mpilPcmciaResetClr( drvp )           (drvp)->funcp->mpilPcmciaResetClrp()
#define mpilPcmciaResetIsSet( drvp )         (drvp)->funcp->mpilPcmciaResetIsSetp()
#define mpilPcmciaCountSet( drvp, rwActiveCountp, inactiveCountp, ceSetupCountp, ceHoldCountp ) \
                                             (drvp)->funcp->mpilPcmciaCountSetp( rwActiveCountp, inactiveCountp, ceSetupCountp, ceHoldCountp )
#define mpilPcmciaCountGet( drvp, rwActiveCountp, inactiveCountp, ceSetupCountp, ceHoldCountp ) \
                                             (drvp)->funcp->mpilPcmciaCountGetp( rwActiveCountp, inactiveCountp, ceSetupCountp, ceHoldCountp )
#define mpilPcmcia16BitEnable( drvp )        (drvp)->funcp->mpilPcmcia16BitEnablep()
#define mpilPcmcia16BitDisable( drvp )       (drvp)->funcp->mpilPcmcia16BitDisablep()
#define mpilPcmcia16BitIsEnabled( drvp )     (drvp)->funcp->mpilPcmcia16BitIsEnabledp()
#define mpilPcmciaNoByteSwapEnable( drvp )   (drvp)->funcp->mpilPcmciaNoByteSwapEnablep()
#define mpilPcmciaNoByteSwapDisable( drvp )  (drvp)->funcp->mpilPcmciaNoByteSwapDisablep()
#define mpilPcmciaNoByteSwapIsEnabled( drvp )(drvp)->funcp->mpilPcmciaNoByteSwapIsEnabledp()
#define mpilCardBusResetSet( drvp )          (drvp)->funcp->mpilCardBusResetSetp()
#define mpilCardBusResetClr( drvp )          (drvp)->funcp->mpilCardBusResetClrp()
#define mpilCardBusResetIsSet( drvp )        (drvp)->funcp->mpilCardBusResetIsSetp()
#define mpilCardBusIdSet( drvp, id )         (drvp)->funcp->mpilCardBusIdSetp( id )
#define mpilCardBusIdGet( drvp )             (drvp)->funcp->mpilCardBusIdGetp()

#define mpilPciToSysAddrSizeRemapGet( drvp, ch, sizep, remapp ) \
                                                (drvp)->funcp->mpilPciToSysAddrSizeRemapGetp( ch, sizep, remapp )
#define mpilPciToSysAddrSizeRemapSet( drvp, ch, sizep, remapp ) \
                                                (drvp)->funcp->mpilPciToSysAddrSizeRemapSetp( ch, sizep, remapp )
#define mpilSysToPciSizeBaseRemapGet( drvp, ch, sizep, basep, remapp ) \
                                                (drvp)->funcp->mpilSysToPciSizeBaseRemapGetp( ch, sizep, basep, remapp )
#define mpilSysToPciSizeBaseRemapSet( drvp, ch, sizep, basep, remapp ) \
                                                (drvp)->funcp->mpilSysToPciSizeBaseRemapSetp( ch, sizep, basep, remapp )
#define mpilSysToPciEnable( drvp, ch )          (drvp)->funcp->mpilSysToPciEnablep( ch )
#define mpilSysToPciDisable( drvp, ch )         (drvp)->funcp->mpilSysToPciDisablep( ch )
#define mpilSysToPciIsEnabled( drvp, ch )       (drvp)->funcp->mpilSysToPciIsEnabledp( ch )
#define mpilSysToPciCardBusEnable( drvp, ch )   (drvp)->funcp->mpilSysToPciCardBusEnablep( ch )
#define mpilSysToPciCardBusDisable( drvp, ch )  (drvp)->funcp->mpilSysToPciCardBusDisablep( ch )
#define mpilSysToPciCardBusIsEnabled( drvp, ch )(drvp)->funcp->mpilSysToPciCardBusIsEnabledp( ch )

#define mpilPciCtrlBar2NoswapEnable( drvp )     (drvp)->funcp->mpilPciCtrlBar2NoswapEnablep()
#define mpilPciCtrlBar2NoswapDisable( drvp )    (drvp)->funcp->mpilPciCtrlBar2NoswapDisablep()
#define mpilPciCtrlBar2NoswapIsEnabled( drvp )  (drvp)->funcp->mpilPciCtrlBar2NoswapIsEnabledp()
#define mpilPciCtrlBar1NoswapEnable( drvp )     (drvp)->funcp->mpilPciCtrlBar1NoswapEnablep()
#define mpilPciCtrlBar1NoswapDisable( drvp )    (drvp)->funcp->mpilPciCtrlBar1NoswapDisablep()
#define mpilPciCtrlBar1NoswapIsEnabled( drvp )  (drvp)->funcp->mpilPciCtrlBar1NoswapIsEnabledp()
#define mpilPciCtrlReadPrefetchSet( drvp, prefetch ) \
                                                (drvp)->funcp->mpilPciCtrlReadPrefetchSetp( prefetch )
#define mpilPciCtrlReadPrefetchGet( drvp )      (drvp)->funcp->mpilPciCtrlReadPrefetchGetp()
#define mpilSysCtrlNoswapEnable( drvp )         (drvp)->funcp->mpilSysCtrlNoswapEnablep()
#define mpilSysCtrlNoswapDisable( drvp )        (drvp)->funcp->mpilSysCtrlNoswapDisablep()
#define mpilSysCtrlNoswapIsEnabled( drvp )      (drvp)->funcp->mpilSysCtrlNoswapIsEnabledp()
#define mpilSysCtrlGpioEnable( drvp )           (drvp)->funcp->mpilSysCtrlGpioEnablep()
#define mpilSysCtrlGpioDisable( drvp )          (drvp)->funcp->mpilSysCtrlGpioDisablep()
#define mpilSysCtrlGpioIsEnabled( drvp )        (drvp)->funcp->mpilSysCtrlGpioIsEnabledp()
#define mpilSysCtrlResetPciBusEnable( drvp )    (drvp)->funcp->mpilSysCtrlResetPciBusEnablep()
#define mpilSysCtrlResetPciBusDisable( drvp )   (drvp)->funcp->mpilSysCtrlResetPciBusDisablep()
#define mpilSysCtrlResetPciBusIsEnabled( drvp ) (drvp)->funcp->mpilSysCtrlResetPciBusIsEnabledp()

#define mpilSysPciCfgRead( drvp, bus, dev, func, offset, datap, len) \
                                                (drvp)->funcp->mpilSysPciCfgReadp( bus, dev, func, offset, datap, len );
#define mpilSysPciCfgWrite( drvp, bus, dev, func, offset, datap, len) \
                                                (drvp)->funcp->mpilSysPciCfgWritep( bus, dev, func, offset, datap, len );
#define mpilLocPciCfgRead( drvp, reg )          (drvp)->funcp->mpilLocPciCfgReadp( reg )
#define mpilLocPciCfgWrite( drvp, reg, value )  (drvp)->funcp->mpilLocPciCfgWritep( reg, value )

#define mpilSysIrqEnable( drvp, irq )           (drvp)->funcp->mpilSysIrqEnablep( irq )
#define mpilSysIrqDisable( drvp, irq )          (drvp)->funcp->mpilSysIrqDisablep( irq )
#define mpilSysIrqIsEnabled( drvp, irq )        (drvp)->funcp->mpilSysIrqIsEnabledp( irq )
#define mpilSysIrqGet( drvp, irq )              (drvp)->funcp->mpilSysIrqGetp( irq )
#define mpilSysIrqClr( drvp, irq )              (drvp)->funcp->mpilSysIrqClrp( irq )
#define mpilSysIrqLowerToUpper( drvp, irq )     (drvp)->funcp->mpilSysIrqLowerToUpperp( irq )
#define mpilSysIrqUpperToLower( drvp, irq )     (drvp)->funcp->mpilSysIrqUpperToLowerp( irq )

#if 0
   int (*mpilPciToSysDmaEnablep)( void );
   int (*mpilPciToSysDmaDisablep)( void );
   int (*mpilPciToSysDmaIsEnabledp)( void );
   int (*mpilPciToSysDmaIrqDescDoneEnablep)( void );
   int (*mpilPciToSysDmaIrqDescDoneDisablep)( void );
   int (*mpilPciToSysDmaIrqDescDoneIsEnabledp)( void );
   int (*mpilPciToSysDmaIrqDescDoneGetp)( void );
   int (*mpilPciToSysDmaIrqDescDoneClrp)( void );
   int (*mpilPciToSysDmaDescAddrSetp)( unsigned int addr );
   unsigned int (*mpilPciToSysDmaDescAddrGetp)( void );

   int (*mpilSysToPciDmaEnablep)( void );
   int (*mpilSysToPciDmaDisablep)( void );
   int (*mpilSysToPciDmaIsEnabledp)( void );
   int (*mpilSysToPciDmaIrqDescDoneEnablep)( void );
   int (*mpilSysToPciDmaIrqDescDoneDisablep)( void );
   int (*mpilSysToPciDmaIrqDescDoneIsEnabledp)( void );
   int (*mpilSysToPciDmaIrqDescDoneGetp)( void );
   int (*mpilSysToPciDmaIrqDescDoneClrp)( void );
   int (*mpilSysToPciDmaDescAddrSetp)( unsigned int addr );
   unsigned int (*mpilSysToPciDmaDescAddrGetp)( void );
#endif

typedef struct
{
   /* This must be the first structure member. */
   const MPIL_FUNCS *funcp;

} MPIL_DRV;


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
