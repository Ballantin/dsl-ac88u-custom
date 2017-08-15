/*****************************************************************************
*
*  Copyright (c) 2004 Broadcom, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    ethl.h
*
*  @brief   Lower ethernet device driver header file
*
*  This file contains the lower device driver header information for the
*  ethernet module.
*/
/****************************************************************************/

#ifndef ETHL_H
#define ETHL_H


/* ---- Include Files ----------------------------------------------------- */


/* ---- Public Constants and Types ---------------------------------------- */


typedef int ETHL_PRI_INFO[8];
typedef int ETHL_PORT_MASK;

#define ETHL_PORT_INT      10


typedef struct
{
   int (*ethlInitp)( void );
   int (*ethlChipInfop)( int id, int rev );
   int (*ethlMacSetp)( char *macp );
   int (*ethlMacGetp)( char *macp );
   int (*ethlRxFrameFormatp)( char **bufpp, int *lenp, int *portp );
   int (*ethlRxOffsetp)( void );
   int (*ethlTxFrameFormatp)( char **bufpp, int *lenp, int port );
   int (*ethlTxFrameFormatPortp)( char *bufp, int len, int port );
   int (*ethlTxOffsetp)( void );
   int (*ethlExtPhyEnablep)( int enable );
   int (*ethlExtPhyIsEnabledp)( void );
   int (*ethlIntPortEnablep)( int enable );
   int (*ethlSoftResetp)( void );
   int (*ethlPhyMonitorp)( void );
   
   int (*ethlDosEnableBcastp)( int enable );
   int (*ethlDosEnableMcastp)( int enable );
   int (*ethlDosEnableUcastp)( int enable );
   unsigned long (*ethlDosMibBcastp)( void );
   unsigned long (*ethlDosMibMcastp)( void );
   unsigned long (*ethlDosMibUcastp)( void );

   int (*ethlHiQconfigGetp)( int port, int *depthp, int *forcep,
                             int *weightp, ETHL_PRI_INFO *mapp );
   int (*ethlHiQconfigSetp)( int port, int depth, int force,
                             int weight, ETHL_PRI_INFO *mapp );
   int (*ethlSecurePortGetp)( ETHL_PORT_MASK *srcMaskp, ETHL_PORT_MASK *dstMaskp );
   int (*ethlSecurePortSetp)( ETHL_PORT_MASK srcMask, ETHL_PORT_MASK dstMask );
   int (*ethlMirrorPortGetp)( ETHL_PORT_MASK *srcMaskp, ETHL_PORT_MASK *dstMaskp );
   int (*ethlMirrorPortSetp)( ETHL_PORT_MASK srcMask, ETHL_PORT_MASK dstMask );

   int (*ethlMultiportGetp)( int *enablep, char *macp, ETHL_PORT_MASK *dstMaskp );
   int (*ethlMultiportSetp)( int enable, char *macp, ETHL_PORT_MASK dstMask );
   int (*ethlBpduGetp)( char *macp );
   int (*ethlBpduSetp)( char *macp );
   int (*ethlIntFwdGetp)( int *ucastp, int *bcastp, int *mcastp, int *bpdup );
   int (*ethlIntFwdSetp)( int ucast, int bcast, int mcast, int bpdu );
   int (*ethlArlConfigGetp)( int *agep );
   int (*ethlArlConfigSetp)( int age );
   int (*ethlSaFilterConfigGetp)( int port, int *enablep, char *macp,
                                  int *dropOnMatchp );
   int (*ethlSaFilterConfigSetp)( int port, int enable, char *macp,
                                  int dropOnMatch );
   int (*ethl802_1xConfigGetp)( int port, int *enablep, int *unauthorizedp );
   int (*ethl802_1xConfigSetp)( int port, int enable, int unauthorized );
   
   int (*ethl802_1pqConfigGetp)( int port, int *enablep, int *priorityp,
                                 int *vlanIdp, int *vlanId2p, int *vlanId3p );
   int (*ethl802_1pqConfigSetp)( int port, int enable, int priority,
                                 int vlanId, int vlanId2, int vlanId3 );
   int (*ethl802_1pConfigGetp)( int port, int *regenEnablep, ETHL_PRI_INFO *mapp );
   int (*ethl802_1pConfigSetp)( int port, int regenEnable, ETHL_PRI_INFO *mapp );
   int (*ethl802_1qConfigGetp)( int port, int *gmrpp, int *gvrpp, int *filterp,
                                int *admitVlanTaggedp, int *vlanIdFilterp,
                                int *replaceTagp, int *addTagp,
                                int *allowUntaggedp, int *removeTagp,
                                int *allowAnyVlanIdp );
   int (*ethl802_1qConfigSetp)( int port, int gmrp, int gvrp, int filter,
                                int admitVlanTagged, int vlanIdFilter,
                                int replaceTag, int addTag,
                                int allowUntagged, int removeTag,
                                int allowAnyVlanId );   

   int (*ethlArlTableGetp)( char *macp, int *portp, int *learnedp,
                            int *staticp, int *indexp );
   int (*ethlArlTableClrp)( void );

   int (*ethlMiiGetp)( int port, int addr );
   int (*ethlMiiSetp)( int port, int addr, int data );
   int (*ethlMiiMaskSetp)( int port, int addr, int mask );
   int (*ethlMiiMaskClrp)( int port, int addr, int mask );

   int (*ethlMibClrp)( int port );
   unsigned int (*ethlMibTxOctetsp)( int port );
   unsigned int (*ethlMibTxOctets2p)( int port );
   unsigned int (*ethlMibTxDropPktsp)( int port );
   unsigned int (*ethlMibTxBroadcastPktsp)( int port );
   unsigned int (*ethlMibTxMulticastPktsp)( int port );
   unsigned int (*ethlMibTxUnicastPktsp)( int port );
   unsigned int (*ethlMibTxCollisionsp)( int port );
   unsigned int (*ethlMibTxSingleCollisionp)( int port );
   unsigned int (*ethlMibTxMultipleCollisionp)( int port );
   unsigned int (*ethlMibTxDeferredTransmitp)( int port );
   unsigned int (*ethlMibTxLateCollisionp)( int port );
   unsigned int (*ethlMibTxExcessiveCollisionp)( int port );
   unsigned int (*ethlMibTxFrameInDiscp)( int port );
   unsigned int (*ethlMibTxPausePktsp)( int port );
   unsigned int (*ethlMibRxOctetsp)( int port );
   unsigned int (*ethlMibRxOctets2p)( int port );
   unsigned int (*ethlMibRxUndersizePktsp)( int port );
   unsigned int (*ethlMibRxPausePktsp)( int port );
   unsigned int (*ethlMibPkts64octetsp)( int port );
   unsigned int (*ethlMibPkts65to127octetsp)( int port );
   unsigned int (*ethlMibPkts128to255octetsp)( int port );
   unsigned int (*ethlMibPkts256to511octetsp)( int port );
   unsigned int (*ethlMibPkts512to1023octetsp)( int port );
   unsigned int (*ethlMibPkts1024to1522octetsp)( int port );
   unsigned int (*ethlMibRxOversizePktsp)( int port );
   unsigned int (*ethlMibRxJabbersp)( int port );
   unsigned int (*ethlMibRxAlignmentErrorsp)( int port );
   unsigned int (*ethlMibRxFcsErrorsp)( int port );
   unsigned int (*ethlMibRxGoodOctetsp)( int port );
   unsigned int (*ethlMibRxGoodOctets2p)( int port );
   unsigned int (*ethlMibRxDropPktsp)( int port );
   unsigned int (*ethlMibRxUnicastPktsp)( int port );
   unsigned int (*ethlMibRxMulticastPktsp)( int port );
   unsigned int (*ethlMibRxBroadcastPktsp)( int port );
   unsigned int (*ethlMibRxSaChangesp)( int port );
   unsigned int (*ethlMibRxFragmentsp)( int port );
   unsigned int (*ethlMibRxExcessSizeDiscp)( int port );
   unsigned int (*ethlMibRxSymbolErrorp)( int port );
} ETHL_FUNCS;

