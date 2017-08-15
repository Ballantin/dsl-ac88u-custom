/*
Copyright (c) 2008 Silicon Laboratories, Inc.
2009-03-20 13:58:40 */
/*ProSLIC API Tool Rev0.31 Beta*/


#include "proslic.h"
#include "si3217x.h"

Si3217x_General_Cfg Si3217x_General_Configuration = {
/* Flags */
0x78,               /* DEVICE_KEY */
BO_DCDC_FLYBACK,    /* BOM_OPT */
BO_GDRV_NOT_INSTALLED, /* GDRV_OPTION */
VDC_7P0_20P0,       /* VDC_RANGE */
VDAA_ENABLED,       /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,  /* ZCAL_EN */
BO_STD_BOM,         /* PM_BOM */
/* Constants */
1100L,              /* I_OITHRESH (1100mA) */
900L,               /* I_OITHRESH_LO (900mA) */
1700L,              /* I_OITHRESH_HI (1700mA) */
136000L,            /* V_OVTHRESH (136000mV) */
5000L,              /* V_UVTHRESH (5000mV) */
1000L,              /* V_UVHYST (1000mV) */
/* RAM Constants */
0x00000000L,        /* DCDC_FSW_VTHLO */
0x00000000L,        /* DCDC_FSW_VHYST */
0x0048D15BL,        /* P_TH_HVIC */
0x07FEB800L,        /* COEF_P_HVIC */
0x00083120L,        /* BAT_HYST */
0x03D70A20L,        /* VBATH_EXPECT (60.00) */
0x070A3D3AL,        /* VBATR_EXPECT (110.00) */
0x0FFF0000L,        /* PWRSAVE_TIMER */
0x01999A00L,        /* OFFHOOK_THRESH */
0x00F00000L,        /* VBAT_TRACK_MIN */
0x00F00000L,        /* VBAT_TRACK_MIN_RNG */
0x00200000L,        /* DCDC_FSW_NORM */
0x00200000L,        /* DCDC_FSW_NORM_LO */
0x00200000L,        /* DCDC_FSW_RING */
0x00200000L,        /* DCDC_FSW_RING_LO */
0x0D980000L,        /* DCDC_DIN_LIM */
0x00C00000L,        /* DCDC_VOUT_LIM */
0x0ADD5500L,        /* DCDC_ANA_SCALE */
0x00800000L,        /* THERM_DBI */
0x00FFFFFFL,        /* VOV_DCDC_SLOPE */
0x00A18937L,        /* VOV_DCDC_OS */
0x00A18937L,        /* VOV_RING_BAT_DCDC */
0x00E49BA5L,        /* VOV_RING_BAT_MAX */
0x01018900L,        /* DCDC_VERR */
0x0080C480L,        /* DCDC_VERR_HYST */
0x00400000L,        /* PD_UVLO */
0x00400000L,        /* PD_OVLO */
0x00200000L,        /* PD_OCLO */
0x00400000L,        /* PD_SWDRV */
0x00000000L,        /* DCDC_UVPOL */
0x00200000L,        /* DCDC_RNGTYPE */
0x00300000L,        /* DCDC_ANA_TOFF */
0x00100000L,        /* DCDC_ANA_TONMIN */
0x00FFC000L,        /* DCDC_ANA_TONMAX */
0x00,               /* IRQEN1 */
0x00,               /* IRQEN2 */
0x03,               /* IRQEN3 */
0x00,               /* IRQEN4 */
0x30,               /* ENHANCE */
0x01,               /* DAA_CNTL */
0x3B                /* AUTO */
};


#ifdef SIVOICE_MULTI_BOM_SUPPORT

int si3217x_genconf_multi_max_preset = 9;

