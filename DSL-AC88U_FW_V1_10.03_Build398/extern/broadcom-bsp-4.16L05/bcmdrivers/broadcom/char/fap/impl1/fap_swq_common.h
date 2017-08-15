#ifndef __FAP_SWQ_COMMON_H_INCLUDED__
#define __FAP_SWQ_COMMON_H_INCLUDED__
/*
<:copyright-BRCM:2014:DUAL/GPL:standard 

   Copyright (c) 2014 Broadcom 
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/

#include "fap4ke_swq.h"

/* this structure holds information about swq  which is not changed
*  once swq is initialized, Storing this information in cached memory
*  avoids some uncached memory accesses
*/

typedef struct {
    fap4ke_SWQueue_t *swq;
    uint32  *qStart;
    uint32  *qEnd;
    uint8   msgSize;
    uint8   fapId;
    uint8   dqm;
    uint8   rsvd1;
} SWQInfo_t;

#endif /* __FAP_SWQ_COMMON_H_INCLUDED__ */
