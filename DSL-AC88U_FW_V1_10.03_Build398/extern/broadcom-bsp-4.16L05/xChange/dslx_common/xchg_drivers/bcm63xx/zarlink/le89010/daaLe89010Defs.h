/***************************************************************************
*
*     Copyright (c) 2002 Broadcom, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      Le89010 definitions
*
****************************************************************************/

#ifndef DAA_Le89010_DEFS_H
#define DAA_Le89010_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LE89010_NUM_CHANNELS                       1

/* 
** Timeslot selection.
** 
** The number programmed into the 89010 represents the number of bytes after the frame sync where the 
**  DAA data begins. If configuration was for 16 bit samples, select the third sample
**  (timeslots 4 and 5) we set the timeslot value of 4
*/




#define Le89010_MAX_CMD_LENGTH              16



#ifdef __cplusplus
}
#endif

#endif /* DAA_Le89010_DEFS_H */
