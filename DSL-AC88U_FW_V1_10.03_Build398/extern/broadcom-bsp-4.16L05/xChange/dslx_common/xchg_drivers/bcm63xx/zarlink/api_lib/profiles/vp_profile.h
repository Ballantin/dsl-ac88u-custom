/*
 * 
 *
 * This header file exports the Profile data types
 *
 * Project Info --
 *   File:  
 *   Type:  VP880 VoicePort MiniPBX Project (Line Module LE71HE0824)
 *   Date:  Thursday, April 01, 2010 10:15:17
 *
 *   This file was generated with Profile Wizard Version: P1.14.1-P2.3.0
 */

#ifndef VP_PROFILE_H
#define VP_PROFILE_H

#ifdef VP_API_TYPES_H
#include "vp_api_types.h"
#else
typedef unsigned char VpProfileDataType;
#endif

/* Le9652 Tracker Inverting Boost */
/************** Device Profile **************/
extern const VpProfileDataType DEV_PRODILE_Le9652_135V_IB[];/* Device Configuration Data 135V Inv-Boost */
extern const VpProfileDataType DEV_PROFILE_Le9652_150V_FB[];/* Device Configuration Data - Le9652 Tracker 150V Flyback */
/* Le9661 Buck-boost profile */
/************** Device Profile **************/
extern const VpProfileDataType DEV_PROFILE_Le9661_Buck_Boost[];/* Device Configuration Data - 47uH 1.5A Buck-Boost */
/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_miSLIC_Le9661_BB_DEF[];/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */
/************** AC Profile **************/
/************** Ringing Profile **************/
extern const VpProfileDataType RING_miSLIC_Le9661_BB_DEF[]; /* Default Ringing 25Hz 50Vrms Tracking, AC Trip - Use for all countries unless country profile exists */
extern const VpProfileDataType RING_miSLIC_Le9661_BB_ETSI[];/* ETSI  Ringing 25Hz 60Vrms Tracking, AC Trip */

/* Le9641 ZLR964122 REV A1 BB ZSI daughter card */
extern const VpProfileDataType Le9641_DEV_PROFILE_ZSI_Buck_Boost[]; /* Device Configuration Data - 47uH 1.5A Buck-Boost */

/* Le9642 miSLIC 2FXS_BUCK-BOOST ABS daughter card */
extern const VpProfileDataType DEV_PROFILE_47UH_1_5A[]; /* Device Configuration Data - 47uH Inductor */
extern const VpProfileDataType DC_FXS_VE960_ABS100V[];  /* DC FXS Parameters - 25mA Current Feed */
extern const VpProfileDataType RING_VE960_ABS100V_DEF[];/* Default Ringing 25Hz 50Vrms Fixed */

/* Le9672 135V Inverting Boost ZLR96741H Rev A0 daughter card */
/************** Device Profile **************/
extern const VpProfileDataType Le9672_12V_IB_135V[]; /* Device Configuration Data-Le9672 Tracker 135V Inverting Boost */

/************** Ringing Profile **************/
extern const VpProfileDataType RING_VE960_FB135V_US[];/* USA Ringing 20Hz 62Vrms + 20VDC Tracking, DC Trip */
/* Le9661 */
/************** Device_Parameters **************/
extern const VpProfileDataType Le9661_FB100V_DEVICE[];
/************** DC_Parameters **************/
extern const VpProfileDataType DC_FXS_ZL880_FB100V_EVAL[];
/*ZLR88842L REV A0 */
extern const VpProfileDataType DEV_PROFILE_47UH_BBABS[];/* Device Configuration Buck Boost ABS - 47uH Inductor */

/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_BBABS100V_DEF[];/* DC FXS Defaults - Use for all countries unless country file exists - 23mA Current Feed */

/************** AC Profile **************/
extern const VpProfileDataType AC_FXS_RF14_600R_DEF[];/* AC FXS RF14 600R Normal Coefficients (Default)  */

/************** Ringing Profile **************/
extern const VpProfileDataType RING_25HZ_70VPK[];    /* 50vrms 25Hz Ringing, AC Trip */
extern const VpProfileDataType RING_20HZ_70VPK[];    /* 50vrms 20Hz Ringing, AC Trip */

/* ZLR88641L */
/************** Device_Parameters **************/
extern const VpProfileDataType ZLR88621L_ZL880_ABS100V_DEVICE[];/* Device Configuration Data - ZL88601 100V ABS (-81V/-27V) */

/************** DC_Parameters **************/
extern const VpProfileDataType DC_FXS_ZL880_ABS100V_DEF[];/* DC FXS Defaults - Use for all countries unless country file exists - 25mA Current Feed */
extern const VpProfileDataType DC_FXS_VE960_ABS100V_DEF[];/* DC FXS Defaults - Use for all countries unless country file exists - 23mA Current Feed */
/************** AC_Coefficients **************/

/************** Ring_Parameters **************/
extern const VpProfileDataType RING_ZL880_ABS100V_DEF[];/* Default Ringing 25Hz 50Vrms Fixed, AC Trip - Use for all countries unless country profile exists */
extern const VpProfileDataType RING_ZL880_ABS100V_AT[];/* Austria Ringing 50Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_CA[];/* Canada Ringing 20Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_FI[];/* Finland Ringing 50Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_FR[];/* France Ringing 50Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_HK[];/* Hong Kong Ringing 20Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_JP[];/* Japan Ringing 16Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_KR[];/* Korea Ringing 20Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_SG[];/* Singapore Ringing 24Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_TW[];/* Taiwan Ringing 20Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_ABS100V_US[];/* USA Ringing 20Hz 50Vrms Fixed, AC Trip */
extern const VpProfileDataType RING_ZL880_FB100V_DEF[];/* Default Ringing 25Hz 50Vrms Fixed, AC Trip - Use for all countries unless country profile exists */
extern const VpProfileDataType RING_ZL880_FB100V_AT[]; /* Austria Ringing 50Hz 50Vrms Fixed, AC Trip */                                                      
extern const VpProfileDataType RING_ZL880_FB100V_CA[]; /* Canada Ringing 20Hz 50Vrms Fixed, AC Trip */                                                       
extern const VpProfileDataType RING_ZL880_FB100V_FI[]; /* Finland Ringing 50Hz 50Vrms Fixed, AC Trip */                                                      
extern const VpProfileDataType RING_ZL880_FB100V_FR[]; /* France Ringing 50Hz 50Vrms Fixed, AC Trip */                                                       
extern const VpProfileDataType RING_ZL880_FB100V_HK[]; /* Hong Kong Ringing 20Hz 50Vrms Fixed, AC Trip */                                                    
extern const VpProfileDataType RING_ZL880_FB100V_JP[]; /* Japan Ringing 16Hz 50Vrms Fixed, AC Trip */                                                        
extern const VpProfileDataType RING_ZL880_FB100V_KR[]; /* Korea Ringing 20Hz 50Vrms Fixed, AC Trip */                                                        
extern const VpProfileDataType RING_ZL880_FB100V_SG[]; /* Singapore Ringing 24Hz 50Vrms Fixed, AC Trip */                                                    
extern const VpProfileDataType RING_ZL880_FB100V_TW[]; /* Taiwan Ringing 20Hz 50Vrms Fixed, AC Trip */                                                       
extern const VpProfileDataType RING_ZL880_FB100V_US[]; /* USA Ringing 20Hz 50Vrms Fixed, AC Trip */                                                          

