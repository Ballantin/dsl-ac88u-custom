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
*  @file    apml.h
*
*  @brief   Lower APM (Audio Processor Module) device driver header file
*
*  This file contains the lower device driver header information for the
*  APM module.
*/
/****************************************************************************/

#ifndef APML_H
#define APML_H

/* ---- Include Files ----------------------------------------------------- */
#include <xdrvTypes.h>

/* ---- Public Constants and Types ---------------------------------------- */

/**
*  @enum     APML_COEFF_TX_EQ
*  Transmit Equalizer coefficients
*/
/**
*  @enum     APML_COEFF_RX_EQ
*  Receive Equalizer coefficients
*/
/**
*  @enum     APML_COEFF_HYBRID_BAL
*  Hybrid Balance coefficients
*/
/**
*  @enum     APML_COEFF_Y_FILTER
*  Y-filter coefficients
*/
/**
*  @enum     APML_COEFF_ASRC_INT
*  ASRC Interpolator coefficients
*/
/**
*  @enum     APML_COEFF_ASRC_DEC
*  ASRC Decimator coefficients
*/

/**
*  @brief   APM coefficient filter type
*
*  The enumeration is used to describe which APM filter to program
*/
typedef enum
{
   APML_COEFF_TX_EQ = 0,   /**< Transmit Equalizer coefficients */
   APML_COEFF_RX_EQ,        /**< Receive Equalizer coefficients */
   APML_COEFF_HYBRID_BAL,   /**< Hybrid Balance coefficients */
   APML_COEFF_Y_FILTER,     /**< Y-filter coefficients */
   APML_COEFF_ASRC_INT,     /**< ASRC Interpolator coefficients */
   APML_COEFF_ASRC_DEC     /**< ASRC Decimator coefficients */
} APML_COEFF;

typedef enum
{
   APML_DIR_TX = 0,         /**< Transmit direction  */
   APML_DIR_RX              /**< Receive direction */
} APML_DIR;

typedef enum
{
   APML_SWITCH_A = 0,            /**< Input A */
   APML_SWITCH_B,                /**< Input B */
   APML_SWITCH_ANALOG_LOOPBACK,  /**< Analog loopback VDAC to VADC */
   APML_SWITCH_NONE              /**< No input */
} APML_SWITCH;

/**
*  @brief   APM clock reference source
*
*/
typedef enum
{
   APML_EXTERNAL_REF_HSS0 = 0,
   APML_EXTERNAL_REF_HSS1,
   APML_INTERNAL_REF
} APML_CLOCK_REF_SOURCE;

/* used to turn off PGA gain blocks in rx direction in APM */
#define APML_RX_PGA_MUTE               -100

typedef struct
{
   int (*apmlInitp)( void );
   int (*apmlChValidp)( int ch );
   int (*apmlDisablep)( int ch );
   int (*apmlEnablep)( int ch );
   int (*apmlIsEnabledp)( int ch );
   int (*apmlPacketLenGetp)( int ch );
   int (*apmlPacketLenSetp)( int ch, int len );
   int (*apmlResetp)( int ch );
   int (*apmlSampleFreqGetp)( int ch );
   int (*apmlSampleFreqSetp)( int ch, int freq );
   int (*apmlSampleSizeGetp)( int ch );
   int (*apmlSampleSizeSetp)( int ch, int size );
   int (*apmlSidetoneVolumeSetp)( int ch, int vol );
   int (*apmlSidetoneStatusSetp)( int ch, int status );
   int (*apmlInitRunOnDsp)( void );
   int (*apmlSetMuxp)( int ch, APML_SWITCH sw, APML_DIR dir );
   int (*apmlGetMuxp)( int ch, APML_SWITCH * sw, APML_DIR dir );
   int (*apmlSetGainp)( int ch, APML_SWITCH sw, APML_DIR dir, int gain );
   int (*apmlGetGainp)( int ch, APML_SWITCH sw, APML_DIR dir, int * gainp );
   int (*apmlWriteCoeffp)( int ch, APML_COEFF select, XDRV_SINT32 *data, int length );
   int (*apmlEqualizerStatusSetp)( int ch, APML_DIR dir, int status );
   int (*apmlEqualizerGainSetp)( int ch, APML_DIR dir, int gain );
   int (*apmlMicDetectionCfgp) (int ch, APML_SWITCH sw);
   int (*apmlMicDetectionGetp)( int ch );
   int (*apmlExternalClockModeSetp)( int ch, APML_CLOCK_REF_SOURCE clockRef, int freq );
   int (*apmlExternalClockModeGetp)( int ch, APML_CLOCK_REF_SOURCE* clockRef, int* freq );
} APML_FUNCS;

