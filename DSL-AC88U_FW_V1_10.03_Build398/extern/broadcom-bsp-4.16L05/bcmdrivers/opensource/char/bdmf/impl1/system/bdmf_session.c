/*
* <:copyright-BRCM:2013:GPL/GPL:standard
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
*/


/*******************************************************************
 * bdmf_session.c
 *
 * CLI engine - session management
 *
 *******************************************************************/

#include <bdmf_system.h>

#define BDMF_INTERNAL
#include <bdmf_session.h>

static bdmf_fastlock session_lock;
static bdmf_session *session_list;
static int session_module_initialized;

/*
 * Internal functions
 */

#ifndef __KERNEL__
static char *_bdmf_session_gets(bdmf_session_handle session, char *buffer, uint32_t size)
{
    const char *line = NULL;
#ifdef CONFIG_LIBEDIT
    if (session && session->el && session->history)
    {
        int line_len;
        line = (el_gets(session->el, &line_len));
        if (!line)
            return NULL;
        if (line_len > size)
        {
            bdmf_printf("%s: buffer is too short %u - got %d. Truncated\n",
                        __FUNCTION__, size, line_len);
        }
        strncpy(buffer, line, size);
        if (*line && *line != '\n' && *line != '#')
            history(session->history, &session->histevent, H_ENTER, line);
    }
    else
#endif
#ifdef CONFIG_LINENOISE
    if (session && session->ln_session)
    {
        char *ln_line = linenoise(session->ln_session, "", buffer, size);
        if (ln_line)
        {
            if (strlen(ln_line))
            {
                linenoiseHistoryAdd(session->ln_session, ln_line); /* Add to the history. */
            }
            else
            {
                strncpy(buffer, "\n", size-1);
            }
            buffer[size-1] = 0;
            line = buffer;
        }
    }
    else
#endif
    {
        if (session->parms.gets)
            line = session->parms.gets(session, buffer, size);
        else
            line = fgets(buffer, size, stdin);
    }
    return line ? buffer : NULL;
}
#endif /* #ifndef __KERNEL__ */

#ifdef CONFIG_LIBEDIT

static char * _bdmf_editline_prompt(EditLine *e)
{
    return "";
}

static int _bdmf_editline_cfn(EditLine *el, char *c)
{
    bdmf_session *session = NULL;
    char insert_buf[80];
    int c1 = bdmf_getchar();
    bdmf_errno rc;

    el_get(el, EL_CLIENTDATA, &session);
    BUG_ON(session == NULL || session->magic != BDMF_SESSION_MAGIC);

    /* ToDo: handle \t parameter extension */
    while (c1 > 0 && c1 == '\t')
    {

        const LineInfo *li = el_line(el);
        char *line = bdmf_alloc(li->cursor - li->buffer + 1);
        if (!line)
            continue;
        memcpy(line, li->buffer, li->cursor - li->buffer);
        line[li->cursor - li->buffer] = 0;
        rc = bdmf_extend(session, line, insert_buf, sizeof(insert_buf));
        bdmf_free(line);
        if (rc)
        {
            c1 = bdmf_getchar();
            continue;
        }
        el_insertstr(el, insert_buf);
        printf("\r");
        el_set(el, EL_REFRESH, NULL);
        c1 = bdmf_getchar();
    }
    if (c1 < 0)
        return -1;
    *c = c1;
    return 1;
}
#endif

/* linenoise line editing library: completion support */
#ifdef CONFIG_LINENOISE

static int _bdmf_linenoise_read_char(long fd_in, char *c)
{
    bdmf_session *session = (bdmf_session *)fd_in;
    int c1;
    if (session->parms.gets)
    {
        char *buf = session->parms.gets(session, c, 1);
        return buf ? 1 : 0;
    }
    /* Default. Read from stdio */
    c1 = bdmf_getchar();
    if (c1 < 0)
    {
        return -1;
    }
    *c = c1;
    return 1;
}