/************** Call_Progress_Tones **************/

/************** Cadence_Definitions **************/

/************** Caller_ID **************/

/************** Metering_Profile **************/

/* ZLR88742 */
/************** Device Profile **************/
extern const VpProfileDataType ZLR88742L_IB90V_DEVICE[];/* Device Configuration Data */

/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_ZL880_IB90V_DEF[];/* DC FXS Default - Use for all countries unless country file exists - 25mA Current Feed */

/* ZL88702 (ZLR88721H Rev A0 ) profiles, provided by Zarlink */
/* For daughter board ZL88702 (ZLR88721H Rev A0 ) ZSI mode */
/* When strapped in ZSI mode a ZL88702 is basically a Le9672*/
/************** Device Profile **************/
extern const VpProfileDataType Le9672_6V_FB150V_DEVICE[];/* Device Configuration Data - Le9672 Tracker 150V Flyback */
extern const VpProfileDataType Le9672_12V_FB150V[];  /* Device Configuration Data-Le9672 Tracker 150V Flyback */
/************** DC Profile **************/
extern const VpProfileDataType DC_FXS_VE960_FB150V_US[];/* USA DC FXS Parameters - 25mA Current Feed */
/************** AC Profile **************/
extern const VpProfileDataType AC_FXS_RF14_600R_DEF[];/* AC FXS RF14 600R Normal Coefficients (Default)  */
/************** Ringing Profile **************/
extern const VpProfileDataType RING_VE960_FB150V_US[];/* USA Ringing 20Hz 62Vrms + 48VDC Tracking, DC Trip */

/* below all use 88601's profile */
/************** AC Profile **************/

/************** Ringing Profile **************/

/************** Tone Profile **************/

/************** Cadence Profile **************/

/************** Caller ID Profile **************/

/************** Metering_Profile **************/

/* ZLR88645L */
/************** Device Profile **************/
extern const VpProfileDataType ZLR88645L_VE960_ABS100V_DEVICE[];/* Device Configuration Data */
extern const VpProfileDataType ZLR96622REV0_VE960_BBABS_DEV_PROFILE_47UH[];   /* Device Configuration Data - 47uH Inductor */

/* below all use 88601's profile */
/************** DC Profile **************/

/************** AC Profile **************/

/************** Ringing Profile **************/

/************** Tone Profile **************/

/************** Cadence Profile **************/

/************** Caller ID Profile **************/

/************** Metering_Profile **************/


/************** Device Parameters **************/
/* Device Profile */
extern const VpProfileDataType DEV_VE880_PROFILE[];
extern const VpProfileDataType DEV_VE880_88266_PROFILE_MASTER[];
extern const VpProfileDataType DEV_VE880_88266_PROFILE_SLAVE[];
extern const VpProfileDataType DEV_VE880_88266_PROFILE_SINGLE[];
extern const VpProfileDataType DEV_VE880_88276_PROFILE[];
extern const VpProfileDataType DEV_VE880_88506_PROFILE_FLYBACK_150V[];
extern const VpProfileDataType DEV_VE880_88506_PROFILE_INV_BOOST_150V[];
extern const VpProfileDataType DEV_VE880_88536_PROFILE_INV_BOOST_100V_THALASSA[];
extern const VpProfileDataType DEV_VE880_88536_PROFILE_INV_BOOST_100V_ZSI[];
extern const VpProfileDataType DEV_VE880_88264_PROFILE_FLYBACK_THALASSA[];
extern const VpProfileDataType DEV_VE880_88264_PROFILE_FLYBACK_ZSI[];
extern const VpProfileDataType DEV_VE890_PROFILE_FLYBACK_100V[];
extern const VpProfileDataType DEV_VE890_PROFILE_FLYBACK_150V[];
extern const VpProfileDataType DEV_VE890_PROFILE_BUCK_BOOST_100V[];
extern const VpProfileDataType DEV_VE890_PROFILE_INV_BOOST_100V[];
extern const VpProfileDataType DEV_VE890_PROFILE_INV_BOOST_150V[];
extern const VpProfileDataType DEV_VE890_PROFILE_FXO_ONLY[];

extern const VpProfileDataType ZLR88621H_ZL880_ABS120V_DEVICE[];

