/***********************************************************************
 *
 *  Copyright (c) 2015  Broadcom Corporation
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


#include <stdio.h>
#include <stdbool.h>
#include <expat/expat.h>

#include "bcm_sslconf.h"

//#define SSLCONF_DEBUG

static bool app_found = false;
static char value[SSLCONF_BUFF_SIZE];


void start_element(void *data, const char *element, const char **attribute)
{
    SslConfData_t *conf = (SslConfData_t *)data;

    if (strcmp(conf->appName, element) == 0)
    {
        app_found = true;

#ifdef SSLCONF_DEBUG
        printf("Start SSL_CONF for: '%s'\n", element);
#endif // SSLCONF_DEBUG
    }
}


void end_element(void *data, const char *element)
{
    int ret = 0;
    SslConfData_t *conf = (SslConfData_t *)data;

    if (app_found == true &&
        element[0] != '\0' &&
        strcmp(conf->appName, element) != 0)
    {
        ret = SSL_CONF_cmd(conf->cctx, element, value);

        switch (ret)
        {
            case 1:
                printf("SSL_CONF_cmd - value is recognised but NOT used: command '%s', value '%s'\n",
                       element, value);
                break;
            case 0:
                printf("SSL_CONF_cmd - error occurred while performing operation: command '%s', value '%s'\n",
                       element, value);
                break;
            case -2:
                printf("SSL_CONF_cmd - command is NOT recognised: command '%s', value '%s'\n",
                       element, value);
                break;
            case -3:
                printf("SSL_CONF_cmd - value is NULL: command '%s', value '%s'\n",
                       element, value);
                break;
            default:
#ifdef SSLCONF_DEBUG
                printf("SSL_CONF_cmd: command '%s', value '%s'\n",
                       element, value);
#endif // SSLCONF_DEBUG
                break;
        }
    }

    if (strcmp(conf->appName, element) == 0)
    {
        SSL_CONF_CTX_finish(conf->cctx);

        app_found = false;

#ifdef SSLCONF_DEBUG
        printf("End SSL_CONF for: '%s'\n", element);
#endif // SSLCONF_DEBUG
    }
}


void char_data (void *data, const XML_Char *s, int len)
{
    if (app_found == true && len > 0)
    {
        if (len > SSLCONF_BUFF_SIZE)
            len = SSLCONF_BUFF_SIZE;

        memset(value, 0, SSLCONF_BUFF_SIZE);
        strncpy(value, s, len);
    }
}


SslConfRet sslconf_apply_config(const char *appName, SSL_CTX *ctx)
{
    int len = 0;
    char buff[SSLCONF_FILE_SIZE_MAX];
    FILE *fp = NULL;
    XML_Parser parser = XML_ParserCreate(NULL);
    SslConfData_t data;
    SslConfRet ret = SSLCONF_RET_SUCCESS;

    fp = fopen(SSLCONF_FILE_NAME, "r");
    if (fp == NULL)
    {
        printf("Failed to open file '%s'\n", SSLCONF_FILE_NAME);
        return SSLCONF_RET_OPEN_FILE_ERROR;
    }

    memset(&data, 0, sizeof(SslConfData_t));

    data.cctx = SSL_CONF_CTX_new();
    if (data.cctx == NULL)
    {
        fclose(fp);
        printf("Failed to allocates and initialises an SSL_CONF_CTX\n");
        return SSLCONF_RET_CONF_CTX_ERROR;
    }

    SSL_CONF_CTX_set_ssl_ctx(data.cctx, ctx);
    SSL_CONF_CTX_set_flags(data.cctx, SSL_CONF_FLAG_FILE);
    SSL_CONF_CTX_set_flags(data.cctx, SSL_CONF_FLAG_CLIENT);
    SSL_CONF_CTX_set_flags(data.cctx, SSL_CONF_FLAG_SERVER);
    SSL_CONF_CTX_set_flags(data.cctx, SSL_CONF_FLAG_SHOW_ERRORS);

    memcpy(data.appName, appName, SSLCONF_BUFF_SIZE);

    XML_SetUserData(parser, &data);
    XML_SetElementHandler(parser, start_element, end_element);
    XML_SetCharacterDataHandler(parser, char_data);

    memset(buff, 0, SSLCONF_FILE_SIZE_MAX);
    len = fread(buff, 1, SSLCONF_FILE_SIZE_MAX, fp);
    fclose(fp);

    /* parse the xml */
    if (XML_Parse(parser, buff, len, XML_TRUE) == XML_STATUS_ERROR)
    {
        printf("Error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));
        ret = SSLCONF_RET_XML_STATUS_ERROR;
    }

    XML_ParserFree(parser);

    SSL_CONF_CTX_free(data.cctx);

    return ret;
}
