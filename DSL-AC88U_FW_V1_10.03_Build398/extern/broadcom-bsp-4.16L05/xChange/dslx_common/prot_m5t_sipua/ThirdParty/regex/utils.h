/*******************************************************************************
** Third-Party Code Modification
** Executed by M5T
********************************************************************************
**
** Author : Jean-Denis Langelier
** Date   : 2006-07-14
** Problem Description: The CHAR_BIT, CHAR_MIN and CHAR_MAX macros are not
**      defined in Nucleus.
** Modification Description: Define the CHAR_BIT, CHAR_MIN and CHAR_MAX macros
**      when building for Nucleus.
**
*******************************************************************************/
#if defined(MXD_OS_NUCLEUS)
    /* Number of bits in a `char'.  */
    #ifndef CHAR_BIT
        #define CHAR_BIT 8
    #endif

    /* Minimum and maximum values a `char' can hold.  */
    #ifndef CHAR_MIN
        #define CHAR_MIN (-128)
    #endif
    #ifndef CHAR_MAX
        #define CHAR_MAX 127
    #endif
#endif
/*******************************************************************************
** End of Modification
*******************************************************************************/

/* utility definitions */
#ifdef _POSIX2_RE_DUP_MAX
#define	DUPMAX	_POSIX2_RE_DUP_MAX
#else
#define	DUPMAX	255
#endif
#define	INFINITY	(DUPMAX + 1)
#define	NC		(CHAR_MAX - CHAR_MIN + 1)
typedef unsigned char uch;

/* switch off assertions (if not already off) if no REDEBUG */
#ifndef REDEBUG
#ifndef NDEBUG
#define	NDEBUG	/* no assertions please */
#endif
#endif
#include <assert.h>

/* for old systems with bcopy() but no memmove() */
#ifdef USEBCOPY
#define	memmove(d, s, c)	bcopy(s, d, c)
#endif