/************** AC_Filter_Coefficients **************/
/* AC Coefficients for FXS Line */
extern const VpProfileDataType FXS_AUSTRALIAProfile[];
extern const VpProfileDataType FXS_AUSTRALIAProfile_RF14[];
extern const VpProfileDataType FXS_AUSTRALIAProfile_890[];
extern const VpProfileDataType FXS_BELGIUMProfile[];
extern const VpProfileDataType FXS_BELGIUMProfile_RF14[];
extern const VpProfileDataType FXS_BELGIUMProfile_890[];
extern const VpProfileDataType FXS_BRAZILProfile[];
extern const VpProfileDataType FXS_BRAZILProfile_RF14[];
extern const VpProfileDataType FXS_BRAZILProfile_890[];
extern const VpProfileDataType FXS_DENMARKProfile[];
extern const VpProfileDataType FXS_DENMARKProfile_RF14[];
extern const VpProfileDataType FXS_DENMARKProfile_890[];
extern const VpProfileDataType FXS_CHILEProfile[];
extern const VpProfileDataType FXS_CHILEProfile_RF14[];
extern const VpProfileDataType FXS_CHILEProfile_890[];
extern const VpProfileDataType FXS_CHINAProfile[];
extern const VpProfileDataType FXS_CHINAProfile_RF14[];
extern const VpProfileDataType FXS_CHINAProfile_890[];
extern const VpProfileDataType FXS_CZECHProfile[];
extern const VpProfileDataType FXS_CZECHProfile_RF14[];
extern const VpProfileDataType FXS_CZECHProfile_890[];
extern const VpProfileDataType FXS_ETSIProfile[];
extern const VpProfileDataType FXS_ETSIProfile_RF14[];
extern const VpProfileDataType FXS_ETSIProfile_890[];
extern const VpProfileDataType FXS_FINLANDProfile[];
extern const VpProfileDataType FXS_FINLANDProfile_RF14[];
extern const VpProfileDataType FXS_FINLANDProfile_890[];
extern const VpProfileDataType FXS_FRANCEProfile[];
extern const VpProfileDataType FXS_FRANCEProfile_RF14[];
extern const VpProfileDataType FXS_FRANCEProfile_890[];
extern const VpProfileDataType FXS_GERMANYProfile[];
extern const VpProfileDataType FXS_GERMANYProfile_RF14[];
extern const VpProfileDataType FXS_GERMANYProfile_890[];
extern const VpProfileDataType FXS_HUNGARYProfile[];
extern const VpProfileDataType FXS_HUNGARYProfile_RF14[];
extern const VpProfileDataType FXS_HUNGARYProfile_890[];
extern const VpProfileDataType FXS_INDIAProfile[];
extern const VpProfileDataType FXS_INDIAProfile_RF14[];
extern const VpProfileDataType FXS_INDIAProfile_890[];
extern const VpProfileDataType FXS_ITALYProfile[];
extern const VpProfileDataType FXS_ITALYProfile_RF14[];
extern const VpProfileDataType FXS_ITALYProfile_890[];
extern const VpProfileDataType FXS_JAPANProfile[];
extern const VpProfileDataType FXS_JAPANProfile_RF14[];
extern const VpProfileDataType FXS_JAPANProfile_890[];
extern const VpProfileDataType FXS_MEXICOProfile[];
extern const VpProfileDataType FXS_MEXICOProfile_RF14[];
extern const VpProfileDataType FXS_MEXICOProfile_890[];
extern const VpProfileDataType FXS_NETHERLANDSProfile[];
extern const VpProfileDataType FXS_NETHERLANDSProfile_RF14[];
extern const VpProfileDataType FXS_NETHERLANDSProfile_890[];
extern const VpProfileDataType FXS_NEW_ZEALANDProfile[];
extern const VpProfileDataType FXS_NEW_ZEALANDProfile_RF14[];
extern const VpProfileDataType FXS_NEW_ZEALANDProfile_890[];
extern const VpProfileDataType FXS_NORTH_AMERICAProfile[];
extern const VpProfileDataType FXS_NORTH_AMERICAProfile_RF14[];
extern const VpProfileDataType FXS_NORTH_AMERICAProfile_890[];
extern const VpProfileDataType FXS_POLANDProfile[];
extern const VpProfileDataType FXS_POLANDProfile_RF14[];
extern const VpProfileDataType FXS_POLANDProfile_890[];
extern const VpProfileDataType FXS_SPAINProfile[];
extern const VpProfileDataType FXS_SPAINProfile_RF14[];
extern const VpProfileDataType FXS_SPAINProfile_890[];
extern const VpProfileDataType FXS_SWEDENProfile[];
extern const VpProfileDataType FXS_SWEDENProfile_RF14[];
extern const VpProfileDataType FXS_SWEDENProfile_890[];
extern const VpProfileDataType FXS_NORWAYProfile[];
extern const VpProfileDataType FXS_NORWAYProfile_RF14[];
extern const VpProfileDataType FXS_NORWAYProfile_890[];
extern const VpProfileDataType FXS_SWITZERLANDProfile[];
extern const VpProfileDataType FXS_SWITZERLANDProfile_RF14[];
extern const VpProfileDataType FXS_SWITZERLANDProfile_890[];
extern const VpProfileDataType FXS_TR57Profile[];
extern const VpProfileDataType FXS_TR57Profile_RF14[];
extern const VpProfileDataType FXS_TR57Profile_890[];
extern const VpProfileDataType FXS_UKProfile[];
extern const VpProfileDataType FXS_UKProfile_RF14[];
extern const VpProfileDataType FXS_UKProfile_890[];
extern const VpProfileDataType FXS_TAIWANProfile[];
extern const VpProfileDataType FXS_TAIWANProfile_RF14[];
extern const VpProfileDataType FXS_TAIWANProfile_890[];
extern const VpProfileDataType FXS_UNITED_ARAB_EMIRATESProfile[];
extern const VpProfileDataType FXS_UNITED_ARAB_EMIRATESProfile_RF14[];
extern const VpProfileDataType FXS_UNITED_ARAB_EMIRATESProfile_890[];
extern const VpProfileDataType FXS_CYPRUSProfile[];
extern const VpProfileDataType FXS_CYPRUSProfile_RF14[];
extern const VpProfileDataType FXS_CYPRUSProfile_890[];

