//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_SOCKETDEFINES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SOCKETDEFINES_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_IPV6_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

#if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #include <sys/socket.h>
    #include <unistd.h>
#elif defined ( MXD_OS_VXWORKS )
    #include <unistd.h>
//BRCM_CUSTOM - [add] Add check for VxWorks version
#if (MXD_VXWORKS_VERSION >= 0x060300)
    #include <ioLib.h>
#endif
//BRCM_CUSTOM - [end] Add check for VxWorks version
#elif defined ( MXD_OS_WINDOWS )
    #include <Qossp.h>
    #if defined(MXD_IPV6_ENABLE_SUPPORT)
        #include <Iphlpapi.h>
    #endif
#elif defined ( MXD_OS_WINDOWS_CE )
    #if defined(MXD_IPV6_ENABLE_SUPPORT)
        #include <Iphlpapi.h>
    #endif
#elif defined ( MXD_OS_NUCLEUS )
    #include "net/inc/nu_net.h"
#elif defined ( MXD_OS_SYMBIAN )
    #include <sys/socket.h>
    #include <es_sock.h>
//BRCM_CUSTOM - [add] Add eCos support.
#elif defined ( MXD_OS_BOS_ECOS )
    #include <bosSocket.h>
//BRCM_CUSTOM - [end] Add eCos support.
#else
    #error "Targeted operating system undefined. Please define MXD_OS_LINUX," \
           "MXD_OS_VXWORKS or MXD_OS_WINDOWS or MXD_OS_WINDOWS_CE"
#endif


MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Operating system specific socket layer error code.
#if defined(MXD_OS_LINUX) || defined(MXD_OS_ANDROID) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS)
    #define nINVALID_SOCKET -1
    #define nSOCKET_ERROR   -1
#elif defined(MXD_OS_VXWORKS)
    #define nINVALID_SOCKET -1
    #define nSOCKET_ERROR   ERROR
#elif defined(MXD_OS_WINDOWS) || \
      defined(MXD_OS_WINDOWS_CE)
    #define nINVALID_SOCKET INVALID_SOCKET
    #define nSOCKET_ERROR   SOCKET_ERROR
#elif defined(MXD_OS_SYMBIAN)
    #define nSOCKET_ERROR   -1
    #define nINVALID_SOCKET NULL
//BRCM_CUSTOM - [add] Add eCos support.
#elif defined( MXD_OS_BOS_ECOS )
    #define nINVALID_SOCKET BOS_SOCKET_INVALID
    #define nSOCKET_ERROR   BOS_SOCKET_ERROR
//BRCM_CUSTOM - [end] Add eCos support.
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// Defines a new type to abstract socket handle under all implementations.
#if defined(MXD_OS_LINUX)   || \
    defined(MXD_OS_ANDROID) || \
    defined(MXD_OS_SOLARIS) || \
    defined(MXD_OS_DARWIN)  || \
    defined(MXD_OS_VXWORKS) || \
    defined(MXD_OS_NUCLEUS)
    typedef int mxt_hSocket;

#elif defined(MXD_OS_WINDOWS) || \
      defined(MXD_OS_WINDOWS_CE)
    typedef SOCKET mxt_hSocket;

#elif defined(MXD_OS_SYMBIAN)
    typedef RSocket* mxt_hSocket;

//BRCM_CUSTOM - [add] Add eCos support.
#elif defined( MXD_OS_BOS_ECOS )
    typedef BOS_SOCKET *mxt_hSocket;
//BRCM_CUSTOM - [end] Add eCos support.

#endif

// Defines a new type to abstract socket address lenght under all implementations.
#if defined(MXD_OS_LINUX) || defined(MXD_OS_ANDROID) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN)
    typedef socklen_t mxt_addrLen;

//BRCM_CUSTOM - [mod] Add eCos support.
#elif defined(MXD_OS_VXWORKS) || \
      defined(MXD_OS_WINDOWS) || \
      defined(MXD_OS_BOS_ECOS)|| \
      defined(MXD_OS_WINDOWS_CE)
//BRCM_CUSTOM - [end] Add eCos support.
    typedef int mxt_addrLen;