#define apmlInit( drvp )                                      (drvp)->funcp->apmlInitp()
#define apmlChValid( drvp , ch )                              (drvp)->funcp->apmlChValidp( ch )
#define apmlDisable( drvp , ch )                              (drvp)->funcp->apmlDisablep( ch )
#define apmlEnable( drvp , ch )                               (drvp)->funcp->apmlEnablep( ch )
#define apmlIsEnabled( drvp , ch )                            (drvp)->funcp->apmlIsEnabledp( ch )
#define apmlPacketLenGet( drvp , ch )                         (drvp)->funcp->apmlPacketLenGetp( ch )
#define apmlPacketLenSet( drvp , ch, freq )                   (drvp)->funcp->apmlPacketLenSetp( ch, freq )
#define apmlReset( drvp , ch )                                (drvp)->funcp->apmlResetp( ch )
#define apmlSampleFreqGet( drvp , ch )                        (drvp)->funcp->apmlSampleFreqGetp( ch )
#define apmlSampleFreqSet( drvp , ch, freq )                  (drvp)->funcp->apmlSampleFreqSetp( ch, freq )
#define apmlSampleSizeGet( drvp , ch )                        (drvp)->funcp->apmlSampleSizeGetp( ch )
#define apmlSampleSizeSet( drvp , ch, size )                  (drvp)->funcp->apmlSampleSizeSetp( ch, size )
#define apmlSidetoneVolumeSet( drvp , ch, vol )               (drvp)->funcp->apmlSidetoneVolumeSetp( ch, vol )
#define apmlSidetoneStatusSet( drvp , ch, status )            (drvp)->funcp->apmlSidetoneStatusSetp( ch, status )
#define apmlInitRunOnDsp( drvp )                              (drvp)->funcp->apmlInitRunOnDsp( )
#define apmlSetMux( drvp , ch, sw, dir )                      (drvp)->funcp->apmlSetMuxp( ch, sw, dir )
#define apmlGetMux( drvp , ch, swp, dir )                     (drvp)->funcp->apmlGetMuxp( ch, swp, dir )
#define apmlSetGain( drvp , ch, sw, dir, gain )               (drvp)->funcp->apmlSetGainp( ch, sw, dir, gain )
#define apmlGetGain( drvp , ch, sw, dir, gainp )              (drvp)->funcp->apmlGetGainp( ch, sw, dir, gainp )
#define apmlWriteCoeff( drvp, ch, select, data, length )      (drvp)->funcp->apmlWriteCoeffp( ch, select, data, length )
#define apmlEqualizerStatusSet( drvp, ch, dir, status )       (drvp)->funcp->apmlEqualizerStatusSetp( ch, dir, status )
#define apmlEqualizerGainSet( drvp, ch, dir, gain )           (drvp)->funcp->apmlEqualizerGainSetp( ch, dir, gain )
#define apmlMicDetectionCfg( drvp, ch, sw )                   (drvp)->funcp->apmlMicDetectionCfgp( ch, sw )
#define apmlMicDetectionGet( drvp, ch )                       (drvp)->funcp->apmlMicDetectionGetp( ch )
#define apmlExternalClockModeSet( drvp, ch, clockRef, freq )  (drvp)->funcp->apmlExternalClockModeSetp( ch, clockRef, freq )
#define apmlExternalClockModeGet( drvp, ch, clockRefp, freqp ) (drvp)->funcp->apmlExternalClockModeGetp( ch, clockRefp, freqp )

typedef struct
{
   /* This must be the first structure member. */
   const APML_FUNCS *funcp;

} APML_DRV;


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
