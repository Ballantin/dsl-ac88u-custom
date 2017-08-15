/*
Copyright (c) 2008 Silicon Laboratories, Inc.
2008-09-29 13:09:53 */
/*ProSLIC API Tool Rev0.31 Beta*/


#include "proslic.h"
#include "si3226.h"

Si3226_General_Cfg Si3226_General_Configuration  = {
BO_DCDC_QCUK,
0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
0x3D70A20L, /* VBATH_EXPECT = 60.000*/
0x0L,	    /* DCDC_FSW_VTHLO */
0x0L,	    /* DCDC_FSW_VHYST */
0x151E255L, /* DCDC_VREF_MIN = 20.000*/
0x1300000L,	/* DCDC_VREF_MIN_RING */
0x200000L,	/* DCDC_FSW_NORM */
0x200000L,	/* DCDC_FSW_NORM_LO */
0x200000L,	/* DCDC_FSW_RING */
0x200000L,	/* DCDC_FSW_RING_LO */
0xD980000L, /* DCDC_DIN_LIM */
0xC00000L,	/* DCDC_VOUT_LIM */
0x0L,	    /* DCDC_DCFF_ENABLE */
0x500000L,	/* DCDC_UVHYST = 2.000*/
0x800000L,	/* DCDC_UVTHRESH = 8.000*/
0x700000L,	/* DCDC_OVTHRESH = 115.000*/
0x1500000L, /* DCDC_OITHRESH = 3.000*/
0x100000L,	/* DCDC_SWDRV_POL */
0x300000L,	/* DCDC_SWFET */
0x600000L,	/* DCDC_VREF_CTRL */
0x200000L,	/* DCDC_RNGTYPE */
0x0L,	    /* DCDC_ANA_GAIN */
0x0L,	    /* DCDC_ANA_TOFF */
0x0L,	    /* DCDC_ANA_TONMIN */
0x0L,	    /* DCDC_ANA_TONMAX */
0x0L,	    /* DCDC_ANA_DSHIFT */
0x0L,	    /* DCDC_ANA_LPOLY */
0x7FD710CL, /* COEF_P_HVIC = 1.000*/
0x369D04L,	/* P_TH_HVIC = 1500.000*/
0x0,		/* CM_CLAMP = 0.000*/
0x3F,		/* AUTO = 0.000*/
0x0,	    /* DAA_CNTL */
0x7F,	    /* IRQEN1 */
0xFF,	    /* IRQEN2 */
0xFF,	    /* IRQEN3 */
0xFF,	    /* IRQEN4 */
0x0,	    /* ENHANCE */
0,	        /* DAA_ENABLE */
0x8000000L,	/* SCALE_KAUDIO */
0x151EB80L,	/* AC_ADC_GAIN */
};

#ifdef SIVOICE_MULTI_BOM_SUPPORT

int si3226_genconf_multi_max_preset = 1;

