/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
****************************************************************************/
#ifndef XDRV_MAILBOX_H
#define XDRV_MAILBOX_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

/* ---- Constants and Types ---------------------------------------------- */

/* --------------------------------------------------------------------------
** 'XDRV_MAILBOX_RX' is the base, generic Mailbox structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific MAILBOX drivers.
**
** 'XDRV_MAILBOX_FUNCS' must be the only structure member of 'XDRV_MAILBOX'.
**
** Chip specific MAILBOX drivers should provide a "derived" version of XDRV_MAILBOX:
**
**    typedef struct SLIC_CHIP_DRV
**    {
**       const XDRV_MAILBOX_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } MAILBOX_CHIP_DRV;
**
** Clients, which have access to XDRV_MAILBOX structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_MAILBOX struct as the first parameter. This struct can be cast to the chip
** specific version of the MAILBOX structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_MAILBOX_RX_FUNCS *pDrvFuncs;

} XDRV_MAILBOX_RX;

/* --------------------------------------------------------------------------
** API functions.
*/

typedef int    (*xdrvMailboxGet)( XDRV_MAILBOX_RX *pDrv );
typedef void   (*xdrvMailboxClear)( XDRV_MAILBOX_RX *pDrv );

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_MAILBOX_RX_FUNCS
{
   xdrvMailboxGet    mailboxGet;
   xdrvMailboxClear  mailboxClear;

} XDRV_MAILBOX_RX_FUNCS;


#define xdrvMailboxRxGet( pDrv ) \
         ( (pDrv)->pDrvFuncs->mailboxGet( (pDrv) ) )

#define xdrvMailboxRxClear( pDrv ) \
         ( (pDrv)->pDrvFuncs->mailboxClear( (pDrv) ) )






/* --------------------------------------------------------------------------
** 'XDRV_MAILBOX_TX' is the base, generic Mailbox structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific MAILBOX drivers.
**
** 'XDRV_MAILBOX_FUNCS' must be the only structure member of 'XDRV_MAILBOX'.
**
** Chip specific MAILBOX drivers should provide a "derived" version of XDRV_MAILBOX:
**
**    typedef struct SLIC_CHIP_DRV
**    {
**       const XDRV_MAILBOX_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } MAILBOX_CHIP_DRV;
**
** Clients, which have access to XDRV_MAILBOX structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_MAILBOX struct as the first parameter. This struct can be cast to the chip
** specific version of the MAILBOX structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_MAILBOX_TX_FUNCS *pDrvFuncs;

} XDRV_MAILBOX_TX;


/* --------------------------------------------------------------------------
** API functions.
*/

typedef void   (*xdrvMailboxSet)( XDRV_MAILBOX_TX *pDrv );


typedef struct XDRV_MAILBOX_TX_FUNCS
{
   xdrvMailboxSet    mailboxSet;

} XDRV_MAILBOX_TX_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the mailbox.
*/
#define xdrvMailboxTxSet( pDrv ) \
         ( (pDrv)->pDrvFuncs->mailboxSet( (pDrv) ) )


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_MAILBOX_H  */