/* AC Wideband Coefficients for FXS Line */
extern const VpProfileDataType FXS_WBAUSTRALIAProfile[];
extern const VpProfileDataType FXS_WBAUSTRALIAProfile_RF14[];
extern const VpProfileDataType FXS_WBAUSTRALIAProfile_890[];
extern const VpProfileDataType FXS_WBBELGIUMProfile[];
extern const VpProfileDataType FXS_WBBELGIUMProfile_RF14[];
extern const VpProfileDataType FXS_WBBELGIUMProfile_890[];
extern const VpProfileDataType FXS_WBBRAZILProfile[];
extern const VpProfileDataType FXS_WBBRAZILProfile_RF14[];
extern const VpProfileDataType FXS_WBBRAZILProfile_890[];
extern const VpProfileDataType FXS_WBDENMARKProfile[];
extern const VpProfileDataType FXS_WBDENMARKProfile_RF14[];
extern const VpProfileDataType FXS_WBDENMARKProfile_890[];
extern const VpProfileDataType FXS_WBCHILEProfile[];
extern const VpProfileDataType FXS_WBCHILEProfile_RF14[];
extern const VpProfileDataType FXS_WBCHILEProfile_890[];
extern const VpProfileDataType FXS_WBCHINAProfile[];
extern const VpProfileDataType FXS_WBCHINAProfile_RF14[];
extern const VpProfileDataType FXS_WBCHINAProfile_890[];
extern const VpProfileDataType FXS_WBCZECHProfile[];
extern const VpProfileDataType FXS_WBCZECHProfile_RF14[];
extern const VpProfileDataType FXS_WBCZECHProfile_890[];
extern const VpProfileDataType FXS_WBETSIProfile[];
extern const VpProfileDataType FXS_WBETSIProfile_RF14[];
extern const VpProfileDataType FXS_WBETSIProfile_890[];
extern const VpProfileDataType FXS_WBFINLANDProfile[];
extern const VpProfileDataType FXS_WBFINLANDProfile_RF14[];
extern const VpProfileDataType FXS_WBFINLANDProfile_890[];
extern const VpProfileDataType FXS_WBFRANCEProfile[];
extern const VpProfileDataType FXS_WBFRANCEProfile_RF14[];
extern const VpProfileDataType FXS_WBFRANCEProfile_890[];
extern const VpProfileDataType FXS_WBGERMANYProfile[];
extern const VpProfileDataType FXS_WBGERMANYProfile_RF14[];
extern const VpProfileDataType FXS_WBGERMANYProfile_890[];
extern const VpProfileDataType FXS_WBHUNGARYProfile[];
extern const VpProfileDataType FXS_WBHUNGARYProfile_RF14[];
extern const VpProfileDataType FXS_WBHUNGARYProfile_890[];
extern const VpProfileDataType FXS_WBINDIAProfile[];
extern const VpProfileDataType FXS_WBINDIAProfile_RF14[];
extern const VpProfileDataType FXS_WBINDIAProfile_890[];
extern const VpProfileDataType FXS_WBITALYProfile[];
extern const VpProfileDataType FXS_WBITALYProfile_RF14[];
extern const VpProfileDataType FXS_WBITALYProfile_890[];
extern const VpProfileDataType FXS_WBJAPANProfile[];
extern const VpProfileDataType FXS_WBJAPANProfile_RF14[];
extern const VpProfileDataType FXS_WBJAPANProfile_890[];
extern const VpProfileDataType FXS_WBMEXICOProfile[];
extern const VpProfileDataType FXS_WBMEXICOProfile_RF14[];
extern const VpProfileDataType FXS_WBMEXICOProfile_890[];
extern const VpProfileDataType FXS_WBNETHERLANDSProfile[];
extern const VpProfileDataType FXS_WBNETHERLANDSProfile_RF14[];
extern const VpProfileDataType FXS_WBNETHERLANDSProfile_890[];
extern const VpProfileDataType FXS_WBNEW_ZEALANDProfile[];
extern const VpProfileDataType FXS_WBNEW_ZEALANDProfile_RF14[];
extern const VpProfileDataType FXS_WBNEW_ZEALANDProfile_890[];
extern const VpProfileDataType FXS_WBNORTH_AMERICAProfile[];
extern const VpProfileDataType FXS_WBNORTH_AMERICAProfile_RF14[];
extern const VpProfileDataType FXS_WBNORTH_AMERICAProfile_890[];
extern const VpProfileDataType FXS_WBPOLANDProfile[];
extern const VpProfileDataType FXS_WBPOLANDProfile_RF14[];
extern const VpProfileDataType FXS_WBPOLANDProfile_890[];
extern const VpProfileDataType FXS_WBSPAINProfile[];
extern const VpProfileDataType FXS_WBSPAINProfile_RF14[];
extern const VpProfileDataType FXS_WBSPAINProfile_890[];
extern const VpProfileDataType FXS_WBNORWAYProfile[];
extern const VpProfileDataType FXS_WBNORWAYProfile_RF14[];
extern const VpProfileDataType FXS_WBNORWAYProfile_890[];
extern const VpProfileDataType FXS_WBSWEDENProfile[];
extern const VpProfileDataType FXS_WBSWEDENProfile_RF14[];
extern const VpProfileDataType FXS_WBSWEDENProfile_890[];
extern const VpProfileDataType FXS_WBSWITZERLANDProfile[];
extern const VpProfileDataType FXS_WBSWITZERLANDProfile_RF14[];
extern const VpProfileDataType FXS_WBSWITZERLANDProfile_890[];
extern const VpProfileDataType FXS_WBTR57Profile[];
extern const VpProfileDataType FXS_WBTR57Profile_RF14[]; 
extern const VpProfileDataType FXS_WBTR57Profile_890[];
extern const VpProfileDataType FXS_WBUKProfile[];
extern const VpProfileDataType FXS_WBUKProfile_RF14[]; 
extern const VpProfileDataType FXS_WBUKProfile_890[];
extern const VpProfileDataType FXS_WBTAIWANProfile[];
extern const VpProfileDataType FXS_WBTAIWANProfile_RF14[];
extern const VpProfileDataType FXS_WBTAIWANProfile_890[];
extern const VpProfileDataType FXS_WBUNITED_ARAB_EMIRATESProfile[];
extern const VpProfileDataType FXS_WBUNITED_ARAB_EMIRATESProfile_RF14[];
extern const VpProfileDataType FXS_WBUNITED_ARAB_EMIRATESProfile_890[];
extern const VpProfileDataType FXS_WBCYPRUSProfile[];
extern const VpProfileDataType FXS_WBCYPRUSProfile_RF14[];
extern const VpProfileDataType FXS_WBCYPRUSProfile_890[];