Si3226_General_Cfg Si3226_General_Configuration_MultiBOM[]  = {
{
BO_DCDC_QCUK,
0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
0x3D70A20L, /* VBATH_EXPECT = 60.000*/
0x0L,	    /* DCDC_FSW_VTHLO */
0x0L,	    /* DCDC_FSW_VHYST */
0x151E255L, /* DCDC_VREF_MIN = 20.000*/
0x1300000L,	/* DCDC_VREF_MIN_RING */
0x200000L,	/* DCDC_FSW_NORM */
0x200000L,	/* DCDC_FSW_NORM_LO */
0x200000L,	/* DCDC_FSW_RING */
0x200000L,	/* DCDC_FSW_RING_LO */
0xD980000L, /* DCDC_DIN_LIM */
0xC00000L,	/* DCDC_VOUT_LIM */
0x0L,	    /* DCDC_DCFF_ENABLE */
0x500000L,	/* DCDC_UVHYST = 2.000*/
0x800000L,	/* DCDC_UVTHRESH = 8.000*/
0x700000L,	/* DCDC_OVTHRESH = 115.000*/
0x1500000L, /* DCDC_OITHRESH = 3.000*/
0x100000L,	/* DCDC_SWDRV_POL */
0x300000L,	/* DCDC_SWFET */
0x600000L,	/* DCDC_VREF_CTRL */
0x200000L,	/* DCDC_RNGTYPE */
0x0L,	    /* DCDC_ANA_GAIN */
0x0L,	    /* DCDC_ANA_TOFF */
0x0L,	    /* DCDC_ANA_TONMIN */
0x0L,	    /* DCDC_ANA_TONMAX */
0x0L,	    /* DCDC_ANA_DSHIFT */
0x0L,	    /* DCDC_ANA_LPOLY */
0x7FD710CL, /* COEF_P_HVIC = 1.000*/
0x369D04L,	/* P_TH_HVIC = 1500.000*/
0x0,		/* CM_CLAMP = 0.000*/
0x3F,		/* AUTO = 0.000*/
0x0,	    /* DAA_CNTL */
0x7F,	    /* IRQEN1 */
0xFF,	    /* IRQEN2 */
0xFF,	    /* IRQEN3 */
0xFF,	    /* IRQEN4 */
0x0,	    /* ENHANCE */
0,	        /* DAA_ENABLE */
0x8000000L,	/* SCALE_KAUDIO */
0x151EB80L,	/* AC_ADC_GAIN */
} /* SI3226_GEN_PARAM_QCUK */
};
#endif

Si3226_DTMFDec_Cfg Si3226_DTMFDec_Presets[] = {
	{0x2d40000L,0x1a660000L,0x2d40000L,0x6ba0000L,
		0x1dcc0000L,0x33f0000L,0xbd30000L,0x19d20000L,
		0x4150000L,0x188F0000L,0x4150000L,0xd970000L,
		0x18620000L,0xf1c0000L}
};
Si3226_GPIO_Cfg Si3226_GPIO_Configuration = {
	0,0,0,0,0,0,0
};
Si3226_CI_Cfg Si3226_CI_Presets [] = {
	{0}
};
Si3226_audioGain_Cfg Si3226_audioGain_Presets [] = {
	{0x1377080L,0},
	{0x80C3180L,0}
};

Si3226_Ring_Cfg Si3226_Ring_Presets[] ={


    /* AU_RINGING */
    /* inputs:  ringtrip ac thresh = 105.577, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
	{	
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1F6000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x98F1EFL,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	//0x58,		/* RINGCON */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* AUS_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L,	/* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* BE_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* BR_RINGING */
	/* inputs:  ringtrip ac thresh = 125.408, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x22C000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0xB5AC63L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* CHL_RINGING */
	/* inputs:  ringtrip ac thresh = 125.408, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x22C000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0xB5AC63L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* CH_RINGING */
	/* inputs:  ringtrip ac thresh = 104.583, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1F2000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x97816DL,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* CZ_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* DN_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* ETSI_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* FI_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    {
    /*  FR_RINGING
	    Loop = 500 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	    Rprot = 30 ohms, Type = BALANCED, Waveform = SINE
    */ 
    0x00040000L,	/* RTPER */
    0x079BC000L,	/* RINGFR (50.000 Hz) */
    0x0036229FL,	/* RINGAMP (35.000 vrms)  */
    0x00000000L,	/* RINGPHAS */
    0x00000000L,	/* RINGOF (0.000 vdc) */
    0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
    0x00D16348L,	/* IRING_LIM (90.000 mA) */
    0x0041C932L,	/* RTACTH (45.411 mA) */
    0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
    0x00008000L,	/* RTACDB (75.000 ms) */
    0x00008000L,	/* RTDCDB (75.000 ms) */
    0x00C49BA0L,	/* VOV_RING_BAT (12.000 v) */
    0x00000000L,	/* VOV_RING_GND (0.000 v) */
    0x0455141CL,	/* VBATR_EXPECT (67.693 v) */
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
    0x0390787CL,	/* DCDC_VREF_MIN_RNG */
    0x40,			/* RINGCON */
    0x00,			/* USERSTAT */
    0x022A8A0EL,	/* VCM_RING (30.846 v) */
    0x022A8A0EL,	/* VCM_RING_FIXED */
    0x003126E8L,	/* DELTA_VCM */
    0x00200000L,	/* DCDC_RNGTYPE */
    },
    /* GER_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* HU_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* IT_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* JP_RINGING */
	/* inputs:  ringtrip ac thresh = 125.284, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x50000L,	/* RTPER */
	0x7EFE000L, /* RINGFR */
	0x1BC000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0xE2DE05L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x6000L,	/* RTACDB */
	0x6000L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINTTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},