#define ethlInit( drvp )                           (drvp)->funcp->ethlInitp()
#define ethlChipInfo( drvp, id, rev )              (drvp)->funcp->ethlChipInfop( id, rev )
#define ethlMacSet( drvp, macp )                   (drvp)->funcp->ethlMacSetp( macp )
#define ethlMacGet( drvp, macp )                   (drvp)->funcp->ethlMacGetp( macp )
#define ethlRxFrameFormat( drvp, bufpp, lenp, portp ) \
                                                   (drvp)->funcp->ethlRxFrameFormatp( bufpp, lenp, portp )
#define ethlRxOffset( drvp )                       (drvp)->funcp->ethlRxOffsetp()
#define ethlTxFrameFormat( drvp, bufpp, lenp, port ) \
                                                   (drvp)->funcp->ethlTxFrameFormatp( bufpp, lenp, port )
#define ethlTxFrameFormatPort( drvp, bufp, len, port ) \
                                                   (drvp)->funcp->ethlTxFrameFormatPortp( bufp, len, port )
#define ethlTxOffset( drvp )                       (drvp)->funcp->ethlTxOffsetp()
#define ethlExtPhyEnable( drvp, enable )           (drvp)->funcp->ethlExtPhyEnablep( enable )
#define ethlExtPhyIsEnabled( drvp )                (drvp)->funcp->ethlExtPhyIsEnabledp()
#define ethlIntPortEnable( drvp, enable )          (drvp)->funcp->ethlIntPortEnablep( enable )
#define ethlSoftReset( drvp )                      (drvp)->funcp->ethlSoftResetp()
#define ethlPhyMonitor( drvp )                     (drvp)->funcp->ethlPhyMonitorp()
#define ethlPromiscuousEnable( drvp, enable )      (drvp)->funcp->ethlPromiscuousEnablep( enable )
#define ethlPromiscuousIsEnabled( drvp )           (drvp)->funcp->ethlPromiscuousIsEnabledp()

