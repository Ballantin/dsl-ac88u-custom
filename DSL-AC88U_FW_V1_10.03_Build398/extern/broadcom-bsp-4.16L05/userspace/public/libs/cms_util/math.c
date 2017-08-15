/*************************************************************************
 *
<:copyright-BRCM:2015:DUAL/GPL:standard

   Copyright (c) 2015 Broadcom 
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
 *
 ************************************************************************/

#include <stdint.h>
#include <math.h>
#include "cms_math.h"


SINT32 pointOneMicroWattsTodBm(UINT32 puw)
{
    float p = puw;
    float dbm;
    SINT32 rv;

    if (puw == 0)
    {
        return INT32_MIN;
    }

    p = p / 10000; /* Convert 0.1 uW to mW. */

    dbm = 10 * (logf(p) / logf(10));
    dbm *= 1000;

    rv = (SINT32)dbm;

    return rv;
}
