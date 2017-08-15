/***************************************************************************
*    Copyright © 2000-2009 Broadcom                             
*                                                                           
*    This program is the proprietary software of Broadcom and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized    
*    License").  Except as set forth in an Authorized License, Broadcom     
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,            
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY 
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: casCtlCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in casCtlCfg.h
*
****************************************************************************/

#ifndef CAS_CTL_CFG_CUSTOM_H
#define CAS_CTL_CFG_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <globalCfg.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/* Task priority for CAS */
#define CAS_CTL_CFG_TASK_PRIORITY                  MTA_TASK_HARDWARE

/* Maximum number of supported endpoints */
#define CAS_CTL_CFG_NUM_ENDPTS                     GLOBAL_CFG_MAX_ENDPT


/* Set these values to 1 to enable support for any set of static, country
** specific ring patterns. Set to 0 if ring patterns are not used or are
** dynamically provisioned */
#define CAS_CTL_CFG_NORTH_AMERICAN_RING_PROFILES   1
#define CAS_CTL_CFG_JAPANESE_RING_PROFILES         1
#define CAS_CTL_CFG_UK_RING_PROFILES               1
#define CAS_CTL_CFG_CHINA_RING_PROFILES            1

/* Egress packets coming from the network are looped back in the DSP 
** and sent back to the network as ingress packets */
#define HOOK_EVENTS_SIMULATED_THROUGH_COMMAND_LINE 0

/* Adjustment to use for debouncing time on SLIC */
#define CAS_CTL_DEFAULT_FXO_DEBOUNCE_MSEC         14

#ifdef __cplusplus
}
#endif

#endif  /* CAS_CTL_CFG_CUSTOM_H  */