#define ethlDosEnableBcast( drvp, enable )         (drvp)->funcp->ethlDosEnableBcastp( enable )
#define ethlDosEnableMcast( drvp, enable )         (drvp)->funcp->ethlDosEnableMcastp( enable )
#define ethlDosEnableUcast( drvp, enable )         (drvp)->funcp->ethlDosEnableUcastp( enable )
#define ethlDosMibBcast( drvp )                    (drvp)->funcp->ethlDosMibBcastp()
#define ethlDosMibMcast( drvp )                    (drvp)->funcp->ethlDosMibMcastp()
#define ethlDosMibUcast( drvp )                    (drvp)->funcp->ethlDosMibUcastp()

#define ethlHiQconfigGet( drvp, portp, depthp, forcep, weightp, mapp ) \
                                                   (drvp)->funcp->ethlHiQconfigGetp( port, depthp, forcep, weightp, mapp )
#define ethlHiQconfigSet( drvp, port, depth, force, weight, mapp ) \
                                                   (drvp)->funcp->ethlHiQconfigSetp( port, depth, force, weight, mapp )
#define ethlSecurePortGet( drvp, srcMaskp, dstMaskp ) \
                                                   (drvp)->funcp->ethlSecurePortGetp( srcMaskp, dstMaskp )
#define ethlSecurePortSet( drvp, srcMask, dstMask ) \
                                                   (drvp)->funcp->ethlSecurePortSetp( srcMask, dstMask )
#define ethlMirrorPortGet( drvp, srcMaskp, dstMaskp ) \
                                                   (drvp)->funcp->ethlMirrorPortGetp( srcMaskp, dstMaskp )
#define ethlMirrorPortSet( drvp, srcMask, dstMask ) \
                                                   (drvp)->funcp->ethlMirrorPortSetp( srcMask, dstMask )
#define ethlMultiportGet( drvp, enablep, macp, dstMaskp ) \
                                                   (drvp)->funcp->ethlMultiportGetp( enablep, macp, dstMaskp )
#define ethlMultiportSet( drvp, enable, macp, dstMask ) \
                                                   (drvp)->funcp->ethlMultiportSetp( enable, macp, dstMask )
#define ethlBpduGet( drvp, macp )                  (drvp)->funcp->ethlBpduGetp( macp )
#define ethlBpduSet( drvp, macp )                  (drvp)->funcp->ethlBpduSetp( macp )
#define ethlIntFwdGet( drvp, ucastp, bcastp, mcastp, bpdup ) \
                                                   (drvp)->funcp->ethlIntFwdGetp( ucastp, bcastp, mcastp, bpdup )
#define ethlIntFwdSet( drvp, ucast, bcast, mcast, bpdu ) \
                                                   (drvp)->funcp->ethlIntFwdSetp( ucast, bcast, mcast, bpdu )
#define ethlArlConfigGet( drvp, agep )             (drvp)->funcp->ethlArlConfigGetp( agep )
#define ethlArlConfigSet( drvp, age )              (drvp)->funcp->ethlArlConfigSetp( age )
#define ethlSaFilterConfigGet( drvp, port, enablep, macp, dropOnMatchp ) \
                                                   (drvp)->funcp->ethlSaFilterConfigGetp( port, enablep, macp, dropOnMatchp )
