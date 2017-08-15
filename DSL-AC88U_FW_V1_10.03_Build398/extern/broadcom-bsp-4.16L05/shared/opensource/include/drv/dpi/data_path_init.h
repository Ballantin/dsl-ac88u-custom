/*
   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
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

#ifndef __DATA_PATH_INIT_H
#define __DATA_PATH_INIT_H

#if defined CONFIG_BCM96838 || defined CONFIG_BCM96848
    #include "oren_data_path_init.h"
#elif defined CONFIG_BCM963138
    #include "bcm63138_data_path_init.h"
#elif defined CONFIG_BCM963148
    #include "bcm63138_data_path_init.h"
#else
    #error "did not included the data_path_init.h correctly!!!"
#endif

#endif /* __DATA_PATH_INIT_H */