static int _bdmf_linenoise_write(long fd_out, const char *buf, size_t len)
{
    bdmf_session *session = (bdmf_session *)fd_out;
    /* Use a shortcut for len==1 - which is char-by-char input.
       bdmf_printf("%*s", buf, 1) misbehaves on vxw platform,
       possibly because it is too slow.
    */
    if (len == 1)
    {
        bdmf_putchar(buf[0]);
        return 1;
    }
    return bdmf_session_write(session, buf, len);
}

static int _bdmf_linenoise_tab(linenoiseSession *ln_session, const char *buf, int pos)
{
    bdmf_session *session = NULL;
    char *line;
    char insert_buf[80]="";
    bdmf_errno rc;
    int len;

    session = linenoiseSessionData(ln_session);
    BUG_ON(session == NULL || session->magic != BDMF_SESSION_MAGIC);
    line = bdmf_alloc(strlen(buf)+1);
    if (!line)
        return 0;
    strcpy(line, buf);
    rc = bdmf_extend(session, line, insert_buf, sizeof(insert_buf));
    bdmf_free(line);
    if (rc || !strlen(insert_buf))
        return 0;

    len = strlen(buf);
    line = bdmf_alloc(strlen(buf)+strlen(insert_buf)+1);
    if (!line)
        return 0;
    if (pos >=0 && pos < len)
    {
        strncpy(line, buf, pos);
        line[pos] = 0;
        strcat(line, insert_buf);
        strcat(line, &buf[pos]);
        pos += strlen(insert_buf);
    }
    else
    {
        strcpy(line, buf);
        strcat(line, insert_buf);
        pos = strlen(line);
    }
    linenoiseSetBuffer(ln_session, line, pos);
    bdmf_free(line);
    return 1;
}

#endif


/** Initialize session management module
 * \return
 *      0   =OK\n
 *      <0  =error code
 */
static void bdmf_session_module_init(void)
{
    bdmf_fastlock_init(&session_lock);
    session_module_initialized = 1;
}

/** Open management session */
int bdmf_session_open(const bdmf_session_parm_t *parm, bdmf_session **p_session)
{
    bdmf_session *session;
    bdmf_session **p_last_next;
    const char *name;
    char *name_clone;
    int size;

    BUG_ON(!p_session);
    BUG_ON(!parm);
    if (!p_session || !parm)
        return BDMF_ERR_PARM;
#ifndef CONFIG_EDITLINE
    if (parm->line_edit_mode == BDMF_LINE_EDIT_ENABLE)
    {
        bdmf_print("Line editing feature is not compiled in. define CONFIG_EDITLINE\n");
        return BDMF_ERR_NOT_SUPPORTED;
    }
#endif
    if (!session_module_initialized)
        bdmf_session_module_init();
    name = parm->name;
    if (!name)
        name = "*unnamed*";
    size = sizeof(bdmf_session) + strlen(name) + 1 + parm->extra_size;
    session=bdmf_calloc(size);
    if (!session)
        return BDMF_ERR_NOMEM;
    session->parms = *parm;
    name_clone = (char *)session + sizeof(bdmf_session) + parm->extra_size;
    strcpy(name_clone, name);
    session->parms.name = name_clone;

#ifdef CONFIG_LIBEDIT
    if (!parm->gets && (parm->line_edit_mode == BDMF_LINE_EDIT_ENABLE ||
                        parm->line_edit_mode == BDMF_LINE_EDIT_DEFAULT))
    {
        /* Initialize editline library */
        session->el = el_init(session->parms.name, stdin, stdout, stderr);
        session->history = history_init();
        if (session->el && session->history)
        {
            el_set(session->el, EL_EDITOR, "emacs");
            el_set(session->el, EL_PROMPT, &_bdmf_editline_prompt);
            el_set(session->el, EL_TERMINAL, "xterm");
            el_set(session->el, EL_GETCFN, _bdmf_editline_cfn);
            el_set(session->el, EL_CLIENTDATA, session);
            history(session->history, &session->histevent, H_SETSIZE, 800);
            el_set(session->el, EL_HIST, history, session->history);
        }
        else
        {
            bdmf_trace(BCMOS_TRACE_LEVEL_ERROR, "Can't initialize editline library\n");
            bdmf_free(session);
            return BDMF_ERR_INTERNAL;
        }
    }
#endif
	
#ifdef CONFIG_LINENOISE
    /* Set the completion callback. This will be called every time the
     * user uses the <tab> key. */
    if (!parm->gets && (parm->line_edit_mode == BDMF_LINE_EDIT_ENABLE ||
                        parm->line_edit_mode == BDMF_LINE_EDIT_DEFAULT))
    {
        linenoiseSessionIO io={.fd_in=(long)session, .fd_out=(long)session,
            .read_char=_bdmf_linenoise_read_char,
            .write=_bdmf_linenoise_write
        };
        if (linenoiseSessionOpen(&io, session, &session->ln_session))
        {
            bdmf_trace(BCMOS_TRACE_LEVEL_ERROR, "Can't create linenoise session\n");
            bdmf_free(session);
            return BDMF_ERR_INTERNAL;
        }
        linenoiseSetCompletionCallback(session->ln_session, _bdmf_linenoise_tab);
    }
#endif

    session->magic = BDMF_SESSION_MAGIC;

    bdmf_fastlock_lock(&session_lock);
    p_last_next = &session_list;
    while(*p_last_next)
        p_last_next = &((*p_last_next)->next);
    *p_last_next = session;
    bdmf_fastlock_unlock(&session_lock);

    *p_session = session;

    return 0;
}