#elif defined(MXD_OS_NUCLEUS)
    typedef int16_t mxt_addrLen;

#endif

// Defines a new shutdown type to abstract all implementations.
enum EShutdownType
{
#if defined(MXD_OS_LINUX)   || \
    defined(MXD_OS_ANDROID) || \
    defined(MXD_OS_SOLARIS) || \
    defined(MXD_OS_DARWIN)  || \
    defined(MXD_OS_NUCLEUS)
    eDISALLOW_RECEIVE   = SHUT_RD,
    eDISALLOW_TRANSMIT  = SHUT_WR,
    eDISALLOW_BOTH      = SHUT_RDWR

#elif defined(MXD_OS_VXWORKS)
    eDISALLOW_RECEIVE   = 0,
    eDISALLOW_TRANSMIT  = 1,
    eDISALLOW_BOTH      = 2

#elif defined(MXD_OS_WINDOWS)|| \
      defined(MXD_OS_WINDOWS_CE)
    eDISALLOW_RECEIVE   = SD_RECEIVE,
    eDISALLOW_TRANSMIT  = SD_SEND,
    eDISALLOW_BOTH      = SD_BOTH

#elif defined(MXD_OS_SYMBIAN)
    eDISALLOW_RECEIVE   = RSocket::EStopInput,
    eDISALLOW_TRANSMIT  = RSocket::EStopOutput,
    eDISALLOW_BOTH      = RSocket::EImmediated
//BRCM_CUSTOM - [add] Add eCos support.
#elif defined( MXD_OS_BOS_ECOS )
    eDISALLOW_RECEIVE   = BOS_SOCKET_SHUTDOWN_RECEIVE,
    eDISALLOW_TRANSMIT  = BOS_SOCKET_SHUTDOWN_SEND,
    eDISALLOW_BOTH      = BOS_SOCKET_SHUTDOWN_BOTH
//BRCM_CUSTOM - [end] Add eCos support.
#endif
};
//M5T_INTERNAL_USE_END

//BRCM_CUSTOM - [begin] Add eCos support.
//==============================================================================
//====  MACRROS TO HIDE OS CONFIGURATION =======================================
//==============================================================================
#if defined( MXD_OS_BOS_ECOS )
   #define MX_GETSOCKETID(mx_hSocket) bosSocketGetNativeId(mx_hSocket)
   #define MX_FD_ZERO(socketSet) bosSocketSetZero(socketSet)
   #define MX_FD_SET(mx_hSocket, socketSet) bosSocketSetAddSocket(mx_hSocket, socketSet)
   #define MX_FD_CLR(mx_hSocket, socketSet) bosSocketSetRemoveSocket(mx_hSocket, socketSet)
   #define MX_FD_ISSET(mx_hSocket, socketSet) bosSocketSetIsSocketSet(mx_hSocket, socketSet)
#else
   #define MX_GETSOCKETID(mx_hSocket) mx_hSocket
   #define MX_FD_ZERO(set) FD_ZERO(set)
   #define MX_FD_SET(mx_hSocket, socketSet) FD_SET(mx_hSocket, socketSet)
   #define MX_FD_CLR(mx_hSocket, socketSet) FD_CLR(mx_hSocket, socketSet)
   #define MX_FD_ISSET(mx_hSocket, socketSet) FD_ISSET(mx_hSocket, socketSet)
#endif
//BRCM_CUSTOM - [end] Add eCos support.

//==============================================================================
//====  NEW INLINED METHOD =====================================================
//==============================================================================

#if defined(MXD_OS_NUCLEUS)
//M5T_INTERNAL_USE_BEGIN
#define AF_INET         NU_FAMILY_IP
#define PF_INET         AF_INET
#define SOCK_DGRAM      NU_TYPE_DGRAM
#define SOCK_STREAM     NU_TYPE_STREAM
#define fd_set          nu_fd_set
#define FD_SET          NU_FD_Set
#define FD_ZERO         NU_FD_Init
#define FD_CLR          NU_FD_Reset
#define FD_ISSET        NU_FD_Check
#define INADDR_ANY      IP_ADDR_ANY
#define INADDR_BROADCAST IP_ADDR_BROADCAST
#define INADDR_LOOPBACK LOOPBACK_ADDR + 1
#define INADDR_NONE     IP_ADDR_BROADCAST //Must find the proper value for this.
#define select          NU_Select
#define listen          NU_Listen
#define SO_ERROR        0xFFFF