//MEXICO_PLACEHOLDER
    /* MX_RINGING */
	/* inputs:  ringtrip ac thresh = 125.284, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
//    { 0x7EFE000L, 0x1BC000L, 0x0L, 0x0L, 0x50000L, 0x80, 0x3E, 0x0, 0x7D, 0x58, 0x800000L, 0xC84B5BL, 0x19C3668DL, 0x0L, 0x0L, 0x51EB8L, 0x6000L, 0xE2DE05L, 0x6000L, 0xFFFFFFFL, 0x9374BCL, 0x624DD2L, 0x4CCCCCCL },
    { 
	0x50000L,	/* RTPER */
	0x7EFE000L, /* RINGFR */
	0x1BC000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0xF5E5200E, /* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x68BF58,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x6000L,	/* RTACDB */
	0x6000L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},
//MEXICO_PLACEHOLDER
   

    /* NE_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* NZ_RINGING */
	/* inputs:  ringtrip ac thresh = 100.111, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1DC000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x910725L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* NA_RINGING */
	/* inputs:  ringtrip ac thresh = 125.284, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
//    { 0x7EFE000L, 0x1BC000L, 0x0L, 0x0L, 0x50000L, 0x80, 0x3E, 0x0, 0x7D, 0x58, 0x800000L, 0xC84B5BL, 0x19C3668DL, 0x0L, 0x0L, 0x51EB8L, 0x6000L, 0xE2DE05L, 0x6000L, 0xFFFFFFFL, 0x9374BCL, 0x624DD2L, 0x4CCCCCCL },
    { 
	0x50000L,	/* RTPER */
	0x7EFE000L, /* RINGFR */
	0x1BC000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0xF5E5200E, /* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x68BF58,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x6000L,	/* RTACDB */
	0x6000L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},
   
   /* PL_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},
    
    /* SP_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 5.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x51EB85L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* SE_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* NO_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

	/* SW_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

    /* GR57_RINGING */
	/* inputs:  ringtrip ac thresh = 125.284, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x50000L,	/* RTPER */
	0x7EFE000L, /* RINGFR */
	0x1BC000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0xE2DE05L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x6000L,	/* RTACDB */
	0x6000L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},

	/* UK_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},
	
	/* TW_RINGING */
	/* inputs:  ringtrip ac thresh = 125.408, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x22C000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0xB5AC63L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},
   
   /* IN_RINGING (copy of NA_RINGING) */
	/* inputs:  ringtrip ac thresh = 125.284, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
//    { 0x7EFE000L, 0x1BC000L, 0x0L, 0x0L, 0x50000L, 0x80, 0x3E, 0x0, 0x7D, 0x58, 0x800000L, 0xC84B5BL, 0x19C3668DL, 0x0L, 0x0L, 0x51EB8L, 0x6000L, 0xE2DE05L, 0x6000L, 0xFFFFFFFL, 0x9374BCL, 0x624DD2L, 0x4CCCCCCL },
    { 
	0x50000L,	/* RTPER */
	0x7EFE000L, /* RINGFR */
	0x1BC000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0xF5E5200E, /* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x68BF58,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x6000L,	/* RTACDB */
	0x6000L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x80,		/* RINGTALO */
	0x3E,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	},


	/* UK_RINGING */
	/* inputs:  ringtrip ac thresh = 99.622, dc_thresh = 4.000, rt debounce = 0.075, vov_ring_gnd = 6.000, vov_ring_bat = 9.000*/
    { 
	0x40000L,	/* RTPER */
	0x7E6C000L, /* RINGFR */
	0x1C8000L,	/* RINGAMP */
	0x0L,		/* RINGPHAS */
	0x0L,		/* RINGOF */
	0x19C3668DL,/* SLOPE_RING */
	0xD16348L,	/* IRING_LIM = 90 mA */
	0x905182L,	/* RTACTH */
	0xFFFFFFFL, /* RTDCTH */
	0x7800L,	/* RTACDB */
	0x7800L,	/* RTDCDB */
	0x9374BCL,	/* VOV_RING_BAT */
	0x624DD2L,	/* VOV_RING_GND */
	0x70A3D3AL, /* VBATR_EXPECT = 110.000*/
	0x40,		/* RINGTALO */
	0x1F,		/* RINGTAHI */
	0x0,		/* RINGTILO */
	0x7D,		/* RINGTIHI */
	0xC84B5BL,	/* ADAP_RING_MIN_I */
	0x3000L,	/* COUNTER_IRING_VAL */
	0x66666L,	/* COUNTER_VTR_VAL */
	0x0L,		/* CONST_028 */
	0x0L,		/* CONST_032 */
	0x0L,		/* CONST_038 */
	0x0L,		/* CONST_046 */
	0x0L,		/* RRD_DELAY */
	0x0L,		/* RRD_DELAY2 */
	0x1893740L,	/* DCDC_VREF_MIN_RNG */
	0x58,		/* RINGCON */
	0x0,		/* USERSTAT */
	0x4CCCCCCL,	/* VCM_RING */
	0x4CCCCCCL,	/* VCM_RING_FIXED */
	0x3126E8L,	/* DELTA_VCM */
	0x200000L	/* DCDC_RNGTYPE */
	}
};