/* AC Coefficients for FXO Line */
extern const VpProfileDataType FXO_AUSTRALIAProfile[];
extern const VpProfileDataType FXO_AUSTRALIAProfile_890[];
extern const VpProfileDataType FXO_BELGIUMProfile[];
extern const VpProfileDataType FXO_BELGIUMProfile_890[];
extern const VpProfileDataType FXO_BRAZILProfile[];
extern const VpProfileDataType FXO_BRAZILProfile_890[];
extern const VpProfileDataType FXO_DENMARKProfile[];
extern const VpProfileDataType FXO_DENMARKProfile_890[];
extern const VpProfileDataType FXO_CHILEProfile[];
extern const VpProfileDataType FXO_CHILEProfile_890[];
extern const VpProfileDataType FXO_CHINAProfile[];
extern const VpProfileDataType FXO_CHINAProfile_890[];
extern const VpProfileDataType FXO_CZECHProfile[];
extern const VpProfileDataType FXO_CZECHProfile_890[];
extern const VpProfileDataType FXO_ETSIProfile[];
extern const VpProfileDataType FXO_ETSIProfile_890[];
extern const VpProfileDataType FXO_FINLANDProfile[];
extern const VpProfileDataType FXO_FINLANDProfile_890[];
extern const VpProfileDataType FXO_FINLAND_2Profile_890[];
extern const VpProfileDataType FXO_FRANCEProfile[];
extern const VpProfileDataType FXO_FRANCEProfile_890[];
extern const VpProfileDataType FXO_GERMANYProfile[];
extern const VpProfileDataType FXO_GERMANYProfile_890[]; 
extern const VpProfileDataType FXO_HUNGARYProfile[];
extern const VpProfileDataType FXO_HUNGARYProfile_890[];
extern const VpProfileDataType FXO_INDIAProfile[];
extern const VpProfileDataType FXO_INDIAProfile_890[] ;
extern const VpProfileDataType FXO_ITALYProfile[];
extern const VpProfileDataType FXO_ITALYProfile_890[];
extern const VpProfileDataType FXO_JAPANProfile[];
extern const VpProfileDataType FXO_JAPANProfile_890[];
extern const VpProfileDataType FXO_MEXICOProfile[];
extern const VpProfileDataType FXO_MEXICOProfile_890[];
extern const VpProfileDataType FXO_NETHERLANDSProfile[];
extern const VpProfileDataType FXO_NETHERLANDSProfile_890[];
extern const VpProfileDataType FXO_NEW_ZEALANDProfile[];
extern const VpProfileDataType FXO_NEW_ZEALANDProfile_890[];
extern const VpProfileDataType FXO_NORTH_AMERICAProfile[];
extern const VpProfileDataType FXO_NORTH_AMERICAProfile_890[];
extern const VpProfileDataType FXO_POLANDProfile[];
extern const VpProfileDataType FXO_POLANDProfile_890[];
extern const VpProfileDataType FXO_SPAINProfile[];
extern const VpProfileDataType FXO_SPAINProfile_890[];
extern const VpProfileDataType FXO_SWEDENProfile[];
extern const VpProfileDataType FXO_SWEDENProfile_890[];
extern const VpProfileDataType FXO_NORWAYProfile[];
extern const VpProfileDataType FXO_NORWAYProfile_890[];
extern const VpProfileDataType FXO_SWITZERLANDProfile[];
extern const VpProfileDataType FXO_SWITZERLANDProfile_890[];
extern const VpProfileDataType FXO_TR57Profile[];
extern const VpProfileDataType FXO_TR57Profile_890[];
extern const VpProfileDataType FXO_UKProfile[];
extern const VpProfileDataType FXO_UKProfile_890[];
extern const VpProfileDataType FXO_TAIWANProfile[];
extern const VpProfileDataType FXO_TAIWANProfile_890[];
extern const VpProfileDataType FXO_UNITED_ARAB_EMIRATESProfile[];
extern const VpProfileDataType FXO_UNITED_ARAB_EMIRATESProfile_890[];
extern const VpProfileDataType FXO_CYPRUSProfile[];
extern const VpProfileDataType FXO_CYPRUSProfile_890[];
extern const VpProfileDataType FXO_CYPRUS_2Profile_890[];


/* FXO/Dialing profile configuration */
/* Older VE880 Dialing Profiles */
extern const VpProfileDataType FXO_DIALING_DFLT[];
extern const VpProfileDataType FXO_DIALING_FRANCE[];
extern const VpProfileDataType FXO_DIALING_SPAIN[];
extern const VpProfileDataType FXO_DIALING_REGION1[];

