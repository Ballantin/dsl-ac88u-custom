/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: endptApiExtension.h
*
****************************************************************************
*    Description:
*
*      Endpoint API extensions
*
****************************************************************************/

#ifndef ENDPT_API_EXTENSION_H
#define ENDPT_API_EXTENSION_H

#ifdef LINUX /* Linux user space */

/*****************************************************************************
*  FUNCTION:   vrgEndptCapabilities
*
*  PURPOSE:    Retrieve the capabilities for an endpoint
*
*  PARAMETERS: endptState   - state of the endpt object
*              capabilities - a pointer to the capabilities struct to populate
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptCapabilities( ENDPT_STATE *endptState, EPZCAP* capabilities );


/*
*****************************************************************************
** FUNCTION:   vrgEndptSignal
**
** PURPOSE:    Generate a signal on the endpoint (or connection)
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - connection identifier (-1 if not applicable)
**             signal      - signal type code (see EPSIG)
**             value       - signal value
**                            BR signal types - 1
**                            OO signal types - 0 == off, 1 == on
**                            TO signal types - 0 = stop/off, 1= start/on
**                            String types - (char *) cast to NULL-term string value
**              duration   - pulse duration (-1 if not used)
**              period     - pulse period (-1 if not used)
**              repetition - depending on the pulsed signal, this parameter
**                           has different meaning:
**                             - 'em': defines the report pulse count (rpc)
**                             - other pulsed signals: defines the repetition (rep)
**                           (-1 if not used)
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptSignal( ENDPT_STATE *endptState, int cnxId, EPSIG signal, unsigned int value, int duration, int period, int repetition );


/*****************************************************************************
*  FUNCTION:   vrgEndptCreateConnection
*
*  PURPOSE:    Function to create a connection
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier
*              cnxParam    - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptCreateConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );


/*****************************************************************************
*  FUNCTION:   vrgEndptModifyConnection
*
*  PURPOSE:    Function to modify a connection
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier
*              cnxParam    - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptModifyConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );


/*****************************************************************************
*  FUNCTION:   vrgEndptDeleteConnection
*
*  PURPOSE:    Function to delete a connection
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId );


/*
*****************************************************************************
** FUNCTION:   vrgEndptPacket
**
** PURPOSE:    Transmit a endpt packet to the endpoint. This function is asynchronous,
**             it does not wait for the data to be transmitted  before returning.
**             In addition, the buffer release mechanism is asynchronous.
**             The endpoint owns the packet passed to this function until a callback
**             is invoked releasing the packet back to the caller (endptPacketReleaseCallback).
**             The value of 'bufDesc' will be passed to the callback function so that
**             the caller can identify which packet is being released by the endpoint.
**
** PARAMETERS: endptState - endpt object.
**             epPacket   - pointer to the EPPACKET packet.
**             length     - the length of the endpt packet including header
**             bufDesc     - buffer descriptor value that uniquely identifies
**                          the packet. This value is returned in the
**                           callback invoked once the buffer has been completely
**                           transmitted. This value is only meaningful to the
**                           caller and is opaque to the endpoint.
**
** RETURNS:    EPSTATUS
**
** NOTE:       If this function does not return EPSTATUS_SUCCESS, it is the caller's
**             responsibility to free the RTP packet.
**
*****************************************************************************
*/
EPSTATUS vrgEndptPacket( ENDPT_STATE *endptState, int cnxId, EPPACKET* epPacket, int length, unsigned int bufDesc);


/*
*****************************************************************************
** FUNCTION:   vrgEndptConsoleCmd
**
** PURPOSE:    Display endpoint info to console
**
** PARAMETERS: endptState  - state of the endpt object
**             cnxId       - the connection index
**             cmd         - endpoint console command type
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptConsoleCmd( ENDPT_STATE *endptState, EPCONSOLECMD cmd, EPCMD_PARMS *pConsoleCmdParams );


/* In order to provide Linux user endpoint APIs, we need to redefine 
** the macros in endpt.h in order to use the Linux user versions 
** of the vrgEndptxxx functions (which are just wrappers for ioctl calls
** to the kernel), not the "real" vrgEndptxxx functions defined in vrgEndpt.c */

#ifdef endptCapabilities
#  undef endptCapabilities
#  define endptCapabilities vrgEndptCapabilities
#endif 

#ifdef endptSignal
#  undef endptSignal
#  define endptSignal vrgEndptSignal
#endif 

#ifdef endptCreateConnection
#  undef endptCreateConnection
#  define endptCreateConnection vrgEndptCreateConnection
#endif 

#ifdef endptModifyConnection
#  undef endptModifyConnection
#  define endptModifyConnection vrgEndptModifyConnection
#endif 

#ifdef endptDeleteConnection
#  undef endptDeleteConnection
#  define endptDeleteConnection vrgEndptDeleteConnection
#endif 

#ifdef endptPacket
#  undef endptPacket
#  define endptPacket vrgEndptPacket
#endif 

#ifdef endptConsoleCmd
#  undef endptConsoleCmd
#  define endptConsoleCmd vrgEndptConsoleCmd
#endif 


int isEndptInitialized( void );


#endif /* Linux user space */

#endif  /* ENDPT_API_EXTENSION_H  */
