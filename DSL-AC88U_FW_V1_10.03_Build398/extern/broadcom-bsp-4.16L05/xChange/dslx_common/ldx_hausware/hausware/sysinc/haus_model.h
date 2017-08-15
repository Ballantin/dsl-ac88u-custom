/****************************************************************************
*
*
****************************************************************************/

#if !defined( HAUS_MODEL_H )
#define  HAUS_MODEL_H

/* ---- Include Files ---------------------------------------------------- */
#include <xcfg_model.h>


/* ---- Constants and Types ---------------------------------------------- */

#if XCFG_MODEL_GENERIC

/*
** None of the XCFGs were set, try to infer the model.
*/
#include <haus_cpu.h>
#if defined(HAUS_CPU_ZSP) && HAUS_CPU_ZSP
#define HAUS_MODEL_ZSP  1
#endif

#if defined(WIN32) || defined(_WIN32)
#define HAUS_MODEL_SIM  1
#endif

#else

#if XCFG_MODEL_SIM
#define HAUS_MODEL_SIM  1
#endif

#if XCFG_MODEL_ZSP
#define HAUS_MODEL_ZSP  1
#endif

#endif

#endif   /* HAUS_MODEL_H   */
