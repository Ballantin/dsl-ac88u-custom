/***********************************************************************
 *
 *  Copyright (c) 2014  Broadcom Corporation
 *  All Rights Reserved
 *
 * <:label-BRCM:2006:DUAL/GPL:standard
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
 *
 ************************************************************************/


#include "httpdshared.h"

static struct { 
    unsigned long keyHash;
    unsigned long  sig;
} gCtx = {0};

static unsigned long power(unsigned long base, unsigned long exp) {
    if (exp == 0)
        return 1;
    else if (exp % 2)
        return base * power(base, exp - 1);
    else {
        int temp = power(base, exp / 2);
        return temp * temp;
    }
}

static unsigned long encrypt(unsigned long key, unsigned long sig ) {
    // a simple one-way encryption algorithm which prevents us from having 
    // to store the session key in the clear.
    unsigned long hash = 0;
    int i;
    int bits = sizeof(sig)*8;
    for (i = 0; i < bits; i++) {
        if ( (sig >> i) & 0x1 ) {
            hash = (((hash >> 3)+(hash&0x7 << (bits-3)))) ^ power(key, i);
        } else {
            hash ^= 0x50505050;
        }
    }
    return hash;
}

void hsl_setSessionKey(unsigned long key, unsigned long sig) {
    gCtx.sig = sig;
    gCtx.keyHash = encrypt(key, sig);
}

int hsl_checkSessionKey(unsigned long key) {
    return (key && (encrypt(key, gCtx.sig) == gCtx.keyHash));
}