typedef struct timeval
{
    unsigned int tv_sec;
    unsigned int tv_usec;
};

typedef struct linger
{
    int l_onoff;
    unsigned int l_linger;
};

typedef struct sockaddr
{
    union
    {
        struct
        {
            int16_t sa_family;
            uint8_t aPadding[sizeof(addr_struct) - sizeof(int16_t)];
        };
        struct addr_struct stNucleus_addr_struct;
    };
};

typedef struct in_addr
{
    uint32_t s_addr;
};

typedef struct sockaddr_in
{
    union
    {
        struct
        {
            uint16_t        sin_family;
            uint16_t        sin_port;
            struct in_addr  sin_addr;
            uint8_t         aPadding[sizeof(addr_struct) -
                                     sizeof(uint16_t) -
                                     sizeof(uint16_t) -
                                     sizeof(in_addr)];
        };
        struct addr_struct stNucleus_addr_struct;
    };
};

inline
int SetErrnoOnError(IN int nNucleusReturnValue)
{
    // In case of an error set errno and return standard error.
    if (nNucleusReturnValue < 0 )
    {
        MX_ERRNO_SET(nNucleusReturnValue);
        nNucleusReturnValue = nSOCKET_ERROR;
    }
    return nNucleusReturnValue;
}

inline
int socket(int domain, int type, int protocol)
{
    return SetErrnoOnError(NU_Socket(static_cast<int16_t>(domain),
                                     static_cast<int16_t>(type),
                                     static_cast<int16_t>(protocol)));
}

inline
int accept(int s, struct sockaddr *addr, mxt_addrLen *addrlen)
{
    return SetErrnoOnError(NU_Accept(s, reinterpret_cast<addr_struct*>(addr), addrlen));
}

inline
int bind(int sockfd, struct sockaddr *my_addr, mxt_addrLen addrlen)
{
    return SetErrnoOnError(NU_Bind(sockfd, reinterpret_cast<addr_struct*>(my_addr), addrlen));
}

inline
int connect(int sockfd, const struct sockaddr *serv_addr, mxt_addrLen addrlen)
{
    return SetErrnoOnError(NU_Connect(sockfd, reinterpret_cast<addr_struct*>(const_cast<sockaddr*>(serv_addr)), addrlen));
}

inline
int recv(int s, void *buf, int len, int flags)
{
    return SetErrnoOnError(NU_Recv(s, reinterpret_cast<char*>(buf), static_cast<uint16_t>(len), flags));
}

inline
int recvfrom(int s, void *buf, int len, int flags,
             struct sockaddr *from, mxt_addrLen *fromlen)
{
    return SetErrnoOnError(NU_Recv_From(s, reinterpret_cast<char*>(buf), static_cast<uint16_t>(len), flags,
                           reinterpret_cast<addr_struct*>(from), fromlen));
}

inline
int send(int s, const void *msg, int len, int flags)
{
    return SetErrnoOnError(NU_Send(s, reinterpret_cast<char*>(const_cast<void*>(msg)), static_cast<uint16_t>(len), flags));
}

inline
int sendto(int s, const void *msg, int len, int flags,
           const struct sockaddr *to, mxt_addrLen tolen)
{
    return SetErrnoOnError(NU_Send_To(s, reinterpret_cast<char*>(const_cast<void*>(msg)), static_cast<uint16_t>(len), flags,
                                   reinterpret_cast<addr_struct*>(const_cast<sockaddr*>(to)), tolen));
}