/* VE880 and VE890 Dialing Profiles */
extern const VpProfileDataType FXO_DIALING_AUSTRALIA_Profile[];
extern const VpProfileDataType FXO_DIALING_AUSTRALIA_Profile_890[];
extern const VpProfileDataType FXO_DIALING_BELGIUM_Profile[];
extern const VpProfileDataType FXO_DIALING_BELGIUM_Profile_890[];
extern const VpProfileDataType FXO_DIALING_BRAZIL_Profile[];
extern const VpProfileDataType FXO_DIALING_BRAZIL_Profile_890[];
extern const VpProfileDataType FXO_DIALING_DENMARK_Profile[];
extern const VpProfileDataType FXO_DIALING_DENMARK_Profile_890[];
extern const VpProfileDataType FXO_DIALING_CHILE_Profile[];
extern const VpProfileDataType FXO_DIALING_CHILE_Profile_890[];
extern const VpProfileDataType FXO_DIALING_CHINA_Profile[];
extern const VpProfileDataType FXO_DIALING_CHINA_Profile_890[];
extern const VpProfileDataType FXO_DIALING_CZECH_Profile[];
extern const VpProfileDataType FXO_DIALING_CZECH_Profile_890[];
extern const VpProfileDataType FXO_DIALING_ETSI_Profile[];
extern const VpProfileDataType FXO_DIALING_ETSI_Profile_890[];
extern const VpProfileDataType FXO_DIALING_FINLAND_Profile[];
extern const VpProfileDataType FXO_DIALING_FINLAND_Profile_890[];
extern const VpProfileDataType FXO_DIALING_FRANCE_Profile[];
extern const VpProfileDataType FXO_DIALING_FRANCE_Profile_890[];
extern const VpProfileDataType FXO_DIALING_GERMANY_Profile[];
extern const VpProfileDataType FXO_DIALING_GERMANY_Profile_890[];
extern const VpProfileDataType FXO_DIALING_HUNGARY_Profile[];
extern const VpProfileDataType FXO_DIALING_HUNGARY_Profile_890[];
extern const VpProfileDataType FXO_DIALING_INDIA_Profile[];
extern const VpProfileDataType FXO_DIALING_INDIA_Profile_890[];
extern const VpProfileDataType FXO_DIALING_ITALY_Profile[];
extern const VpProfileDataType FXO_DIALING_ITALY_Profile_890[];
extern const VpProfileDataType FXO_DIALING_JAPAN_Profile[];
extern const VpProfileDataType FXO_DIALING_JAPAN_Profile_890[];
extern const VpProfileDataType FXO_DIALING_MEXICO_Profile[];
extern const VpProfileDataType FXO_DIALING_MEXICO_Profile_890[];
extern const VpProfileDataType FXO_DIALING_NETHERLANDS_Profile[];
extern const VpProfileDataType FXO_DIALING_NETHERLANDS_Profile_890[];
extern const VpProfileDataType FXO_DIALING_NEW_ZEALAND_Profile[];
extern const VpProfileDataType FXO_DIALING_NEW_ZEALAND_Profile_890[];
extern const VpProfileDataType FXO_DIALING_NORTH_AMERICA_Profile[];
extern const VpProfileDataType FXO_DIALING_NORTH_AMERICA_Profile_890[];
extern const VpProfileDataType FXO_DIALING_POLAND_Profile[];
extern const VpProfileDataType FXO_DIALING_POLAND_Profile_890[];
extern const VpProfileDataType FXO_DIALING_SPAIN_Profile[];
extern const VpProfileDataType FXO_DIALING_SPAIN_Profile_890[];
extern const VpProfileDataType FXO_DIALING_SWEDEN_Profile[];
extern const VpProfileDataType FXO_DIALING_SWEDEN_Profile_890[];
extern const VpProfileDataType FXO_DIALING_NORWAY_Profile[];
extern const VpProfileDataType FXO_DIALING_NORWAY_Profile_890[];
extern const VpProfileDataType FXO_DIALING_SWITZERLAND_Profile[];
extern const VpProfileDataType FXO_DIALING_SWITZERLAND_Profile_890[];
extern const VpProfileDataType FXO_DIALING_TR57_Profile[];
extern const VpProfileDataType FXO_DIALING_TR57_Profile_890[];
extern const VpProfileDataType FXO_DIALING_UK_Profile[];
extern const VpProfileDataType FXO_DIALING_UK_Profile_890[];
extern const VpProfileDataType FXO_DIALING_TAIWAN_Profile[];
extern const VpProfileDataType FXO_DIALING_TAIWAN_Profile_890[];
extern const VpProfileDataType FXO_DIALING_UNITED_ARAB_EMIRATES_Profile[];
extern const VpProfileDataType FXO_DIALING_UNITED_ARAB_EMIRATES_Profile_890[];
extern const VpProfileDataType FXO_DIALING_CYPRUS_Profile[];
extern const VpProfileDataType FXO_DIALING_CYPRUS_Profile_890[];

/************** DC_Feed_Parameters **************/
/* DC Profiles for all countries */
extern const VpProfileDataType DC_25MA_CC[];
extern const VpProfileDataType DC_25MA_CC_890[];
extern const VpProfileDataType DC_22MA_CC[];         /* 22mA  current feed */

/************** Ring_Signal_Parameters **************/
extern const VpProfileDataType RING_20HZ_SINE[];
extern const VpProfileDataType RING_20HZ_SINE_890[];
extern const VpProfileDataType RING_25HZ_SINE[];
extern const VpProfileDataType RING_25HZ_SINE_890[];
extern const VpProfileDataType RING_50HZ_SINE[];
extern const VpProfileDataType RING_50HZ_SINE_890[];
extern const VpProfileDataType RING_16HZ_SINE_890[];

extern const VpProfileDataType RING_25HZ_SINE_890HV[];/* Default Ringing 25 Hz 75 Vrms Tracking - Use for all countries unless country file exists */
extern const VpProfileDataType RING_50HZ_SINE_890HV[];/* Ringing 50 Hz 75 Vrms Tracking */
extern const VpProfileDataType RING_16HZ_SINE_890HV[];/* Ringing 16 Hz 75 Vrms Tracking */
extern const VpProfileDataType RING_20HZ_SINE_890HV[];/* Ringing 20 Hz 75 Vrms Tracking */

extern const VpProfileDataType RING_ZL880_ABS120V_DEF[];
extern const VpProfileDataType RING_ZL880_ABS120V_AT[];
extern const VpProfileDataType RING_ZL880_ABS120V_FI[];
extern const VpProfileDataType RING_ZL880_ABS120V_FR[];
extern const VpProfileDataType RING_ZL880_ABS120V_JP[];
extern const VpProfileDataType RING_ZL880_ABS120V_TW[];

extern const VpProfileDataType RING_ZL880_ABS100V_DEF[];
extern const VpProfileDataType RING_ZL880_ABS100V_AT[];
extern const VpProfileDataType RING_ZL880_ABS100V_CA[];
extern const VpProfileDataType RING_ZL880_ABS100V_FI[];
extern const VpProfileDataType RING_ZL880_ABS100V_FR[];
extern const VpProfileDataType RING_ZL880_ABS100V_HK[];
extern const VpProfileDataType RING_ZL880_ABS100V_JP[];
extern const VpProfileDataType RING_ZL880_ABS100V_KR[];
extern const VpProfileDataType RING_ZL880_ABS100V_SG[];
extern const VpProfileDataType RING_ZL880_ABS100V_TW[];
extern const VpProfileDataType RING_ZL880_ABS100V_US[];
extern const VpProfileDataType RING_ZL880_FB100V_DEF[];
extern const VpProfileDataType RING_ZL880_FB100V_AT[];
extern const VpProfileDataType RING_ZL880_FB100V_CA[];
extern const VpProfileDataType RING_ZL880_FB100V_FI[];
extern const VpProfileDataType RING_ZL880_FB100V_FR[];
extern const VpProfileDataType RING_ZL880_FB100V_HK[];
extern const VpProfileDataType RING_ZL880_FB100V_JP[];
extern const VpProfileDataType RING_ZL880_FB100V_KR[];
extern const VpProfileDataType RING_ZL880_FB100V_SG[];
extern const VpProfileDataType RING_ZL880_FB100V_TW[];
extern const VpProfileDataType RING_ZL880_FB100V_US[];