Si3226_DCfeed_Cfg Si3226_DCfeed_Presets[] = {

    /* AU_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 
		0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 
		0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 
		0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 
		0x8000L, 0x80000L, 0x80000L, 0x140000L, 
		0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* AUS_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* BE_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* BR_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* CHL_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* CH_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
	{ 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* CZ_DC */
    /* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* DN_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* ETSI_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* FI_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* FR_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* GER_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* HU_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* IT_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* JP_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },

//MEXICO_PLACEHOLDER
    /* MX_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
//MEXICO_PLACEHOLDER

    /* NE_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* NZ_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* NA_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* PL_DC */
    /* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* SP_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
	/* SE_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* NO_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* SW_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* GR57_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* UK_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* TW_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
    /* IN_DC (copy of NA_DC) */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=18.000,v_ilim=33.200, i_ilim=20.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1BF7C284L, 0x1FFE9CF9L, 0x40A0E0L, 0x1E036765L, 0x1E22038BL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0x7D3EA9L, 0x5D0FA6L, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  },
 /* UAE_DC */
	/* inputs: v_vlim=48.000, i_vlim=0.000, v_rfeed=42.800, i_rfeed=20.000,v_ilim=33.200, i_ilim=25.000,
 vcm_oh=27.000, vov_gnd=4.000, vov_bat=4.000, r_ring=320.000
 lcronhk=10.000, lcroffhk=12.000, lcrdbi=5.000, longhith=8.000, longloth=7.000, longdbi=5.000
 lcrmask=80.000, lcrmask_polrev=80.000, lcrmask_state=200.000, lcrmask_linecap=200.000 */
    { 0x1B7DE2B3L, 0x1F9B971FL, 0x40A0E0L, 0x1E0E6F06L, 0x1E51CFFDL, 0x5A38633L, 0x5072476L, 0x3E67006L, 0xC4C917L, 0x74538FL, 0x2D8D96L, 0x5B0AFBL, 0x6D4060L, 0x8000L, 0x48D595L, 0x3FBAE2L, 0x8000L, 0x80000L, 0x80000L, 0x140000L, 0x140000L, 0x1BA5E35L, 0x418937L, 0x418937L  }
     
};

