/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      Pulse Signal definitions
*
****************************************************************************/

#ifndef HAPI_PULSE_SIGNAL_H
#define HAPI_PULSE_SIGNAL_H

/* ---- Include Files ---------------------------------------- */

// NV - FIXME
//#include <pxd_cfg.h>
#define PXD_CFG_NUM_PXDS   2

#include <hapi_types.h>
#include <xcfg_vhd_custom.h>

#ifndef XCFG_VHD_NUM_LINE_VHDS
   #error "How many line VHDs do you have?"
#endif

/* ---- Constants and Types ---------------------------------- */

/*
** Pulse signal parameter structure
*/
typedef struct HAPI_PULSE_SIGNAL_PARMS
{
   HAPI_UINT16   pulseFrequency;  /* Pulse frequency (in kHz) */
   HAPI_UINT16   pulseAmplitude;  /* Pulse amplitude (in dB) */
   HAPI_UINT16   pulseDuration;   /* Pulse duration (in ms) */
   HAPI_UINT16   pulsePeriod;     /* Pulse period (in ms) */
   HAPI_UINT16   pulseCount;      /* Remaining number pulses to generate */

} HAPI_PULSE_SIGNAL_PARMS;


/*
** Pulse Signal Frequency
*/
#define METER_PULSE_FREQ_12KHZ      12000
#define METER_PULSE_FREQ_16KHZ      16000

/* Global pulse signal structure */
extern HAPI_PULSE_SIGNAL_PARMS   gPulseSignalParms[XCFG_VHD_NUM_LINE_VHDS];


#endif  /* HAPI_PULSE_SIGNAL_H */
