/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: boardHalDspCommon.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent DSP interface.
*
****************************************************************************/

/* --------------------------------------------------------------------------
** TODO - The following functionality has been grouped into this common
**        file because I don't want to the duplicate the same code in every
**        board HAL. However, it is assumed that all DSP images are
**        composite DSP "image-archive" that contains images to be loaded,
**        image capabilities, config info, etc..
**
**        These assumptions should be true for all cablex boaard HALs; however,
**        are likely not true for other groups. We will need to either
**        abstract these dependencies or have other groups adopt the same
**        conventions.
*/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <boardHalDsp.h>
#include <dspImageArchive.h>
#include <xchgAssert.h>
#include <endpt.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalDspDisplayCfg
**
** PURPOSE:    Displays DSP configuration and capabilities.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
**
*****************************************************************************
*/
void boardHalDspDisplayCfg( void )
{
   const DSP_CAPABILITIES *pCap;
   const DSP_CONFIG       *pConfig;
   const char       *trueStr  = "TRUE";
   const char       *falseStr = "FALSE";
   const char       *trueOrFalseStr;

#define COLUMN_WIDTH    15

   /* Look up the DSP capabilities */
   pCap     = &(dspImageArchiveGet()->caps);
   pConfig  = &(dspImageArchiveGet()->config);


   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\tDSP BUILD DATE: %s", VRG_DSP_DATE_STR ));
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "Capabilities:" ));
   trueOrFalseStr = falseStr;
   if ( pCap->g711Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.711:", trueOrFalseStr ));

   if ( pCap->g722Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.722:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->g729ASupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.729A:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->g729Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.729:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->g729ESupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.729E:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->g729ABSupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.729B:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->g7231Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.723.1:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->g728Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.728:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->g726Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "G.726:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->bv16Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "BV16:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pCap->bv32Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "BV32:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->lpcm64Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "LINPCM64:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->lpcm128Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "LINPCM128:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->lpcm256Support )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "LINPCM256:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->iLbcSupport  )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "iLBC:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->gsmamrSupport  )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "AMR:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->amrwbSupport  )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "AMRWB:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->faxSupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "T.38:", trueOrFalseStr ));


   trueOrFalseStr = falseStr;
   if ( pConfig->isHauswareDistributed )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "Distributed:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pConfig->isWidebandEnabled )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "Wideband:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->icpSupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "ICP:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->gplcSupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "GPLC:", trueOrFalseStr ));

   trueOrFalseStr = falseStr;
   if ( pCap->bvcSupport )
   {
      trueOrFalseStr = trueStr;
   }
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\t%-*s %s", COLUMN_WIDTH, "BVC:", trueOrFalseStr ));


   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "" ));
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\tFrame rate:           %d ms", pConfig->frameSyncSamples>>3 ));

   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "" ));
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\tMax endpts:           %d", pCap->numLineVhds ));
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\tMax full connections: %d", pCap->numGatewayVhds ));
   VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "\tMax lite connections: %d", pCap->numConferenceVhds ));
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspPrintCfg
**
** PURPOSE:    Retrieves DSP configuration and capabilities.
**
** PARAMETERS: dspCfg   (out) DSP config info.
**
** RETURNS:    none.
**
** NOTE:
**
*****************************************************************************
*/
void boardHalDspGetCfg( BOARD_HAL_DSP_CFG *dspCfg )
{
   VRG_UINT8         vbdCapabilities;
   const DSP_CAPABILITIES *pCap;
   const DSP_CONFIG       *pConfig;

   vbdCapabilities = EPVBDMODE_NONE;

   /* Look up the DSP capabilities */
   pCap     = &(dspImageArchiveGet()->caps);
   pConfig  = &(dspImageArchiveGet()->config);

   XCHG_ASSERT( pCap != NULL );

   if ( pCap->g711Support )
   {
      dspCfg->codecCapabilities[CODEC_PCMU] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_PCMA] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: PCMA/PCMU"));
   }

   if ( pCap->g722Support )
   {
      dspCfg->codecCapabilities[CODEC_G722_MODE_1] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G722"));
   }

   /* Only support G729A and G729E. Not informing support of Annex B*/
   if ( pCap->g729ASupport )
   {
      dspCfg->codecCapabilities[CODEC_G729A] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G729A"));
   }

   if ( pCap->g729Support )
   {
      dspCfg->codecCapabilities[CODEC_G729] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G729"));
   }

   if ( pCap->g729ABSupport )
   {
      dspCfg->codecCapabilities[CODEC_G729B] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G729B"));
   }

   if ( pCap->g729ESupport )
   {
      dspCfg->codecCapabilities[CODEC_G729E] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G729E"));
   }

   if ( pCap->g7231Support )
   {
      /* Will not inform support of G.723.1A at this time */
      dspCfg->codecCapabilities[CODEC_G7231_53] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G7231_63] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G7231_VAR] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G7231_53_VAR] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G7231_63_VAR] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G723"));
   }

   if ( pCap->g728Support )
   {
      dspCfg->codecCapabilities[CODEC_G728] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G728"));
   }

   if ( pCap->g726Support )
   {
      dspCfg->codecCapabilities[CODEC_G726_16] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G726_24] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G726_32] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_G726_40] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: G726"));
   }

   if ( pCap->bv16Support )
   {
      dspCfg->codecCapabilities[CODEC_BV16] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: BV16"));
   }

   if ( pCap->bv32Support )
   {
      dspCfg->codecCapabilities[CODEC_BV32] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: BV32"));
   }

   if ( pCap->lpcm64Support )
   {
      dspCfg->codecCapabilities[CODEC_LINPCM64] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_CLEARCHANNEL] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: LINPCM64"));
   }

   if ( pCap->lpcm128Support )
   {
      dspCfg->codecCapabilities[CODEC_LINPCM128] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: LINPCM128"));
   }

   if ( pCap->lpcm256Support )
   {
      dspCfg->codecCapabilities[CODEC_LINPCM256] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: LINPCM256"));
   }

   if ( pCap->iLbcSupport  )
   {
      dspCfg->codecCapabilities[CODEC_ILBC_20] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_ILBC_30] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: ILBC"));
   }

   if ( pCap->gsmamrSupport  )
   {
      dspCfg->codecCapabilities[CODEC_GSMAMR_12K] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_10K] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_795] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_740] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_670] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_590] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_515] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_475] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_12K] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_10K] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_795] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_740] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_670] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_590] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_515] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_GSMAMR_OA_475] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: AMRNB"));
   }

   if ( pCap->gsmefrSupport  )
   {
      dspCfg->codecCapabilities[CODEC_GSMEFR] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: GSMEFR"));
   }

   if ( pCap->gsmfrSupport  )
   {
      dspCfg->codecCapabilities[CODEC_GSMFR] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: GSMFR"));
   }

   if ( pCap->gsmhrSupport  )
   {
      dspCfg->codecCapabilities[CODEC_GSMHR] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: GSMHR"));
   }

   if ( pCap->amrwbSupport  )
   {
      dspCfg->codecCapabilities[CODEC_AMRWB_66] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_885] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_1265] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_1425] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_1585] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_1825] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_1985] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_2305] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_2385] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_66] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_885] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_1265] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_1425] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_1585] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_1825] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_1985] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_2305] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_AMRWB_OA_2385] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: AMRWB"));
   }

   dspCfg->codecCapabilities[CODEC_NTE] = CODEC_SUPPORTED;

   /* Redundancy support.  RFC 2198, used for VBD V.152 mode. */
   dspCfg->codecCapabilities[CODEC_RED] = CODEC_SUPPORTED;
   dspCfg->maxRfc2198Level = 2;

   /* Check if T.38 is supported */
   if ( pCap->faxSupport )
   {
      dspCfg->codecCapabilities[CODEC_T38] = CODEC_SUPPORTED;
      dspCfg->codecCapabilities[CODEC_T38_MUTE] = CODEC_SUPPORTED;
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Codec Support: T.38"));
   }

   /* Always support legacy voice band data. */
   vbdCapabilities |= ( EPVBDMODE_LEGACY );

   /* VBD V.152 is currently always supported */
   vbdCapabilities |= ( EPVBDMODE_V152 );

   dspCfg->vbdCapability = vbdCapabilities;

   /* Is idle profiler supported? */
   dspCfg->isIcpEnabled = pCap->icpSupport;

   /* Is voice stack distributed? */
   dspCfg->isDistributed = pConfig->isHauswareDistributed;

   /* Is wideband supported? */
   dspCfg->isWidebandEnabled = pConfig->isWidebandEnabled;

   /* What packet rate is DSP running at? */
   dspCfg->frameSyncSamples = pConfig->frameSyncSamples;
   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Frame Sync Samples: %d",  pConfig->frameSyncSamples));

   /* Set maximum number of endpoints and connections available */
   dspCfg->maxEndPts    = pCap->numLineVhds;
   dspCfg->maxFullCnxs  = pCap->numGatewayVhds;
   dspCfg->maxLiteCnxs  = pCap->numConferenceVhds;

   /* PLC settings */
   dspCfg->gplcSupport = pCap->gplcSupport;
   dspCfg->bvcSupport = pCap->bvcSupport;

   /* Is software equalizer supported? */
   dspCfg->isEquEnabled = pCap->equSupport;
}