Si3226_Impedance_Cfg Si3226_Impedance_Presets[] ={
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=220_820_120 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07139500L, 0x1FD62700L, 0x000C9000L, 0x1FFD8D80L,    /* TXACEQ */
 0x0A771280L, 0x1BCAE100L, 0x00A2D780L, 0x1FD3DE00L},   /* RXACEQ */
{0x00153680L, 0x1FD65C80L, 0x01789480L, 0x010BDE80L,    /* ECFIR/ECIIR */
 0x02291780L, 0x00D44480L, 0x0062F280L, 0x005D3900L,
 0x0016D600L, 0x1FE6E700L, 0x0CA29B80L, 0x1B54FC00L},
{0x1FBBF280L, 0x1EB92B80L, 0x018A1300L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0xCD}, 
 0x08000000L,   /* TXACGAIN */
 0x010B4F00L,   /* RXACGAIN */
 0x07BD0780L, 0x1842F900L, 0x077A0F80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* AU_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* AUS_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* BE_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* BR_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* CHL_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=200_680_100 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07725180L, 0x1FDF8B00L, 0x00097880L, 0x1FFD6400L,    /* TXACEQ */
 0x09AA3900L, 0x1D4A2D80L, 0x0070EF00L, 0x1FDF3480L},   /* RXACEQ */
{0x000AA080L, 0x00056B80L, 0x01235780L, 0x01307280L,    /* ECFIR/ECIIR */
 0x027E2A00L, 0x1F36C380L, 0x0219EE00L, 0x1E65AB80L,
 0x01438F00L, 0x1EB7AA00L, 0x0380A280L, 0x045F9380L},
{0x1DF0C080L, 0x024AEC00L, 0x1FC38C80L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0xDC}, 
 0x08000000L,   /* TXACGAIN */
 0x011A8500L,   /* RXACGAIN */
 0x07BB7680L, 0x18448A00L, 0x0776ED00L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* CH_IMPEDANCE */
/* Source: Database file:  */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x08066500L, 0x1FF6FC80L, 0x00050B00L, 0x1FFE2680L,    /* TXACEQ */
 0x0800D000L, 0x1FF6C700L, 0x00025700L, 0x1FFB2300L},   /* RXACEQ */
{0x00151F00L, 0x1FE4D400L, 0x01E78880L, 0x00573380L,    /* ECFIR/ECIIR */
 0x020AF880L, 0x1EF79A80L, 0x00B35800L, 0x1F955080L,
 0x003E6600L, 0x1FBC7880L, 0x01E9AA00L, 0x05F27E00L},
{0x025AFA00L, 0x1C1FAD80L, 0x0185EC00L, 0x0A195E00L,    /* ZSYNTH */
 0x1DE3B180L, 0x31}, 
 0x08775F80L,   /* TXACGAIN */
 0x01450700L,   /* RXACGAIN */
 0x07B41580L, 0x184BEB00L, 0x07682B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* CZ_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* DN_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* ETSI_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* FI_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* FR_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=220_820_115 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07082A00L, 0x1FD85E00L, 0x000BB780L, 0x1FFD7900L,    /* TXACEQ */
 0x0A5FF900L, 0x1BF97980L, 0x009D7300L, 0x1FD54200L},   /* RXACEQ */
{0x00244200L, 0x1FA43280L, 0x01D1D080L, 0x00BDBD80L,    /* ECFIR/ECIIR */
 0x025E3100L, 0x00CFA180L, 0x004FD000L, 0x0063AB80L,
 0x00129000L, 0x1FEB2080L, 0x0CB1E500L, 0x1B45D900L},
{0x1F635E80L, 0x1F6BE480L, 0x012FEE00L, 0x0A12B400L,    /* ZSYNTH */
 0x1DEA5880L, 0xCC}, 
 0x08000000L,   /* TXACGAIN */
 0x0109D680L,   /* RXACGAIN */
 0x07BC6300L, 0x18439D80L, 0x0778C600L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* GER_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* HU_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* IT_IMPEDANCE */
{ {0x5DFC747L, 0x1FF966F2L, 0x3B0FDL, 0x1FFEA566L, 
0x800D000L, 0x1FF6C700L, 0x25700L, 0x1FFB2300L}, 
{0x151F00L, 0x1FE4D400L, 0x1E78880L, 0x573380L, 
0x20AF880L, 0x1EF79A80L, 0xB35800L, 0x1F955080L, 
0x3E6600L, 0x1FBC7880L, 0x1E9AA00L, 0x5F27E00L}, 
{0x25AFA00L, 0x1C1FAD80L, 0x185EC00L, 0xA195E00L, 
0x1DE3B180L, 0x31}, 
0x8775F80L, 
0xEBBB95L, 
0x7B41580L, 0x184BEB00L, 0x7682B80L,
0,0}, /* JP_IMPEDANCE */