/** Close management session.
 * \param[in]   session         Session handle
 */
void bdmf_session_close(bdmf_session_handle session)
{
    BUG_ON(session->magic != BDMF_SESSION_MAGIC);
    bdmf_fastlock_lock(&session_lock);
    if (session==session_list)
        session_list = session->next;
    else
    {
        bdmf_session *prev = session_list;
        while (prev && prev->next != session)
            prev = prev->next;
        if (!prev)
        {
            bdmf_fastlock_unlock(&session_lock);
            bdmf_print("%s: can't find session\n", __FUNCTION__);
            return;
        }
        prev->next = session->next;
    }
    bdmf_fastlock_unlock(&session_lock);

#ifdef CONFIG_LIBEDIT
    if (session->history)
        history_end(session->history);
    if (session->el)
        el_end(session->el);
#endif
#ifdef CONFIG_LINENOISE
    if (session->ln_session)
        linenoiseSessionClose(session->ln_session);
#endif
    session->magic = BDMF_SESSION_MAGIC_DEL;
    bdmf_free(session);

}


/** Default write callback function
 * write to stdout
 */
static int _bdmf_session_write(bdmf_session_handle session, const char *buf, uint32_t size)
{
    return bdmf_print("%.*s", size, buf);
}


/** Write function.
 * Write buffer to the current session.
 * \param[in]   session         Session handle. NULL=use stdout
 * \param[in]   buffer          output buffer
 * \param[in]   size            number of bytes to be written
 * \return
 *  >=0 - number of bytes written\n
 *  <0  - output error
 */
int bdmf_session_write(bdmf_session_handle session, const char *buf, uint32_t size)
{
    int (*write_cb)(bdmf_session_handle session, const char *buf, uint32_t size);
    if (session && session->parms.write)
    {
        BUG_ON(session->magic != BDMF_SESSION_MAGIC);
        write_cb = session->parms.write;
    }
    else
        write_cb = _bdmf_session_write;
    return write_cb(session, buf, size);
}


#ifndef __KERNEL__
/** Read line
 * \param[in]       session         Session handle. NULL=use default
 * \param[in,out]   buf             input buffer
 * \param[in]       size            buf size
 * \return
 *      buf if successful
 *      NULL if EOF or error
 */
char *bdmf_session_gets(bdmf_session_handle session, char *buf, uint32_t size)
{
    return _bdmf_session_gets(session, buf, size);
}
#endif

/** Print function.
 * Prints in the context of current session.
 * \param[in]   session         Session handle. NULL=use stdout
 * \param[in]   format          print format - as in printf
 * \param[in]   ap              parameters list. Undefined after the call
 */