#define FXS_AUSTRALIARingProfile                   RING_25HZ_SINE
#define FXS_AUSTRALIARingProfile_886               RING_ZL880_ABS100V_AT
#define FXS_AUSTRALIARingProfile_887               RING_ZL880_FB100V_AT
#define FXS_AUSTRALIARingProfile_890               RING_25HZ_SINE_890
#define FXS_AUSTRALIARingProfile_890HV             RING_25HZ_SINE_890HV
#define FXS_BELGIUMRingProfile                     RING_25HZ_SINE
#define FXS_BELGIUMRingProfile_890                 RING_25HZ_SINE_890
#define FXS_BELGIUMRingProfile_890HV               RING_25HZ_SINE_890HV
#define FXS_BRAZILRingProfile                      RING_25HZ_SINE
#define FXS_BRAZILRingProfile_890                  RING_25HZ_SINE_890
#define FXS_BRAZILRingProfile_890HV                RING_25HZ_SINE_890HV
#define FXS_DENMARKRingProfile                     RING_25HZ_SINE
#define FXS_DENMARKRingProfile_890                 RING_25HZ_SINE_890
#define FXS_DENMARKRingProfile_890HV               RING_25HZ_SINE_890HV
#define FXS_CHILERingProfile                       RING_25HZ_SINE
#define FXS_CHILERingProfile_890                   RING_25HZ_SINE_890
#define FXS_CHILERingProfile_890HV                 RING_25HZ_SINE_890HV
#define FXS_CHINARingProfile                       RING_25HZ_SINE
#define FXS_CHINARingProfile_890                   RING_25HZ_SINE_890
#define FXS_CHINARingProfile_890HV                 RING_25HZ_SINE_890HV
#define FXS_CZECHRingProfile                       RING_25HZ_SINE
#define FXS_CZECHRingProfile_890                   RING_25HZ_SINE_890
#define FXS_CZECHRingProfile_890HV                 RING_25HZ_SINE_890HV
#define FXS_ETSIRingProfile                        RING_25HZ_SINE
#define FXS_ETSIRingProfile_890                    RING_25HZ_SINE_890
#define FXS_ETSIRingProfile_890HV                  RING_25HZ_SINE_890HV
#define FXS_FINLANDRingProfile                     RING_50HZ_SINE
#define FXS_FINLANDRingProfile_886                 RING_ZL880_ABS100V_FI
#define FXS_FINLANDRingProfile_887                 RING_ZL880_FB100V_FI
#define FXS_FINLANDRingProfile_890                 RING_50HZ_SINE_890
#define FXS_FINLANDRingProfile_890HV               RING_50HZ_SINE_890HV
#define FXS_FRANCERingProfile                      RING_50HZ_SINE
#define FXS_FRANCERingProfile_886                  RING_ZL880_ABS100V_FR
#define FXS_FRANCERingProfile_887                  RING_ZL880_FB100V_FR
#define FXS_FRANCERingProfile_890                  RING_50HZ_SINE_890
#define FXS_FRANCERingProfile_890HV                RING_50HZ_SINE_890HV
#define FXS_GERMANYRingProfile                     RING_25HZ_SINE
#define FXS_GERMANYRingProfile_890                 RING_25HZ_SINE_890
#define FXS_GERMANYRingProfile_890HV               RING_25HZ_SINE_890HV
#define FXS_HUNGARYRingProfile                     RING_25HZ_SINE
#define FXS_HUNGARYRingProfile_890                 RING_25HZ_SINE_890
#define FXS_HUNGARYRingProfile_890HV               RING_25HZ_SINE_890HV
#define FXS_INDIARingProfile                       RING_25HZ_SINE
#define FXS_INDIARingProfile_890                   RING_25HZ_SINE_890
#define FXS_INDIARingProfile_890HV                 RING_25HZ_SINE_890HV
#define FXS_ITALYRingProfile                       RING_25HZ_SINE
#define FXS_ITALYRingProfile_890                   RING_25HZ_SINE_890
#define FXS_ITALYRingProfile_890HV                 RING_25HZ_SINE_890HV
#define FXS_JAPANRingProfile                       RING_20HZ_SINE
#define FXS_JAPANRingProfile_886                   RING_ZL880_ABS100V_JP
#define FXS_JAPANRingProfile_887                   RING_ZL880_FB100V_JP
#define FXS_JAPANRingProfile_890                   RING_16HZ_SINE_890
#define FXS_JAPANRingProfile_890HV                 RING_16HZ_SINE_890HV
#define FXS_MEXICORingProfile                      RING_25HZ_SINE
#define FXS_MEXICORingProfile_890                  RING_25HZ_SINE_890
#define FXS_MEXICORingProfile_890HV                RING_25HZ_SINE_890HV
#define FXS_NETHERLANDSRingProfile                 RING_25HZ_SINE
#define FXS_NETHERLANDSRingProfile_890             RING_25HZ_SINE_890
#define FXS_NETHERLANDSRingProfile_890HV           RING_25HZ_SINE_890HV
#define FXS_NEW_ZEALANDRingProfile                 RING_25HZ_SINE
#define FXS_NEW_ZEALANDRingProfile_890             RING_25HZ_SINE_890
#define FXS_NEW_ZEALANDRingProfile_890HV           RING_25HZ_SINE_890HV
#define FXS_NORTH_AMERICARingProfile               RING_20HZ_SINE
#define FXS_NORTH_AMERICARingProfile_886           RING_ZL880_ABS100V_DEF
#define FXS_NORTH_AMERICARingProfile_887           RING_ZL880_FB100V_DEF
#define FXS_NORTH_AMERICARingProfile_890           RING_20HZ_SINE_890
#define FXS_NORTH_AMERICARingProfile_890HV         RING_20HZ_SINE_890HV
#define FXS_POLANDRingProfile                      RING_25HZ_SINE
#define FXS_POLANDRingProfile_890                  RING_25HZ_SINE_890
#define FXS_POLANDRingProfile_890HV                RING_25HZ_SINE_890HV
#define FXS_SPAINRingProfile                       RING_25HZ_SINE
#define FXS_SPAINRingProfile_890                   RING_25HZ_SINE_890
#define FXS_SPAINRingProfile_890HV                 RING_25HZ_SINE_890HV
#define FXS_SWEDENRingProfile                      RING_25HZ_SINE
#define FXS_SWEDENRingProfile_890                  RING_25HZ_SINE_890
#define FXS_SWEDENRingProfile_890HV                RING_25HZ_SINE_890HV
#define FXS_NORWAYRingProfile                      RING_25HZ_SINE
#define FXS_NORWAYRingProfile_890                  RING_25HZ_SINE_890
#define FXS_NORWAYRingProfile_890HV                RING_25HZ_SINE_890HV
#define FXS_SWITZERLANDRingProfile                 RING_25HZ_SINE
#define FXS_SWITZERLANDRingProfile_890             RING_25HZ_SINE_890
#define FXS_SWITZERLANDRingProfile_890HV           RING_25HZ_SINE_890HV
#define FXS_TR57RingProfile                        RING_25HZ_SINE
#define FXS_TR57RingProfile_890                    RING_25HZ_SINE_890
#define FXS_TR57RingProfile_890HV                  RING_25HZ_SINE_890HV
#define FXS_UKRingProfile                          RING_25HZ_SINE
#define FXS_UKRingProfile_890                      RING_25HZ_SINE_890
#define FXS_UKRingProfile_890HV                    RING_25HZ_SINE_890HV
#define FXS_TAIWANRingProfile                      RING_20HZ_SINE
#define FXS_TAIWANRingProfile_886                  RING_ZL880_ABS100V_TW
#define FXS_TAIWANRingProfile_887                  RING_ZL880_FB100V_TW
#define FXS_TAIWANRingProfile_890                  RING_20HZ_SINE_890
#define FXS_TAIWANRingProfile_890HV                RING_20HZ_SINE_890HV
#define FXS_UNITED_ARAB_EMIRATESRingProfile        RING_20HZ_SINE
#define FXS_UNITED_ARAB_EMIRATESRingProfile_890    RING_20HZ_SINE_890
#define FXS_UNITED_ARAB_EMIRATESRingProfile_890HV  RING_20HZ_SINE_890HV
#define FXS_CYPRUSRingProfile                      RING_25HZ_SINE
#define FXS_CYPRUSRingProfile_890                  RING_25HZ_SINE_890
#define FXS_CYPRUSRingProfile_890HV                RING_25HZ_SINE_890HV

