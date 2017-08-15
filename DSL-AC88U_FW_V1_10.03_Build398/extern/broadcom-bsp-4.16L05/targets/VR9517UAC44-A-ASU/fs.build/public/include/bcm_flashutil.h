/***********************************************************************
 *
 *  Copyright (c) 2007  Broadcom Corporation
 *  All Rights Reserved
 *
 * <:label-BRCM:2011:DUAL/GPL:standard
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 ************************************************************************/


#ifndef _BCM_FLASHUTIL_H__
#define _BCM_FLASHUTIL_H_

#include <mtd/mtd-user.h>

#define FLASH_INFO_FLAG_NOR    0x0001
#define FLASH_INFO_FLAG_NAND   0x0002

#define IMAGE_VERSION_FILE_NAME "image_version"

/** Get info about the flash.  Currently, just returns the type of flash,
 *  but in the future, could return more useful info.
 *
 *  @flags (OUT)  Bit field containing info about the flash type.
 *
 *  @return CmsRet enum.
 */

int writeImageToNand( char *string, int size );
int getFlashInfo(unsigned int *flags);
int get_image_version( uint8_t *imagePtr, int imageSize, int erasesize, char *image_name, int image_name_len );


/* Incremental flashing. */

mtd_info_t *get_mtd_device_nm(const char *check, int *mtd_fd);
void put_mtd_device(mtd_info_t *mtd, int mtd_fd);
int nandEraseBlk(mtd_info_t *mtd, int blk_addr, int mtd_fd);
int nandWriteBlk(mtd_info_t *mtd, int blk_addr, int data_len, char *data_ptr,
  int mtd_fd, int write_JFFS2_clean_marker);
char *getCferamName(void);
int getSequenceNumber(int imageNumber);
char *searchCferamSeqNum(char *fname, char *dataPtr, int dataLen, int *donep,
  char **dirPP);
void updateCferamSeqNum(char *dataPtr, int seq);
int flashCferam(mtd_info_t *mtd, int mtd_fd, int rsrvd_for_cferam,
  char *cferam_ptr);
int readNvramData(void *pNvramData);
int handleCferom(mtd_info_t *mtd0, char *image_ptr, unsigned int wfiFlags,
  void *inMemNvramData_buf);
int flashCferom(char *image_ptr, int btrmFlag, int ofs, int *cferom_offset);
int validateWfiTag(void *wt, int blksize, uint32_t btrmEnabled);

unsigned int otp_is_btrm_boot(void);
unsigned int otp_is_boot_secure(void);

int getImageVersion(uint8_t *imagePtr, int imageSize, char *image_name,
  int image_name_len);

#endif /* _BCM_FLASHUTIL_H_ */