Si3217x_General_Cfg Si3217x_General_Configuration_MultiBOM[] = {
{/* 0: FLBK + GATE DRIVER */
0x73,                       /* DEVICE_KEY */
BO_DCDC_FLYBACK,            /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_7P0_20P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{/* 1:  PMOS BB 3-6VDC + GATE DRIVER */
0x73,                       /* DEVICE_KEY */
BO_DCDC_PMOS_BUCK_BOOST,    /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_3P0_6P0,                /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x051EB82AL,                /* VBATR_EXPECT (80.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00200000L,                /* DCDC_ANA_TONMIN */
0x01F00000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{/* 2:  PMOS BB 9-24VDC + GATE DRIVER */
0x73,                       /* DEVICE_KEY */
BO_DCDC_PMOS_BUCK_BOOST,    /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_9P0_24P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00200000L,                /* DCDC_ANA_TONMIN */
0x00D00000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{/* 3:  FLBK */
0x73,                       /* DEVICE_KEY */
BO_DCDC_FLYBACK,            /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_7P0_20P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{/* 4:  BJT BUCK-BOOST */
0x73,                       /* DEVICE_KEY */
BO_DCDC_BUCK_BOOST,         /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_8P0_16P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x06147AB2L,                /* VBATR_EXPECT (95.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00000000L,                /* DCDC_RNGTYPE */
0x00600000L,                /* DCDC_ANA_TOFF */
0x00400000L,                /* DCDC_ANA_TONMIN */
0x01500000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{ /* 5:  PMOS BB 3-6VDC */
0x73,                       /* DEVICE_KEY */
BO_DCDC_PMOS_BUCK_BOOST,    /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_3P0_6P0,                /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x051EB82AL,                /* VBATR_EXPECT (80.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00200000L,                /* DCDC_ANA_TONMIN */
0x01F00000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{ /* 6:  PMOS BB 9-24VDC */
0x73,                       /* DEVICE_KEY */
BO_DCDC_PMOS_BUCK_BOOST,    /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_9P0_24P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1100L,                      /* I_OITHRESH (1100mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1700L,                      /* I_OITHRESH_HI (1700mA) */
136000L,                    /* V_OVTHRESH (136000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00200000L,                /* DCDC_ANA_TONMIN */
0x00D00000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3B,                       /* AUTO */
},
{ /*  7. LCQC 3W */ 
0x71,                       /* DEVICE_KEY */
BO_DCDC_LCQCUK,             /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_4P5_16P0,               /* VDC_RANGE_OPT */
VDAA_DISABLED,              /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1850L,                      /* I_OITHRESH (1850mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1850L,                      /* I_OITHRESH_HI (1850mA) */
128000L,                    /* V_OVTHRESH (128000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x00,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
},
{ /*  8.  LCQC 6W */ 
0x71,                       /* DEVICE_KEY */
BO_DCDC_LCQCUK,             /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_4P5_16P0,               /* VDC_RANGE_OPT */
VDAA_DISABLED,              /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1750L,                      /* I_OITHRESH (1750mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1750L,                      /* I_OITHRESH_HI (1750mA) */
128000L,                    /* V_OVTHRESH (128000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x070A3D3AL,                /* VBATR_EXPECT (110.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00F00000L,                /* VBAT_TRACK_MIN */
0x00F00000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00200000L,                /* DCDC_RNGTYPE */
0x00300000L,                /* DCDC_ANA_TOFF */
0x00100000L,                /* DCDC_ANA_TONMIN */
0x00FFC000L,                /* DCDC_ANA_TONMAX */
0x00,                       /* IRQEN1 */
0x00,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x00,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
}
};

#endif

Si3217x_DTMFDec_Cfg Si3217x_DTMFDec_Presets[] = {
	{0x2d40000L,0x1a660000L,0x2d40000L,0x6ba0000L,0x1dcc0000L,0x33f0000L,0xbd30000L,0x19d20000L,0x4150000L,0x188F0000L,0x4150000L,0xd970000L,0x18620000L,0xf1c0000L}
};
Si3217x_GPIO_Cfg Si3217x_GPIO_Configuration = {
	0,0,0,0,0,0,0
};

Si3217x_PulseMeter_Cfg Si3217x_PulseMeter_Presets[] ={

    /* inputs:  freq = 12kHz, amp = 1.000Vrms, cal = First, ramp = 24kHz, power = Normal */
    { 0x7A2B6AL, 0x0, 0x0 }
};

Si3217x_CI_Cfg Si3217x_CI_Presets [] = {
	{0}
};
Si3217x_audioGain_Cfg Si3217x_audioGain_Presets [] = {
	{0x1377080L,0},
	{0x80C3180L,0}
};

Si3217x_Ring_Cfg Si3217x_Ring_Presets[] ={
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001F322FL,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004AAC35L,	/* RTACTH (51.546 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E6CE0CL,	/* VBATR_EXPECT (76.587 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02736706L,	/* VCM_RING (35.294 v) */
0x02736706L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* AU_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* AUS_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* BE_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001F322FL,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004AAC35L,	/* RTACTH (51.546 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E6CE0CL,	/* VBATR_EXPECT (76.587 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02736706L,	/* VCM_RING (35.294 v) */
0x02736706L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* BR_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001F322FL,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004AAC35L,	/* RTACTH (51.546 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E6CE0CL,	/* VBATR_EXPECT (76.587 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02736706L,	/* VCM_RING (35.294 v) */
0x02736706L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* CHL_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001F322FL,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004AAC35L,	/* RTACTH (51.546 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E6CE0CL,	/* VBATR_EXPECT (76.587 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02736706L,	/* VCM_RING (35.294 v) */
0x02736706L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* CH_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* CZ_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* DN_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* ETSI_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* FI_RINGING */
,
{
0x00040000L,	/* RTPER */
0x079BC000L,	/* RINGFR (50.000 Hz) */
0x0036F785L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x0041A104L,	/* RTACTH (45.303 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04631897L,	/* VBATR_EXPECT (68.548 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x000CCCCCL,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x039E7CF7L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02318C4BL,	/* VCM_RING (31.274 v) */
0x02318C4BL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* FR_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* GER_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* HU_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* IT_RINGING */
,
{
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x0018ED52L,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x005D08A4L,	/* RTACTH (51.376 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E65379L,	/* VBATR_EXPECT (76.558 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x027329BCL,	/* VCM_RING (35.279 v) */
0x027329BCL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* JP_RINGING */
,
//MEXICO_PLACEHOLDER
{
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x0018ED52L,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x005D08A4L,	/* RTACTH (51.376 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E65379L,	/* VBATR_EXPECT (76.558 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x027329BCL,	/* VCM_RING (35.279 v) */
0x027329BCL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* MX_RINGING */
,
//MEXICO_PLACEHOLDER
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* NE_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001DA2E0L,	/* RINGAMP (38.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x0046F065L,	/* RTACTH (48.969 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04B1E520L,	/* VBATR_EXPECT (73.358 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x0258F290L,	/* VCM_RING (33.679 v) */
0x0258F290L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* NZ_RINGING */
,
{
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x0018ED52L,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x005D08A4L,	/* RTACTH (51.376 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E65379L,	/* VBATR_EXPECT (76.558 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x027329BCL,	/* VCM_RING (35.279 v) */
0x027329BCL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* NA_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* PL_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* SP_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* SE_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* NO_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* SW_RINGING */
,
{
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x0018ED52L,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x005D08A4L,	/* RTACTH (51.376 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E65379L,	/* VBATR_EXPECT (76.558 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x027329BCL,	/* VCM_RING (35.279 v) */
0x027329BCL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* GR57_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* UK_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001F322FL,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004AAC35L,	/* RTACTH (51.546 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E6CE0CL,	/* VBATR_EXPECT (76.587 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02736706L,	/* VCM_RING (35.294 v) */
0x02736706L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* TW_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001F322FL,	/* RINGAMP (40.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004AAC35L,	/* RTACTH (51.546 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04E6CE0CL,	/* VBATR_EXPECT (76.587 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x02736706L,	/* VCM_RING (35.294 v) */
0x02736706L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* IN_RINGING */
,
{
0x00040000L,	/* RTPER */
0x07E6C000L,	/* RINGFR (25.000 Hz) */
0x001B4BE9L,	/* RINGAMP (35.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) revised scale factor*/ 
0x004156AEL,	/* RTACTH (45.103 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00008000L,	/* RTACDB (75.000 ms) */
0x00008000L,	/* RTDCDB (75.000 ms) */
0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x046287BEL,	/* VBATR_EXPECT (68.514 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00066666L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x40,			/* RINGCON */
0x00,			/* USERSTAT */
0x023143DFL,	/* VCM_RING (31.257 v) */
0x023143DFL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
}/* UAE_RINGING (COPY OF UK) */
};



Si3217x_DCfeed_Cfg Si3217x_DCfeed_Presets[] = {

    /* AU: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* AUS: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* BE: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* BR: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* CHL: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* CH: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* CZ: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* DN: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* ETSI: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* FI: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* FR: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* GER: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* HU: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* IT: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* JP: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },

//MEXICO_PLACEHOLDER
   /* MX: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
//MEXICO_PLACEHOLDER  

    /* NE: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* NZ: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* NA: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* PL: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* SP: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* SE: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* NO: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* SW: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* TR57: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* UK: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* TW: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* IN: inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
/* UAE(COPY OF UK): inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  }
    
};

Si3217x_Impedance_Cfg Si3217x_Impedance_Presets[] ={
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=220_820_120 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07F46C00L, 0x000E4600L, 0x00008580L, 0x1FFD6100L,    /* TXACEQ */
 0x07EF5000L, 0x0013F580L, 0x1FFDE000L, 0x1FFCB280L},   /* RXACEQ */
{0x0027CB00L, 0x1F8A8880L, 0x02801180L, 0x1F625C80L,    /* ECFIR/ECIIR */
 0x0314FB00L, 0x1E6B8E80L, 0x00C5FF00L, 0x1FC96F00L,
 0x1FFD1200L, 0x00023C00L, 0x0ED29D00L, 0x192A9400L},
{0x00810E00L, 0x1EFEBE80L, 0x00803500L, 0x0FF66D00L,    /* ZSYNTH */
 0x18099080L, 0x59}, 
 0x088E0D80L,   /* TXACGAIN */
 0x01456D80L,   /* RXACGAIN */
 0x07ABE580L, 0x18541B00L, 0x0757CB00L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* AU_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* AUS_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* BE_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* BR_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* CHL_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=200_680_100 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0778B980L, 0x1FB97E00L, 0x00030780L, 0x1FFC2580L,    /* TXACEQ */
 0x09CC0780L, 0x1D104400L, 0x0076CB80L, 0x1FDE3D80L},   /* RXACEQ */
{0x1FF64C00L, 0x00456280L, 0x00BEC500L, 0x014D3E80L,    /* ECFIR/ECIIR */
 0x02EB2B00L, 0x1E983B80L, 0x029EE280L, 0x1E7B7400L,
 0x00D19A80L, 0x1F293D80L, 0x06116D00L, 0x01D55C00L},
{0x01241700L, 0x1CB53A80L, 0x02269400L, 0x0A14BA00L,    /* ZSYNTH */
 0x1DE9D080L, 0x99}, 
 0x08000000L,   /* TXACGAIN */
 0x01152480L,   /* RXACGAIN */
 0x07B96C00L, 0x18469480L, 0x0772D800L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* CH_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* CZ_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* DN_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* ETSI_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* FI_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* FR_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=220_820_115 rprot=30 rfuse=24 emi_cap=10*/
{
{0x070AC700L, 0x1FCC7280L, 0x00098700L, 0x1FFCE080L,    /* TXACEQ */
 0x0A6A6400L, 0x1BE48B80L, 0x009F3B80L, 0x1FD56000L},   /* RXACEQ */
{0x00314700L, 0x1F6C1D80L, 0x02347480L, 0x00158B80L,    /* ECFIR/ECIIR */
 0x03173D00L, 0x0058E580L, 0x00A6DA80L, 0x004B0780L,
 0x001B1300L, 0x1FE2DE80L, 0x0C313180L, 0x1BB7FE00L},
{0x1FD95980L, 0x1ECDE680L, 0x0156F600L, 0x0A0C9600L,    /* ZSYNTH */
 0x1DEBF080L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01069C80L,   /* RXACGAIN */
 0x07BECB80L, 0x18413500L, 0x077D9700L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* GER_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* HU_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* IT_IMPEDANCE */
{ {0x5BC7AA3L, 0xA4AEDL, 0x6045L, 0x1FFE1C21L, 
		0x7EF5000L, 0x13F580L, 0x1FFDE000L, 0x1FFCB280L}, 
		{0x27CB00L, 0x1F8A8880L, 0x2801180L, 0x1F625C80L, 
		0x314FB00L, 0x1E6B8E80L, 0xC5FF00L, 0x1FC96F00L, 
		0x1FFD1200L, 0x23C00L, 0xED29D00L, 0x192A9400L}, 
		{0x810E00L, 0x1EFEBE80L, 0x803500L, 0xFF66D00L, 
		0x18099080L, 0x59}, 
		0x88E0D80L, 
		0xEF950CL, 
		0x7ABE580L, 0x18541B00L, 0x757CB00L,
		0,0 }, /* JP_IMPEDANCE */
		
//MEXICO_PLACEHOLDER
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07F3A400L, 0x000FF180L, 0x00009380L, 0x1FFDA800L,    /* TXACEQ */
 0x07EF1600L, 0x0014B500L, 0x1FFD6580L, 0x1FFCA400L},   /* RXACEQ */
{0x0008EF00L, 0x00099780L, 0x017DF600L, 0x0096B900L,    /* ECFIR/ECIIR */
 0x02549000L, 0x1E4B7D00L, 0x018EEE00L, 0x1EEE0600L,
 0x008A8080L, 0x1F713080L, 0x0489BA00L, 0x03592500L},
{0x0086CE00L, 0x1EF46980L, 0x0084CB00L, 0x0FE34F00L,    /* ZSYNTH */
 0x181CA780L, 0x5D}, 
 0x08EB8E00L,   /* TXACGAIN */
 0x01532100L,   /* RXACGAIN */
 0x07AA7180L, 0x18558F00L, 0x0754E300L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* MX_IMPEDANCE */

//MEXICO_PLACEHOLDER 		
		
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NE_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=370_620_310 rprot=30 rfuse=24 emi_cap=10*/
{
{0x08363C80L, 0x1FB03200L, 0x1FFBD200L, 0x1FFC7A00L,    /* TXACEQ */
 0x0A0D0800L, 0x1BEB0880L, 0x1F9DF080L, 0x1FE07F00L},   /* RXACEQ */
{0x00236380L, 0x1F947D00L, 0x020DE380L, 0x1FBEED00L,    /* ECFIR/ECIIR */
 0x03050300L, 0x1F7D1D00L, 0x010A9F80L, 0x00329D80L,
 0x003E4100L, 0x1FC0DF00L, 0x0DAADE80L, 0x1A4F2600L},
{0x00226100L, 0x1F8EEE80L, 0x004E9D00L, 0x0F0B9B00L,    /* ZSYNTH */
 0x18F3E580L, 0x99}, 
 0x0808D100L,   /* TXACGAIN */
 0x0131BE80L,   /* RXACGAIN */
 0x07B5C100L, 0x184A3F80L, 0x076B8200L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NZ_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07F3A400L, 0x000FF180L, 0x00009380L, 0x1FFDA800L,    /* TXACEQ */
 0x07EF1600L, 0x0014B500L, 0x1FFD6580L, 0x1FFCA400L},   /* RXACEQ */
{0x0008EF00L, 0x00099780L, 0x017DF600L, 0x0096B900L,    /* ECFIR/ECIIR */
 0x02549000L, 0x1E4B7D00L, 0x018EEE00L, 0x1EEE0600L,
 0x008A8080L, 0x1F713080L, 0x0489BA00L, 0x03592500L},
{0x0086CE00L, 0x1EF46980L, 0x0084CB00L, 0x0FE34F00L,    /* ZSYNTH */
 0x181CA780L, 0x5D}, 
 0x08EB8E00L,   /* TXACGAIN */
 0x01532100L,   /* RXACGAIN */
 0x07AA7180L, 0x18558F00L, 0x0754E300L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NA_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* PL_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* SP_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* SE_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NO_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0750E500L, 0x1FC70280L, 0x000BA980L, 0x1FFD2880L,    /* TXACEQ */
 0x0A8E2380L, 0x1B905280L, 0x00847700L, 0x1FDAFA00L},   /* RXACEQ */
{0x002C8880L, 0x1F630D80L, 0x027F7980L, 0x1F3AD200L,    /* ECFIR/ECIIR */
 0x040B8680L, 0x1F414D00L, 0x01427B00L, 0x00208200L,
 0x0026AE00L, 0x1FD71680L, 0x0C8EDB00L, 0x1B688A00L},
{0x1F657980L, 0x0096FE00L, 0x00035500L, 0x0D7FE800L,    /* ZSYNTH */
 0x1A7F1A80L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x01106B80L,   /* RXACGAIN */
 0x07BC8400L, 0x18437C80L, 0x07790880L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* SW_IMPEDANCE */
{ {0x6D6E3E2L, 0x1FF4B61CL, 0x3E645L, 0x1FFD95B6L, 
		0x809FD80L, 0x1FF5EF80L, 0x29A00L, 0x1FFBA500L}, 
		{0x1FC91500L, 0x15A6D00L, 0x1F6BC580L, 0x5DCCE80L, 
		0x1DC3DD80L, 0x2BA0900L, 0x1F0D7000L, 0x6C5980L, 
		0x273000L, 0x1FDAEB00L, 0xF8CC800L, 0x18724500L}, 
		{0x281B800L, 0x1BDA1880L, 0x1A40A00L, 0xA0E4300L, 
		0x1DEB8180L, 0x2}, 
		0x8000000L, 
		0x8DBE90L, 
		0x7981100L, 0x1867EF80L, 0x7302200L,
		0,0 }, /* GR57_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=370_620_310 rprot=30 rfuse=24 emi_cap=10*/
{
{0x08363C80L, 0x1FB03200L, 0x1FFBD200L, 0x1FFC7A00L,    /* TXACEQ */
 0x0A0D0800L, 0x1BEB0880L, 0x1F9DF080L, 0x1FE07F00L},   /* RXACEQ */
{0x00236380L, 0x1F947D00L, 0x020DE380L, 0x1FBEED00L,    /* ECFIR/ECIIR */
 0x03050300L, 0x1F7D1D00L, 0x010A9F80L, 0x00329D80L,
 0x003E4100L, 0x1FC0DF00L, 0x0DAADE80L, 0x1A4F2600L},
{0x00226100L, 0x1F8EEE80L, 0x004E9D00L, 0x0F0B9B00L,    /* ZSYNTH */
 0x18F3E580L, 0x99}, 
 0x0808D100L,   /* TXACGAIN */
 0x0131BE80L,   /* RXACGAIN */
 0x07B5C100L, 0x184A3F80L, 0x076B8200L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* UK_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07F3A400L, 0x000FF180L, 0x00009380L, 0x1FFDA800L,    /* TXACEQ */
 0x07EF1600L, 0x0014B500L, 0x1FFD6580L, 0x1FFCA400L},   /* RXACEQ */
{0x0008EF00L, 0x00099780L, 0x017DF600L, 0x0096B900L,    /* ECFIR/ECIIR */
 0x02549000L, 0x1E4B7D00L, 0x018EEE00L, 0x1EEE0600L,
 0x008A8080L, 0x1F713080L, 0x0489BA00L, 0x03592500L},
{0x0086CE00L, 0x1EF46980L, 0x0084CB00L, 0x0FE34F00L,    /* ZSYNTH */
 0x181CA780L, 0x5D}, 
 0x08EB8E00L,   /* TXACGAIN */
 0x01532100L,   /* RXACGAIN */
 0x07AA7180L, 0x18558F00L, 0x0754E300L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* TW_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07F3A400L, 0x000FF180L, 0x00009380L, 0x1FFDA800L,    /* TXACEQ */
 0x07EF1600L, 0x0014B500L, 0x1FFD6580L, 0x1FFCA400L},   /* RXACEQ */
{0x0008EF00L, 0x00099780L, 0x017DF600L, 0x0096B900L,    /* ECFIR/ECIIR */
 0x02549000L, 0x1E4B7D00L, 0x018EEE00L, 0x1EEE0600L,
 0x008A8080L, 0x1F713080L, 0x0489BA00L, 0x03592500L},
{0x0086CE00L, 0x1EF46980L, 0x0084CB00L, 0x0FE34F00L,    /* ZSYNTH */
 0x181CA780L, 0x5D}, 
 0x08EB8E00L,   /* TXACGAIN */
 0x01532100L,   /* RXACGAIN */
 0x07AA7180L, 0x18558F00L, 0x0754E300L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },    /* IN_IMPEDANCE */


/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07F3A400L, 0x000FF180L, 0x00009380L, 0x1FFDA800L,    /* TXACEQ */
 0x07EF1600L, 0x0014B500L, 0x1FFD6580L, 0x1FFCA400L},   /* RXACEQ */
{0x0008EF00L, 0x00099780L, 0x017DF600L, 0x0096B900L,    /* ECFIR/ECIIR */
 0x02549000L, 0x1E4B7D00L, 0x018EEE00L, 0x1EEE0600L,
 0x008A8080L, 0x1F713080L, 0x0489BA00L, 0x03592500L},
{0x0086CE00L, 0x1EF46980L, 0x0084CB00L, 0x0FE34F00L,    /* ZSYNTH */
 0x181CA780L, 0x5D}, 
 0x08EB8E00L,   /* TXACGAIN */
 0x01532100L,   /* RXACGAIN */
 0x07AA7180L, 0x18558F00L, 0x0754E300L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 }  /* UAE_IMPEDANCE */
};

Si3217x_FSK_Cfg Si3217x_FSK_Presets[] ={

    /* inputs: mark freq=1200.000, space freq2200.000, amp=0.220, baud=1200.000, startStopDis=0, interrupt depth = 0 */
    { 0x2232000L, 0x77C2000L, 0x3C0000L, 0x200000L, 0x6B60000L, 0x79C0000L,0, 0 }
};

Si3217x_Tone_Cfg Si3217x_Tone_Presets[] ={

    /* inputs:  freq1 = 350.000, amp1 = -18.000, freq2 = 350.000, amp2 = -18.000, ta1 = 0.000, ti1 = 0.000, ta2 = 0.000, ti2 = 0.000*/
    { 
		{0x7B30000L, 0x3A000L, 0x0L, 0x0, 0x0, 0x0, 0x0}, 
		{0x7B30000L, 0x3A000L, 0x0L, 0x0, 0x0, 0x0, 0x0}, 
		0x46 
	}
};

Si3217x_PCM_Cfg Si3217x_PCM_Presets[] ={
	{
	0x00, 	 /* PCM_FMT - A-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK FALLING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x01 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_8ALAW */   
	{
	0x01, 	 /* PCM_FMT - u-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK FALLING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_8ULAW */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK FALLING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_16LIN */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x01, 	 /* WIDEBAND - ENABLED (7kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	}    /* PCM_16LIN_WB */
};