#define FXS_BELGIUMRingProfile_886                 RING_ZL880_ABS100V_DEF
#define FXS_BRAZILRingProfile_886                  RING_ZL880_ABS100V_DEF
#define FXS_DENMARKRingProfile_886                 RING_ZL880_ABS100V_DEF
#define FXS_CHILERingProfile_886                   RING_ZL880_ABS100V_DEF
#define FXS_CHINARingProfile_886                   RING_ZL880_ABS100V_DEF
#define FXS_CZECHRingProfile_886                   RING_ZL880_ABS100V_DEF
#define FXS_ETSIRingProfile_886                    RING_ZL880_ABS100V_DEF
#define FXS_GERMANYRingProfile_886                 RING_ZL880_ABS100V_DEF
#define FXS_HUNGARYRingProfile_886                 RING_ZL880_ABS100V_DEF
#define FXS_INDIARingProfile_886                   RING_ZL880_ABS100V_DEF
#define FXS_ITALYRingProfile_886                   RING_ZL880_ABS100V_DEF
#define FXS_NETHERLANDSRingProfile_886             RING_ZL880_ABS100V_DEF
#define FXS_NEW_ZEALANDRingProfile_886             RING_ZL880_ABS100V_DEF
#define FXS_POLANDRingProfile_886                  RING_ZL880_ABS100V_DEF
#define FXS_SPAINRingProfile_886                   RING_ZL880_ABS100V_DEF
#define FXS_SWEDENRingProfile_886                  RING_ZL880_ABS100V_DEF
#define FXS_NORWAYRingProfile_886                  RING_ZL880_ABS100V_DEF
#define FXS_SWITZERLANDRingProfile_886             RING_ZL880_ABS100V_DEF
#define FXS_TR57RingProfile_886                    RING_ZL880_ABS100V_DEF
#define FXS_UKRingProfile_886                      RING_ZL880_ABS100V_DEF
#define FXS_UNITED_ARAB_EMIRATESRingProfile_886    RING_ZL880_ABS100V_DEF
#define FXS_MEXICORingProfile_886                  RING_ZL880_ABS100V_DEF
#define FXS_CYPRUSRingProfile_886                  RING_ZL880_ABS100V_DEF

#define FXS_BELGIUMRingProfile_887                 RING_ZL880_FB100V_DEF
#define FXS_BRAZILRingProfile_887                  RING_ZL880_FB100V_DEF
#define FXS_DENMARKRingProfile_887                 RING_ZL880_FB100V_DEF
#define FXS_CHILERingProfile_887                   RING_ZL880_FB100V_DEF
#define FXS_CHINARingProfile_887                   RING_ZL880_FB100V_DEF
#define FXS_CZECHRingProfile_887                   RING_ZL880_FB100V_DEF
#define FXS_ETSIRingProfile_887                    RING_ZL880_FB100V_DEF
#define FXS_GERMANYRingProfile_887                 RING_ZL880_FB100V_DEF
#define FXS_HUNGARYRingProfile_887                 RING_ZL880_FB100V_DEF
#define FXS_INDIARingProfile_887                   RING_ZL880_FB100V_DEF
#define FXS_ITALYRingProfile_887                   RING_ZL880_FB100V_DEF
#define FXS_NETHERLANDSRingProfile_887             RING_ZL880_FB100V_DEF
#define FXS_NEW_ZEALANDRingProfile_887             RING_ZL880_FB100V_DEF
#define FXS_POLANDRingProfile_887                  RING_ZL880_FB100V_DEF
#define FXS_SPAINRingProfile_887                   RING_ZL880_FB100V_DEF
#define FXS_SWEDENRingProfile_887                  RING_ZL880_FB100V_DEF
#define FXS_NORWAYRingProfile_887                  RING_ZL880_FB100V_DEF
#define FXS_SWITZERLANDRingProfile_887             RING_ZL880_FB100V_DEF
#define FXS_TR57RingProfile_887                    RING_ZL880_FB100V_DEF
#define FXS_UKRingProfile_887                      RING_ZL880_FB100V_DEF
#define FXS_UNITED_ARAB_EMIRATESRingProfile_887    RING_ZL880_FB100V_DEF
#define FXS_MEXICORingProfile_887                  RING_ZL880_FB100V_DEF
#define FXS_CYPRUSRingProfile_887                  RING_ZL880_FB100V_DEF
#endif /* VP_PROFILE_H */