#define ethlSaFilterConfigSet( drvp, port, enable, macp, dropOnMatch ) \
                                                   (drvp)->funcp->ethlSaFilterConfigSetp( port, enable, macp, dropOnMatch )
#define ethl802_1xConfigGet( drvp, port, enablep, unauthorizedp ) \
                                                   (drvp)->funcp->ethl802_1xConfigGetp( port, enablep, unauthorizedp )
#define ethl802_1xConfigSet( drvp, port, enable, unauthorized ) \
                                                   (drvp)->funcp->ethl802_1xConfigSetp( port, enable, unauthorized )

#define ethl802_1pqConfigGet( drvp, port, enablep, priorityp, vlanIdp, vlanId2p, vlanId3p ) \
                                                   (drvp)->funcp->ethl802_1pqConfigGetp( port, enablep, priorityp, \
                                                                                         vlanIdp, vlanId2p, vlanId3p )
#define ethl802_1pqConfigSet( drvp, port, enable, priority, vlanId, vlanId2, vlanId3 ) \
                                                   (drvp)->funcp->ethl802_1pqConfigSetp( port, enable, priority, \
                                                                                         vlanId, vlanId2, vlanId3 )
#define ethl802_1pConfigGet( drvp, port, regenEnablep, mapp ) \
                                                   (drvp)->funcp->ethl802_1pConfigGetp( port, regenEnablep, mapp )
#define ethl802_1pConfigSet( drvp, port, regenEnable, mapp ) \
                                                   (drvp)->funcp->ethl802_1pConfigSetp( port, regenEnable, mapp )
#define ethl802_1qConfigGet( drvp, port, gmrpp, gvrpp, filterp, admitVlanTaggedp, \
                             vlanIdFilterp, replaceTagp, addTagp, allowUntaggedp, \
                             removeTagp, allowAnyVlanIdp ) \
                                                   (drvp)->funcp->ethl802_1qConfigGetp( port, gmrpp, gvrpp, filterp, \
                                                                                        admitVlanTaggedp, vlanIdFilterp, \
                                                                                        replaceTagp, addTagp, \
                                                                                        allowUntaggedp, removeTagp, \
                                                                                        allowAnyVlanIdp )
#define ethl802_1qConfigSet( drvp, port, gmrp, gvrp, filter, admitVlanTagged, \
                             vlanIdFilter, replaceTag, addTag, allowUntagged, \
                             removeTag, allowAnyVlanId ) \
                                                   (drvp)->funcp->ethl802_1qConfigSetp( port, gmrp, gvrp, filter, \
                                                                                        admitVlanTagged, vlanIdFilter, \
                                                                                        replaceTag, addTag, \
                                                                                        allowUntagged, removeTag, \
                                                                                        allowAnyVlanId )

#define ethlArlTableGet( drvp, macp, portp, learnedp, staticp, indexp ) \
                                                   (drvp)->funcp->ethlArlTableGetp( macp, portp, learnedp, \
                                                                                    staticp, indexp )
#define ethlArlTableClr( drvp )                    (drvp)->funcp->ethlArlTableClrp()

#define ethlMiiGet( drvp, port, addr )             (drvp)->funcp->ethlMiiGetp( port, addr )
#define ethlMiiSet( drvp, port, addr, data )       (drvp)->funcp->ethlMiiSetp( port, addr, data )
#define ethlMiiMaskSet( drvp, port, addr, mask )   (drvp)->funcp->ethlMiiMaskSetp( port, addr, mask )
#define ethlMiiMaskClr( drvp, port, addr, mask )   (drvp)->funcp->ethlMiiMaskClrp( port, addr, mask )

