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


#ifndef _OPENSSL_CONF_LIB_H_
#define _OPENSSL_CONF_LIB_H_

#include <openssl/ssl.h>


#define SSLCONF_FILE_NAME           "/etc/openssl/sslconf.xml"
#define SSLCONF_FILE_SIZE_MAX       (4*1024)
#define SSLCONF_BUFF_SIZE            1024


typedef enum
{
   SSLCONF_RET_SUCCESS              = 0,
   SSLCONF_RET_OPEN_FILE_ERROR      = 1,
   SSLCONF_RET_XML_STATUS_ERROR     = 2,
   SSLCONF_RET_CONF_CTX_ERROR       = 3,
   SSLCONF_RET_INTERNAL_ERROR       = 4
} SslConfRet;


typedef struct {
    char appName[SSLCONF_BUFF_SIZE];
    SSL_CONF_CTX *cctx;
} SslConfData_t;


SslConfRet sslconf_apply_config(const char *appName, SSL_CTX *ctx);

#endif