void bdmf_session_vprint(bdmf_session_handle session, const char *format, va_list ap)
{
    if (session && session->parms.write)
    {
        BUG_ON(session->magic != BDMF_SESSION_MAGIC);
        vsnprintf(session->outbuf, sizeof(session->outbuf), format, ap);
        bdmf_session_write(session, session->outbuf, strlen(session->outbuf));
    }
    else
        bdmf_vprint(format, ap);
}


/** Print function.
 * Prints in the context of current session.
 * \param[in]   session         Session handle. NULL=use stdout
 * \param[in]   format          print format - as in printf
 */
void bdmf_session_print(bdmf_session_handle session, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bdmf_session_vprint(session, format, ap);
    va_end(ap);
}

/** Get user_priv provoded in session partameters when it was registered
 * \param[in]       session         Session handle. NULL=use stdin
 * \return usr_priv value
 */
void *bdmf_session_user_priv(bdmf_session_handle session)
{
    if (!session)
        return NULL;
    BUG_ON(session->magic != BDMF_SESSION_MAGIC);
    return session->parms.user_priv;
}


/** Get extra data associated with the session
 * \param[in]       session         Session handle. NULL=default session
 * \return extra_data pointer or NULL if there is no extra data
 */
void *bdmf_session_data(bdmf_session_handle session)
{
    if (!session)
        return NULL;
    BUG_ON(session->magic != BDMF_SESSION_MAGIC);
    if (session->parms.extra_size <= 0)
        return NULL;
    return (char *)session + sizeof(*session);
}


/** Get session namedata
 * \param[in]       session         Session handle. NULL=default session
 * \return session name
 */
const char *bdmf_session_name(bdmf_session_handle session)
{
    if (!session)
        return NULL;
    BUG_ON(session->magic != BDMF_SESSION_MAGIC);
    return session->parms.name;
}


/** Get session access righte
 * \param[in]       session         Session handle. NULL=default debug session
 * \return session access right
 */
bdmf_access_right_t bdmf_session_access_right(bdmf_session_handle session)
{
    if (!session)
        return BDMF_ACCESS_DEBUG;
    BUG_ON(session->magic != BDMF_SESSION_MAGIC);
    return session->parms.access_right;
}

/* HexPrint a single line */
#define BYTES_IN_LINE   16

#define b2a(c)  (isprint(c)?c:'.')

static void _hexprint1( bdmf_session_handle session, uint16_t o, uint8_t *p_data, uint16_t count )
{
    int  i;

    bdmf_session_print(session, "%04x: ", o);
    for( i=0; i<count; i++ )
    {
        bdmf_session_print(session, "%02x", p_data[i]);
        if (!((i+1)%4))
            bdmf_session_print(session, " ");
    }
    for( ; i<BYTES_IN_LINE; i++ )
    {
        if (!((i+1)%4))
            bdmf_session_print(session, "   ");
        else
            bdmf_session_print(session, "  ");
    }
    for( i=0; i<count; i++ )
        bdmf_session_print(session, "%c", b2a(p_data[i]));
    bdmf_session_print(session, "\n");
}

/** Print buffer in hexadecimal format
 * \param[in]   session         Session handle. NULL=use stdout
 * \param[in]   buffer          Buffer address
 * \param[in]   offset          Start offset in the buffer
 * \param[in]   count           Number of bytes to dump
 */
void bdmf_session_hexdump(bdmf_session_handle session, void *buffer, uint32_t offset, uint32_t count)
{
    uint8_t *p_data = buffer;
    uint16_t n;
    while( count )
    {
        n = (count > BYTES_IN_LINE) ? BYTES_IN_LINE : count;
        _hexprint1(session, offset, p_data, n );
        count -= n;
        p_data += n;
        offset += n;
    }
}


/*
 * Exports
 */
EXPORT_SYMBOL(bdmf_session_open);
EXPORT_SYMBOL(bdmf_session_close);
EXPORT_SYMBOL(bdmf_session_write);
EXPORT_SYMBOL(bdmf_session_vprint);
EXPORT_SYMBOL(bdmf_session_print);
EXPORT_SYMBOL(bdmf_session_access_right);
EXPORT_SYMBOL(bdmf_session_data);
EXPORT_SYMBOL(bdmf_session_name);
EXPORT_SYMBOL(bdmf_session_hexdump);
