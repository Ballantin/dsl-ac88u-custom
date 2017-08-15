/****************************************************************************
 * <:copyright-BRCM:2013:DUAL/GPL:standard
 * 
 *    Copyright (c) 2013 Broadcom 
 *    All Rights Reserved
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
 ***************************************************************************/

#if !defined(_PCMSHIMDRV_H_)
#define _PCMSHIMDRV_H_

#if defined(__cplusplus)
extern "C" {
#endif

/* pcmshim driver is following pcm device driver major and minor number */
#define PCMSHIM_MAJOR    217
#define PCMSHIM_MINOR    0
#define PCMSHIM_DEVNAME  "pcmshim"


typedef struct {
   unsigned int* bufp;
} PCMSHIMDRV_GETBUF_PARAM, *PPCMSHIMDRV_GETBUF_PARAM;


#define PCMSHIMIOCTL_GETBUF_CMD \
    _IOWR(PCMSHIM_MAJOR, 0, PCMSHIMDRV_GETBUF_PARAM)

#define MAX_PCMSHIM_IOCTL_CMDS  1


#if defined(__cplusplus)
}
#endif

#endif /* _PCMSHIMDRV_H_ */