inline
int getsockname(int s, struct sockaddr *name, mxt_addrLen *namelen)
{
    sockaddr_struct stAddr;
    *namelen = sizeof(stAddr);
    int nRes =  SetErrnoOnError(NU_Get_Sock_Name(s, &stAddr, namelen));

    if (nRes != nSOCKET_ERROR)
    {
        MX_ASSERT(MAX_ADDRESS_SIZE == sizeof(struct in_addr));
        reinterpret_cast<sockaddr_in*>(name)->sin_family = AF_INET;
        reinterpret_cast<sockaddr_in*>(name)->sin_port = stAddr.port_num;
        reinterpret_cast<sockaddr_in*>(name)->sin_addr = *reinterpret_cast<in_addr*>(&stAddr.ip_num);
        *namelen = 8; // sizeof(uint16_t) + sizeof(uint16_t) + sizeof(struct in_addr)
    }
    return nRes;
}

inline
int getpeername(int s, struct sockaddr *name, mxt_addrLen *namelen)
{
    sockaddr_struct stAddr;
    *namelen = sizeof(stAddr);
    int nRes = SetErrnoOnError(NU_Get_Peer_Name(s, &stAddr, namelen));

    if (nRes != nSOCKET_ERROR)
    {
        MX_ASSERT(MAX_ADDRESS_SIZE == sizeof(struct in_addr));
        reinterpret_cast<sockaddr_in*>(name)->sin_family = AF_INET;
        reinterpret_cast<sockaddr_in*>(name)->sin_port = stAddr.port_num;
        reinterpret_cast<sockaddr_in*>(name)->sin_addr = *reinterpret_cast<in_addr*>(&stAddr.ip_num);
        *namelen = 8; // sizeof(uint16_t) + sizeof(uint16_t) + sizeof(struct in_addr)
    }

    return nRes;
}

inline
int setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
    return SetErrnoOnError(NU_Setsockopt(s, level, optname, const_cast<void*>(optval), optlen ));
}

inline
int getsockopt(int s, int level, int optname, void *optval, int *optlen)
{
    return SetErrnoOnError(NU_Getsockopt(s, level, optname, optval, optlen ));
}

inline
int select(int s, fd_set* pReadSet, fd_set* pWriteSet, fd_set* pExceptionSet, timeval* pstTimeval)
{
    int nStatus = NU_Select(s, pReadSet, pWriteSet, pExceptionSet, pstTimeval->tv_sec);

    // The following maps NU_Select return code to something more BSD compliant.

    // For Nucleus 0 mean a success and at least 1 descriptor is set.
    if (nStatus == 0)
    {
        nStatus = 1;
    }
    // For Nucleus NU_NO_DATA also means a success but 0 descriptor is set.
    else if (nStatus == NU_NO_DATA)
    {
        nStatus = 0;
    }
    return nStatus;
}
//M5T_INTERNAL_USE_END
#endif // #if defined(MXD_OS_NUCLEUS)

//-- The following forward declaration is for documentation purposes only.
#if 0
int MxCloseSocket(IN mxt_hSocket hSocket);
#endif

//==========================================================================
//==
//== MxCloseSocket
//==
//==========================================================================
//<GROUP NETWORK_FUNCTIONS>
//
//  Summary:
//      Closes a socket.
//
//  Parameters:
//      hSocket:
//          Handle to a socket.
//
//  Returns:
//      Returns 0 on success, -1 on error.
//
//  Description:
//      Closes a socket handle.
//
//==========================================================================
inline
int MxCloseSocket( IN mxt_hSocket hSocket )
{
#if defined(MXD_OS_WINDOWS) || \
    defined(MXD_OS_WINDOWS_CE)
    return closesocket(hSocket);
#elif defined(MXD_OS_LINUX)   || \
      defined(MXD_OS_ANDROID) || \
      defined(MXD_OS_SOLARIS) || \
      defined(MXD_OS_DARWIN)  || \
      defined(MXD_OS_VXWORKS)
    return close( hSocket );
#elif defined(MXD_OS_NUCLEUS)
    return SetErrnoOnError(NU_Close_Socket(hSocket));
#elif defined(MXD_OS_SYMBIAN)
    hSocket->Close();
    return 0;
//BRCM_CUSTOM - [begin] Add eCos support.
#elif defined ( MXD_OS_BOS_ECOS )
    if ( bosSocketClose( hSocket ) != BOS_STATUS_OK )
    {
       return nSOCKET_ERROR;
    }
    return 0;
//BRCM_CUSTOM - [end] Add eCos support.
#endif
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_SOCKETDEFINES_H