//MEXICO_PLACEHOLDER
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0805C180L, 0x1FF7F000L, 0x00051B80L, 0x1FFE2A80L,    /* TXACEQ */
 0x07FFDF00L, 0x1FF81200L, 0x00022500L, 0x1FFB2D00L},   /* RXACEQ */
{0x001F5A80L, 0x1FC03600L, 0x02263C80L, 0x001B3080L,    /* ECFIR/ECIIR */
 0x02141580L, 0x1F326E80L, 0x004D6300L, 0x1FFD5980L,
 0x1FEAD480L, 0x000F7200L, 0x02977E00L, 0x05466300L},
{0x023B8300L, 0x1C570780L, 0x016E0A00L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0x39}, 
 0x08D3B800L,   /* TXACGAIN */
 0x0152E100L,   /* RXACGAIN */
 0x07B44600L, 0x184BBA80L, 0x07688B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* MX_IMPEDANCE */ 
//MEXICO_PLACEHOLDER 

/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NE_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=370_620_310 rprot=30 rfuse=24 emi_cap=10*/
{
{0x08066500L, 0x1FF6FC80L, 0x00050B00L, 0x1FFE2680L,    /* TXACEQ */
 0x0800D000L, 0x1FF6C700L, 0x00025700L, 0x1FFB2300L},   /* RXACEQ */
{0x00151F00L, 0x1FE4D400L, 0x01E78880L, 0x00573380L,    /* ECFIR/ECIIR */
 0x020AF880L, 0x1EF79A80L, 0x00B35800L, 0x1F955080L,
 0x003E6600L, 0x1FBC7880L, 0x01E9AA00L, 0x05F27E00L},
{0x025AFA00L, 0x1C1FAD80L, 0x0185EC00L, 0x0A195E00L,    /* ZSYNTH */
 0x1DE3B180L, 0x31}, 
 0x08775F80L,   /* TXACGAIN */
 0x01450700L,   /* RXACGAIN */
 0x07B41580L, 0x184BEB00L, 0x07682B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NZ_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0805C180L, 0x1FF7F000L, 0x00051B80L, 0x1FFE2A80L,    /* TXACEQ */
 0x07FFDF00L, 0x1FF81200L, 0x00022500L, 0x1FFB2D00L},   /* RXACEQ */
{0x001F5A80L, 0x1FC03600L, 0x02263C80L, 0x001B3080L,    /* ECFIR/ECIIR */
 0x02141580L, 0x1F326E80L, 0x004D6300L, 0x1FFD5980L,
 0x1FEAD480L, 0x000F7200L, 0x02977E00L, 0x05466300L},
{0x023B8300L, 0x1C570780L, 0x016E0A00L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0x39}, 
 0x08D3B800L,   /* TXACGAIN */
 0x0152E100L,   /* RXACGAIN */
 0x07B44600L, 0x184BBA80L, 0x07688B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NA_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* PL_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* SP_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* SE_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* NO_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=30 rfuse=24 emi_cap=10*/
{
{0x07512200L, 0x1FC88180L, 0x000CEA80L, 0x1FFDA780L,    /* TXACEQ */
 0x0A8D5080L, 0x1B936400L, 0x0085C580L, 0x1FDADA00L},   /* RXACEQ */
{0x00313A00L, 0x1F52AA80L, 0x02CE3600L, 0x1ECD9D00L,    /* ECFIR/ECIIR */
 0x0494E800L, 0x1EA53200L, 0x01984A00L, 0x1FF95280L,
 0x00294F00L, 0x1FD4C300L, 0x0C611180L, 0x1B95DE80L},
{0x001AFF00L, 0x1F22FE80L, 0x00C1B800L, 0x0D3F2A00L,    /* ZSYNTH */
 0x1ABF7880L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01132500L,   /* RXACGAIN */
 0x07BBB280L, 0x18444E00L, 0x07776480L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* SW_IMPEDANCE */
{ {0x5A14D7CL, 0x1FFA0782L, 0x3296BL, 0x1FFE3819L, 
0x8081C00L, 0x1FFA1380L, 0x2DB00L, 0x1FFBA080L}, 
{0x1FD91300L, 0x1061A00L, 0x1FB00C80L, 0x5695780L, 
0x1E0FBB80L, 0x30B8E80L, 0x1E9C8200L, 0xB52A00L, 
0x11C300L, 0xF3D80L, 0x9395580L, 0x1EB58680L}, 
{0x639000L, 0x1F58B680L, 0x42ED00L, 0xA645F00L, 
0x1D953780L, 0x3A}, 
0x8000000L, 
0x9BD483L, 
0x7951E00L, 0x186AE280L, 0x72A3C00L,
0,0}, /* GR57_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=370_620_310 rprot=30 rfuse=24 emi_cap=10*/
{
{0x08066500L, 0x1FF6FC80L, 0x00050B00L, 0x1FFE2680L,    /* TXACEQ */
 0x0800D000L, 0x1FF6C700L, 0x00025700L, 0x1FFB2300L},   /* RXACEQ */
{0x00151F00L, 0x1FE4D400L, 0x01E78880L, 0x00573380L,    /* ECFIR/ECIIR */
 0x020AF880L, 0x1EF79A80L, 0x00B35800L, 0x1F955080L,
 0x003E6600L, 0x1FBC7880L, 0x01E9AA00L, 0x05F27E00L},
{0x025AFA00L, 0x1C1FAD80L, 0x0185EC00L, 0x0A195E00L,    /* ZSYNTH */
 0x1DE3B180L, 0x31}, 
 0x08775F80L,   /* TXACGAIN */
 0x01450700L,   /* RXACGAIN */
 0x07B41580L, 0x184BEB00L, 0x07682B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* UK_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0805C180L, 0x1FF7F000L, 0x00051B80L, 0x1FFE2A80L,    /* TXACEQ */
 0x07FFDF00L, 0x1FF81200L, 0x00022500L, 0x1FFB2D00L},   /* RXACEQ */
{0x001F5A80L, 0x1FC03600L, 0x02263C80L, 0x001B3080L,    /* ECFIR/ECIIR */
 0x02141580L, 0x1F326E80L, 0x004D6300L, 0x1FFD5980L,
 0x1FEAD480L, 0x000F7200L, 0x02977E00L, 0x05466300L},
{0x023B8300L, 0x1C570780L, 0x016E0A00L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0x39}, 
 0x08D3B800L,   /* TXACGAIN */
 0x0152E100L,   /* RXACGAIN */
 0x07B44600L, 0x184BBA80L, 0x07688B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },  /* TW_IMPEDANCE */
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0805C180L, 0x1FF7F000L, 0x00051B80L, 0x1FFE2A80L,    /* TXACEQ */
 0x07FFDF00L, 0x1FF81200L, 0x00022500L, 0x1FFB2D00L},   /* RXACEQ */
{0x001F5A80L, 0x1FC03600L, 0x02263C80L, 0x001B3080L,    /* ECFIR/ECIIR */
 0x02141580L, 0x1F326E80L, 0x004D6300L, 0x1FFD5980L,
 0x1FEAD480L, 0x000F7200L, 0x02977E00L, 0x05466300L},
{0x023B8300L, 0x1C570780L, 0x016E0A00L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0x39}, 
 0x08D3B800L,   /* TXACGAIN */
 0x0152E100L,   /* RXACGAIN */
 0x07B44600L, 0x184BBA80L, 0x07688B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 },    /* IN_IMPEDANCE */
 
/* Source: Database file: C:\Program Files\Silicon Laboratories\ProSLIC API General Release Version 5.6.2L\proslic_api\config_tools\cwdb.db */
/* Database information: version: 1.0.1 build date: 2010-07-21*/
/* parameters: zref=600_0_0 rprot=30 rfuse=24 emi_cap=10*/
{
{0x0805C180L, 0x1FF7F000L, 0x00051B80L, 0x1FFE2A80L,    /* TXACEQ */
 0x07FFDF00L, 0x1FF81200L, 0x00022500L, 0x1FFB2D00L},   /* RXACEQ */
{0x001F5A80L, 0x1FC03600L, 0x02263C80L, 0x001B3080L,    /* ECFIR/ECIIR */
 0x02141580L, 0x1F326E80L, 0x004D6300L, 0x1FFD5980L,
 0x1FEAD480L, 0x000F7200L, 0x02977E00L, 0x05466300L},
{0x023B8300L, 0x1C570780L, 0x016E0A00L, 0x0A12B500L,    /* ZSYNTH */
 0x1DEA5780L, 0x39}, 
 0x08D3B800L,   /* TXACGAIN */
 0x0152E100L,   /* RXACGAIN */
 0x07B44600L, 0x184BBA80L, 0x07688B80L,    /* RXACHPF */
 0, 0  /* TXGAIN, RXGAIN */
 }    /* UAE_IMPEDANCE */

};

