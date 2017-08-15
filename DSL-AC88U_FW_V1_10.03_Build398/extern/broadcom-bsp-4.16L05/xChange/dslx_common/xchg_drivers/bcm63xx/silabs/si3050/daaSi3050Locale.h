/***************************************************************************
*
*    Copyright 2005  Broadcom
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
*
*    Description:
*		This is the register definition for the different country supported.
*
****************************************************************************/
#ifndef __DAA_SI3050_LOCALE_H__INCLUDED__
#define __DAA_SI3050_LOCALE_H__INCLUDED__

/* ---- Constants and Types ---------------------------------- */

/**
     AUSTRALIA  (Spark quenching, according to Si3050 datasheet)
**/
const SI3050_REGISTER_VALUE si3050AUSTRALIAArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x01 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x00 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x03 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x03 },
   { REG_SPARKQUENCHING, FIELD_SQ1,   0x01 },
   { REG_SPARKQUENCHING, FIELD_SQ2,   0x01 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     BELGIUM
**/
const SI3050_REGISTER_VALUE si3050BELGIUMArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     BRAZIL
**/
const SI3050_REGISTER_VALUE si3050BRAZILArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x00 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x03 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     CHILE
**/
const SI3050_REGISTER_VALUE si3050CHILEArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x00 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x03 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     CHINA
**/
const SI3050_REGISTER_VALUE si3050CHINAArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x00 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x03 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x0F },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**  
     CZECH REPUBLIC
**/
const SI3050_REGISTER_VALUE si3050CZECHArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x01 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     DENMARK
**/
const SI3050_REGISTER_VALUE si3050DENMARKArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     ETSI (Using TBR21)
**/
const SI3050_REGISTER_VALUE si3050ETSIArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     FINLAND
**/
const SI3050_REGISTER_VALUE si3050FINLANDArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     FRANCE
**/
const SI3050_REGISTER_VALUE si3050FRANCEArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     GERMANY
**/
const SI3050_REGISTER_VALUE si3050GERMANYArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x03 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     HUNGARY
**/
const SI3050_REGISTER_VALUE si3050HUNGARYArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     INDIA
**/
const SI3050_REGISTER_VALUE si3050INDIAArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     ITALY
**/
const SI3050_REGISTER_VALUE si3050ITALYArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     JAPAN
**/
const SI3050_REGISTER_VALUE si3050JAPANArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x00 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x03 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     MEXICO (COPY OF NA)
**/
const SI3050_REGISTER_VALUE si3050MEXICOArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     NETHERLANDS
**/
const SI3050_REGISTER_VALUE si3050NETHERLANDSArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   { REG_RINGVAL3,       FIELD_RNGV,  0x01 },  //Ringvalidation on
   { REG_RINGVAL1,       FIELD_RDLY10,0x01 },  //256ms
   { REG_RINGVAL2,       FIELD_RDLY2, 0x00 },  //
   { REG_RINGVAL2,       FIELD_RTO,   0x02 },  //256ms
   { REG_RINGVAL2,       FIELD_RCC ,  0x03 },  //256ms
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     NEW_ZEALAND
**/
const SI3050_REGISTER_VALUE si3050NEW_ZEALANDArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x04 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     NORTH_AMERICA
**/
const SI3050_REGISTER_VALUE si3050NORTH_AMERICAArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**  
     POLAND (HACK: Manually copied from CZECH REPUBLIC)
     Needs to be regenerated using Silabs tools
**/
const SI3050_REGISTER_VALUE si3050POLANDArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x01 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     SPAIN
**/
const SI3050_REGISTER_VALUE si3050SPAINArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     SWEDEN
**/
const SI3050_REGISTER_VALUE si3050SWEDENArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     NORWAY
**/
const SI3050_REGISTER_VALUE si3050NORWAYArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x08 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     SWITZERLAND
**/
const SI3050_REGISTER_VALUE si3050SWITZERLANDArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     TR57
**/
const SI3050_REGISTER_VALUE si3050TR57Archive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     UK
**/
const SI3050_REGISTER_VALUE si3050UKArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x05 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};


/**
     TAIWAN
**/
const SI3050_REGISTER_VALUE si3050TAIWANArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x00 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x00 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x00 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     UNITED ARAB EMIRATES (COPY OF UK)
**/
const SI3050_REGISTER_VALUE si3050UNITED_ARAB_EMIRATESArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x05 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};

/**
     CYPRUS (COPY OF FINLAND)
**/
const SI3050_REGISTER_VALUE si3050CYPRUSArchive[] =
{
   { REG_INTERNAT1,      FIELD_OHS,   0x00 },
   { REG_DAACONTROL5,    FIELD_OHS2,  0x01 },
   { REG_INTERNAT1,      FIELD_RZ,    0x00 },
   { REG_INTERNAT1,      FIELD_RT,    0x00 },
   { REG_DCTERMCONTROL,  FIELD_ILIM,  0x01 },
   { REG_DCTERMCONTROL,  FIELD_DCV,   0x03 },
   { REG_DCTERMCONTROL,  FIELD_MINI,  0x00 },
   { REG_ACTERMCONTROL,  FIELD_ACIM,  0x02 },
   /* Terminate the array with an INVALID register address. */
   { REG_INVALID,        0,           0x00 },
};
#endif   /* __DAA_SI3050_LOCALE_H__INCLUDED__ */
