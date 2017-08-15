/*
 * -----------------------------------------------------------------------------
 * (c) Copyright 2008, SiTel Semiconductor BV
 * All Rights Reserved
 * ------------------------------------------------------------------------------
 * SITEL SEMICONDUCTOR CONFIDENTIAL
 * ------------------------------------------------------------------------------
 * This code includes Confidential, Proprietary Information and is a Trade 
 * Secret of Sitel Semiconductor BV. All use, disclosure, and/or reproduction 
 * is prohibited unless authorized in writing.
 * ------------------------------------------------------------------------------
 *
 * Description: DIP (Dect Instruction Processor) header file
 *
 * ----------------------------------------------------------------------------- 
 * Synchronicity history: 
 * 
 *  $Log: dip.h.rca $
 *  
 *   Revision: 1.3 Mon Feb  9 17:22:14 2009 barels
 *   added DIP commands
 *  
 *   Revision: 1.2 Tue Feb  3 14:38:08 2009 snijders
 *   Updated some comment.
 *  
 *   Revision: 1.1 Thu Jan 22 16:04:10 2009 snijders
 *   Moved from directory '.../development/software_include' to 
 *   '.../development/simulation/software_include'. The directory
 *   '.../development/software_include' will be removed.
 *  
 *   Revision: 1.2 Mon Dec 22 15:08:16 2008 snijders
 *   Added official SiTel header.
 *  
 *   Revision: 1.1 Wed Jun  4 15:49:15 2008 witlox
 *   read_data, etc. Now part of sitel_sw_driver.c + generic makefile
 *  
 * ----------------------------------------------------------------------------- 
*/


#define	BR          0x01
#define	JMP         0x02
#define	JMP1        0x03
#define	RTN         0x04
#define	BK_A1       0x05
#define	WNTM1       0x06
#define	WNTP1       0x07
#define	WNT         0x08
#define	WT          0x09
#define	RFDIS       0x0A
#define	RFEN        0x0B
#define	LD_PTR      0x0C
#define	SLOTZERO    0x0D
#define	BK_A        0x0E
#define	BK_C        0x0F
#define	BR_B0       0x10
#define	BR_B1       0x11
#define	JMPF        0x12

#define	RTNF        0x14
#define	BK_A3       0x15
#define	BK_MA       0x16

#define RTNO        0x17

#define	PAG_ON      0x18
#define	PAG_OFF     0x19

#define BR_B2       0x1A
#define JMPF2       0x1B

#define	LD_PTR2     0x1C
#define JMPF0       0x1D

#define	BK_A2       0x1E
#define	BK_MC       0x1F
#define	B_RST       0x20
#define	B_ST2       0x21
#define	B_PPT       0x22
#define	B_BTFM      0x23
#define	B_XT        0x24
#define	B_BTFU      0x25
#define	B_BT2       0x25
#define	B_XOFF      0x26
#define	B_XON       0x27
#define	UNLCK       0x28
#define	B_SR        0x29
#define	B_BRFD      0x2A
#define	B_XR        0x2B
#define	EN_SL_ADJ   0x2C
#define	B_BRFU      0x2D
#define	B_RINV      0x2E
#define	B_RON       0x2F

#define	B_ST        0x31
#define	B_AT        0x32
#define	B_RC        0x33
#define	B_BT        0x34
#define	B_BTFP      0x35

#define	B_AT2       0x37
#define	B_WRS2      0x38
#define	B_WRS       0x39
#define	B_AR        0x3A

#define	B_BR        0x3C
#define	B_BR2       0x2D
#define	B_BRFP      0x3D

#define	B_AR2       0x3F
#define	D_RST       0x40

#define	D_PREP      0x44
#define	D_PREP2     0x45

#define	WSC         0x48
#define	A_RX        0x49
#define	A_TX        0x4A
#define	BCPV        0x4B
#define	U_VNMI      0x4C
#define	B_DIV4      0x4D
#define	B_DIV2      0x4E
#define	B_DIV1      0x4F
#define	D_LDK       0x50

#define	D_LDS       0x57

#define	D_WRS       0x5F
#define	U_PSC       0x60
#define	U_INT0      0x61

#define	U_VINT      0x63
#define	B_WB_OFF    0x64
#define	B_WB_ON     0x65

#define	U_INT1      0x6B

#define	U_INT2      0x6D
#define	BRK         0x6E
#define	U_INT3      0x6F

#define	B_BTDU      0x71
#define	B_BTDP      0x72
#define B_BTLP      0x75

#define	B_BRDU      0x79
#define B_BRDP      0x7A
#define B_BRLP      0x7D

#define R_LD0       0x80
#define R_LDH0      0x81
#define R_LDL0      0x82
#define R_LD1       0x84
#define R_LDH1      0x85
#define R_LDL1      0x86
#define R_LD2       0x88
#define R_LDH2      0x89
#define R_LDL2      0x8A


#define	A_RCV27     0x8B
#define	A_RCV21     0x8C
#define	A_RCV15     0x8D
#define A_RCV9      0x8E
#define	A_RCV3      0x8F

#define	B_BTLM      0x93
#define	B_BTLU      0x95

#define	BUNLCK      0x98

#define	B_BRLD      0x9A
#define	B_BRLU      0x9D

#define	B_NIC       0x9F
#define	M_INI0      0xA0
#define	M_INI1      0xA1

#define	MEN1N       0xA4
#define	MEN1        0xA5

#define	M_RST       0xA9
#define	M_CON       0xAA

#define	C_ON2       0xAE
#define	C_OFF2      0xAF

#define	M_WR        0xB9
#define	C_LD2       0xBA

#define	M_RD        0xBC

#define	A_RST       0xC0
#define	A_MUTE      0xC1
#define	A_STOFF     0xC2

#define	A_NORM      0xC5
#define	A_LDR       0xC6
#define	A_LDW       0xC7

#define	A_MTOFF     0xC9
#define	A_MUTE1     0xCA
#define	A_MTOFF1    0xCB
#define	A_STON      0xCC

#define	A_LDR1      0xCE
#define	A_LDW1      0xCF
#define	A_RST2      0xD0
#define	A_MUTE2     0xD1

#define	A_LDR2      0xD6
#define	A_LDW2      0xD7

#define	A_MTOFF2    0xD9
#define	A_MUTE3     0xDA
#define	A_MTOFF3    0xDB

#define	A_LDR3      0xDE
#define	A_LDW3      0xDF

#define	A_ST18      0xE1

#define	A_RST3      0xE3

#define	P_FAD       0xE6
#define	P_ANT       0xE7
#define	P_LD        0xE8
#define	P_EN        0xE9
#define	P_SC        0xEA
#define	A_RST1      0xEB
#define	P_LDL       0xEC
#define	P_LDH       0xED
#define	C_ON        0xEE
#define	C_OFF       0xEF

#define	C_LD        0xFA

#define SO_SLOT     0x01
#define BIAS_DCF    0x02
#define SYNTH       0x04
#define PLLCLOSED   0x08
#define MODCAL      0x10
#define DP0         0x80 

#define PADR        0x01
#define PORT1       0x02
#define PORT2       0x04
#define PORT3       0x08
#define PORT4       0x10
#define DP2         0x40
#define DP3         0x80

#define LNAMIX      0x01
#define IF          0x02
#define ADC         0x04
#define DEM         0x08
#define FAD_WINDOW  0x10
#define RSSIPH      0x20
#define DP1         0x80