#define ethlMibClr( drvp, port )                   (drvp)->funcp->ethlMibClrp( port )
#define ethlMibTxOctets( drvp, port )              (drvp)->funcp->ethlMibTxOctetsp( port )
#define ethlMibTxOctets2( drvp, port )             (drvp)->funcp->ethlMibTxOctets2p( port )
#define ethlMibTxDropPkts( drvp, port )            (drvp)->funcp->ethlMibTxDropPktsp( port )
#define ethlMibTxBroadcastPkts( drvp, port )       (drvp)->funcp->ethlMibTxBroadcastPktsp( port )
#define ethlMibTxMulticastPkts( drvp, port )       (drvp)->funcp->ethlMibTxMulticastPktsp( port )
#define ethlMibTxUnicastPkts( drvp, port )         (drvp)->funcp->ethlMibTxUnicastPktsp( port )
#define ethlMibTxCollisions( drvp, port )          (drvp)->funcp->ethlMibTxCollisionsp( port )
#define ethlMibTxSingleCollision( drvp, port )     (drvp)->funcp->ethlMibTxSingleCollisionp( port )
#define ethlMibTxMultipleCollision( drvp, port )   (drvp)->funcp->ethlMibTxMultipleCollisionp( port )
#define ethlMibTxDeferredTransmit( drvp, port )    (drvp)->funcp->ethlMibTxDeferredTransmitp( port )
#define ethlMibTxLateCollision( drvp, port )       (drvp)->funcp->ethlMibTxLateCollisionp( port )
#define ethlMibTxExcessiveCollision( drvp, port )  (drvp)->funcp->ethlMibTxExcessiveCollisionp( port )
#define ethlMibTxFrameInDisc( drvp, port )         (drvp)->funcp->ethlMibTxFrameInDiscp( port )
#define ethlMibTxPausePkts( drvp, port )           (drvp)->funcp->ethlMibTxPausePktsp( port )
#define ethlMibRxOctets( drvp, port )              (drvp)->funcp->ethlMibRxOctetsp( port )
#define ethlMibRxOctets2( drvp, port )             (drvp)->funcp->ethlMibRxOctets2p( port )
#define ethlMibRxUndersizePkts( drvp, port )       (drvp)->funcp->ethlMibRxUndersizePktsp( port )
#define ethlMibRxPausePkts( drvp, port )           (drvp)->funcp->ethlMibRxPausePktsp( port )
#define ethlMibPkts64octets( drvp, port )          (drvp)->funcp->ethlMibPkts64octetsp( port )
#define ethlMibPkts65to127octets( drvp, port )     (drvp)->funcp->ethlMibPkts65to127octetsp( port )
#define ethlMibPkts128to255octets( drvp, port )    (drvp)->funcp->ethlMibPkts128to255octetsp( port )
#define ethlMibPkts256to511octets( drvp, port )    (drvp)->funcp->ethlMibPkts256to511octetsp( port )
#define ethlMibPkts512to1023octets( drvp, port )   (drvp)->funcp->ethlMibPkts512to1023octetsp( port )
#define ethlMibPkts1024to1522octets( drvp, port )  (drvp)->funcp->ethlMibPkts1024to1522octetsp( port )
#define ethlMibRxOversizePkts( drvp, port )        (drvp)->funcp->ethlMibRxOversizePktsp( port )
#define ethlMibRxJabbers( drvp, port )             (drvp)->funcp->ethlMibRxJabbersp( port )
#define ethlMibRxAlignmentErrors( drvp, port )     (drvp)->funcp->ethlMibRxAlignmentErrorsp( port )
#define ethlMibRxFcsErrors( drvp, port )           (drvp)->funcp->ethlMibRxFcsErrorsp( port )
#define ethlMibRxGoodOctets( drvp, port )          (drvp)->funcp->ethlMibRxGoodOctetsp( port )
#define ethlMibRxGoodOctets2( drvp, port )         (drvp)->funcp->ethlMibRxGoodOctets2p( port )
#define ethlMibRxDropPkts( drvp, port )            (drvp)->funcp->ethlMibRxDropPktsp( port )
#define ethlMibRxUnicastPkts( drvp, port )         (drvp)->funcp->ethlMibRxUnicastPktsp( port )
#define ethlMibRxMulticastPkts( drvp, port )       (drvp)->funcp->ethlMibRxMulticastPktsp( port )
#define ethlMibRxBroadcastPkts( drvp, port )       (drvp)->funcp->ethlMibRxBroadcastPktsp( port )
#define ethlMibRxSaChanges( drvp, port )           (drvp)->funcp->ethlMibRxSaChangesp( port )
#define ethlMibRxFragments( drvp, port )           (drvp)->funcp->ethlMibRxFragmentsp( port )
#define ethlMibRxExcessSizeDisc( drvp, port )      (drvp)->funcp->ethlMibRxExcessSizeDiscp( port )
#define ethlMibRxSymbolError( drvp, port )         (drvp)->funcp->ethlMibRxSymbolErrorp( port )


typedef struct
{
   /* This must be the first structure member. */
   const ETHL_FUNCS *funcp;

} ETHL_DRV;


#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Variables -------------------------------------------------- */


/* ---- Public Function Prototypes ---------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif
