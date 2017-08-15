/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom 
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

#ifndef _BCMMII_H_
#define _BCMMII_H_

#define NUM_OF_NETDEVS 2  /* For 960333(Duna) only */

typedef struct {
  unsigned int lnk:1;
  unsigned int fdx:1;
  unsigned int spd1000:1;
  unsigned int spd100:1;
  unsigned int spd10:1;
} PHY_STAT;

#endif /* _BCMMII_H_ */