Si3226_FSK_Cfg Si3226_FSK_Presets[] ={

    /* inputs: mark freq=1200.000, space freq2200.000, amp=0.220, baud=1200.000, startStopDis=0, interrupt depth = 0 */
    { 0x2232000L, 0x77C2000L, 0x3C0000L, 0x200000L, 
		0x6B60000L, 0x79C0000L,0, 0 }
};

Si3226_Tone_Cfg Si3226_Tone_Presets[] ={

    /* inputs:  freq1 = 350.000, amp1 = -18.000, freq2 = 350.000, amp2 = -18.000, ta1 = 0.000, ti1 = 0.000, ta2 = 0.000, ti2 = 0.000*/
    { {0x7B30000L, 0x3A000L, 0x0L, 0x0, 
		0x0, 0x0, 0x0}, 
	{0x7B30000L, 0x3A000L, 0x0L, 0x0, 
	0x0, 0x0, 0x0}, 0x46 }
};

Si3226_PCM_Cfg Si3226_PCM_Presets[] ={
	{
	0x00, 	 /* PCM_FMT - A-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK FALLING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x01 	 /* A-LAW -  INVERT NONE */
	},  /* ALAW_NEGATIVEEDGE_PCM */
	{
	0x01, 	 /* PCM_FMT - u-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK FALLING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* ULAW_NEGATIVEEDGE_PCM */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x01, 	 /* PCM_TRI - PCLK FALLING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	}    /* LINEAR_NEGATIVEEDGE_PCM */
};
