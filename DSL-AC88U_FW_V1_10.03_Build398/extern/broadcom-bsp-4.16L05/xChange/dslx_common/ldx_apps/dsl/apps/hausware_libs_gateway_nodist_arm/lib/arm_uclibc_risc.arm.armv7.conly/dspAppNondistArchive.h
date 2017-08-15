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
*    Description:                                                             
*                                                                             
*     Composite DSP image.                                                    
*                                                                             
****************************************************************************  
*                                                                             
*     Filename:   /projects/views/redekopg_lab_2/CommEngine/release/REL_416L05/xChange/dslx_common/ldx_apps/dsl/apps/hausware_libs_gateway_nodist_arm/lib/arm_uclibc_risc.arm.armv7.conly/dspAppNondistArchive.h                                              
*     Date:                                                             
*     Directory:  /projects/views/redekopg_lab_2/CommEngine/release/REL_416L05/xChange/dslx_common/ldx_apps/dsl/apps/hausware_libs_gateway_nodist_arm                                                      
*     Input file listing:                                                     
*         Internal memory image:           0        
*         Program overlay image:           0            
*         Instance memory overlay image:   0           
*                                                                             
***************************************************************************/  



#include <dspImageArchive.h>






/*
** Composite DSP "image-archive" that contains images
** to be loaded, image capabilities, config info, etc...
*/
const DSP_IMAGE_ARCHIVE dspAppNondistArchive =
{
   {
      /* Internal memory image. */
      0,
      /* Program overlay image. */
      0,
      /* Program overlay size (words) */
      0,
      /* Instance memory overlay image. */
      0,
      /* Instance memory overlay size (words) */
      0,
   },
   {
      /* numGatewayVhds */
      6,
      /* numPhoneVhds */
      0,
      /* numToneVhds */
      0,
      /* numConferenceVhds */
      4,
      /* numHalfVhds */
      0,
      /* numPstnVhds */
      1,
      /* numLineVhds */
      7,
      /* g711Support */
      1,
      /* g722Support */
      1,
      /* g729Support */
      (1 && 0),
      /* g729ASupport */
      (1 && 1),
      /* g729BSupport */
      (1 && 1),
      /* g729ESupport */
      (1 && 0),
      /* g7231Support */
      0,
      /* g7231ASupport */
      (0 && 1),
      /* g728Support */
      0,
      /* g726Support */
      1,
      /* gBV16Support */
      0,
      /* gBV32Support */
      0,
      /* lpcm64Support */
      0,
      /* lpcm128Support */
      1,
      /* lpcm256Support */
      1,
      /* gILBCSupport */
      0,
      /* gsmamrSupport */
      1,
      /* gsmefrSupport */
      1,
      /* gsmfrSupport */
      1,
      /* gsmhrSupport */
      1,
      /* amrwbSupport */
      1,
      /* faxSupport */
      1,
      /* gplcSupport */
      1,
      /* bvcSupport */
      1,
      /* icpSupport */
      0,
      /* equSupport */
      1
   },
   {
      /* resamplerInternationalSupport */
      0xdead,
      /* resamplerCoeffInProgMem */
      0xdead,
      /* resamplerInterpFilterLen */
      0xdead,
      /* resamplerInterpFilterAddr */
      0xdead,
      /* resamplerDecimFilterLen */
      0xdead,
      /* resamplerDecimFilterAddr */
      0xdead,
      /* Is Hausware distributed? */
      !1,
      /* Is wideband sampling used? */
      1,
      /* Num 8kHz samples per hausware tick */
      (40)
   }
};






