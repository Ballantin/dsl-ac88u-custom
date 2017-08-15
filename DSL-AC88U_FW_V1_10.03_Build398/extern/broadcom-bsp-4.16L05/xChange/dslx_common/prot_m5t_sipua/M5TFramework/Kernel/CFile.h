//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
//==============================================================================
#ifndef MXG_CFILE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CFILE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

// CFile is supported only on these operating systems.
#if defined(MXD_OS_WINDOWS)    || \
    defined(MXD_OS_WINDOWS_CE) || \
    defined(MXD_OS_LINUX)      || \
    defined(MXD_OS_SOLARIS)    || \
    defined(MXD_OS_DARWIN)     || \
    defined(MXD_OS_VXWORKS)    || \
    defined(MXD_OS_SYMBIAN)    || \
    defined(MXD_OS_NUCLEUS)    || \
    defined(MXD_OS_ANDROID)

//-- Framework Configuration
//---------------------------
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h" // MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION
#endif

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
    #endif
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

#include <sys/stat.h>
#include <fcntl.h>
#if defined(MXD_OS_VXWORKS)
    #include <stdio.h>
    #include <ioLib.h>
#elif defined(MXD_OS_WINDOWS) || \
      defined(MXD_OS_WINDOWS_CE)
    #include <io.h>
#elif defined(MXD_OS_NUCLEUS)
    #include "posix/inc/unistd.h"
#elif defined(MXD_OS_SYMBIAN)
    #include <stdio.h>
    #include <sys/unistd.h>
#endif
MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//<GROUP KERNEL_TYPES>
//
//  Summary:
//    File descriptor basic data type.
//
//  Description:
//    The file descriptor is used to access a file. It is created when opening
//    a file and is usually valid until the file is closed. All methods of CFile use
//    this descriptor when calling the operating system's file primitives.
//
//    On all operating systems, the descriptor is an integer except on Symbian
//    where it is a pointer to a RFile object.
//
//    A file descriptor should never be kept because it can be invalidated
//    by the following methods: Close, Truncate.
//    On Windows CE, the Stat methods can also invalidate a file descriptor.
//
//  See Also:
//    GetFileDescriptor
//==============================================================================
typedef int mxt_fd;

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_OS_WINDOWS) || \
    defined(MXD_OS_WINDOWS_CE)
    #if !defined(mode_t)
        typedef int mode_t;
    #endif
    #if !defined(off_t)
        typedef long off_t;
    #endif
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const mxt_fd INVALID_FILE_DESCRIPTOR = -1;
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: CFile
//========================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//  Class handling file manipulation.
//
// Description:
//  CFile is the class that handles file manipulation. It provides a common
//  interface with unbuffered methods for file manipulation on all operating
//  systems.
//
//  It is essentially a wrapper above the operating system file's primitives.
//  It does not support operations on directories. Thus it does not support
//  creating a file in a new directory using the open method.
//
//  CFile can be safely shared between multiple threads. This means that all
//  operations from each thread will be executed but the application is
//  responsible to synchronize these operations from each thread.
//
// Location:
//   Kernel/CFile.h
//
//==EDOC========================================================================
class CFile
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CFile();

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CFile();


//M5T_INTERNAL_USE_BEGIN

#if defined(MXD_CFILE_TRACES_ENABLE_SUPPORT)

    #define MX_TRACE_CFILE0 MX_TRACE0
    #define MX_TRACE_CFILE1 MX_TRACE1
    #define MX_TRACE_CFILE2 MX_TRACE2
    #define MX_TRACE_CFILE3 MX_TRACE3
    #define MX_TRACE_CFILE4 MX_TRACE4
    #define MX_TRACE_CFILE5 MX_TRACE5
    #define MX_TRACE_CFILE6 MX_TRACE6
    #define MX_TRACE_CFILE7 MX_TRACE7
    #define MX_TRACE_CFILE8 MX_TRACE8
    #define MX_TRACE_CFILE9 MX_TRACE9

#else

    #define MX_TRACE_CFILE0 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE1 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE2 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE3 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE4 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE5 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE6 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE7 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE8 MX_TRACE_EMPTY
    #define MX_TRACE_CFILE9 MX_TRACE_EMPTY

#endif // #if defined(MXD_CFILE_TRACES_ENABLE_SUPPORT)

    // Non exhaustive list of flags
#if defined(MXD_OS_WINDOWS) || \
    defined(MXD_OS_WINDOWS_CE)

    #if !defined(O_RDONLY)
        #define O_RDONLY _O_RDONLY
    #endif
    #if !defined(O_WRONLY)
        #define O_WRONLY _O_WRONLY
    #endif
    #if !defined(O_RDWR)
        #define O_RDWR _O_RDWR
    #endif
    #if !defined(O_CREAT)
        #define O_CREAT _O_CREAT
    #endif
    #if !defined(O_TRUNC)
        #define O_TRUNC _O_TRUNC
    #endif
    #if !defined(O_APPEND)
        #define O_APPEND _O_APPEND
    #endif
    #if !defined(O_BINARY)
        #define O_BINARY _O_BINARY
    #endif
    #if defined(MXD_OS_WINDOWS)
        #if !defined(O_TEXT)
            #define O_TEXT _O_TEXT
        #endif
    #elif defined(MXD_OS_WINDOWS_CE)
        #if defined(O_TEXT)
            #undef O_TEXT
        #endif
        #define O_TEXT 0x4000
    #endif
#elif defined(MXD_OS_LINUX)   || \
      defined(MXD_OS_DARWIN)  || \
      defined(MXD_OS_VXWORKS) || \
      defined(MXD_OS_SOLARIS) || \
      defined(MXD_OS_NUCLEUS) || \
      defined(MXD_OS_ANDROID)

    // O_BINARY/O_TEXT flags are not defined under these operating systems because text and
    // binary files are the same thing (see http://djgpp.linux-mirror.org/v2faq/faq9_4.html).
    // We set a specific value to binary and text flag to be able to detect theses flags
    // when opening a file.
    #if defined(O_BINARY)
        #undef O_BINARY
    #endif
    #define O_BINARY 0x80000
    #if defined(O_TEXT)
        #undef O_TEXT
    #endif
    #define O_TEXT 0x40000

#endif // #if defined(MXD_OS_WINDOWS) ||
       //     defined(MXD_OS_WINDOWS_CE)

// On all OS except Nucleus the value of 0_RDONLY is 0 so we need to replace
// it by another value to be able to detect it in the open method.
#if !defined(MXD_OS_NUCLEUS)
    #undef O_RDONLY
    #define O_RDONLY 0x0004
#endif

//M5T_INTERNAL_USE_END

    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    //   Defines the possible flags to use when opening a file.
    //
    // Description:
    //   When opening a file, you can combine these flags to
    //   obtain the behaviour you want when accessing the file.
    enum EFlags
    {
        // Description:
        // Opens a file for reading only. No writing operations are allowed.
        eREAD_ONLY  = O_RDONLY,

        // Description:
        // Opens a file for writing only. No reading operations are allowed.
        eWRITE_ONLY = O_WRONLY,

        // Description:
        // Opens a file for reading and writing. This is the default flag.
        eREAD_WRITE = O_RDWR,

        // Description:
        // Opens a file and creates it if it does not exist. If the file already
        // exists, it will still be opened. It cannot be used with read-only flag.
        eCREATE     = O_CREAT,

        // Description:
        // Truncates a file upon opening. The file must be opened in write mode
        // (read-write or write-only), otherwise open fails. The file will
        // have a size of 0 bytes after this operation.
        eTRUNCATE   = O_TRUNC,

        // Description:
        // Appends every write operation to the end of file. The file must be
        // opened with write mode (read-write or write-only), otherwise open
        // fails.
        eAPPEND     = O_APPEND,

        // Description:
        // Opens a file in binary mode. This is the default mode.
        eBINARY     = O_BINARY,

        // Description:
        // Opens a file in text mode. This mode is only available on Windows.
        // It will be ignored on all other operating systems.
        eTEXT       = O_TEXT
    };

//M5T_INTERNAL_USE_BEGIN

    // Non exhaustive list of modes
#if defined(MXD_OS_WINDOWS) || \
    defined(MXD_OS_WINDOWS_CE)
        #if !defined(S_IRWXU)
            #define S_IRWXU     (S_IREAD | S_IWRITE | S_IEXEC)
        #endif
#elif defined(MXD_OS_VXWORKS)
        #if !defined(S_IREAD)
            #define S_IREAD     0000400
        #endif
        #if !defined(S_IWRITE)
            #define S_IWRITE    0000200
        #endif
        #if !defined(S_IEXEC)
            #define S_IEXEC     0000100
        #endif
        #if !defined(S_IRWXU)
            #define S_IRWXU     (S_IREAD | S_IWRITE | S_IEXEC)
        #endif
#elif defined (MXD_OS_NUCLEUS)
        #if !defined(S_IREAD)
            #define S_IREAD     O_RDONLY
        #endif
        #if !defined(S_IWRITE)
            #define S_IWRITE    O_WRONLY
        #endif
        #if !defined(S_IEXEC)
            #define S_IEXEC     ( O_RDONLY | O_WRONLY )
        #endif
        #if !defined(S_IRWXU)
            #define S_IRWXU     ( O_RDONLY | O_WRONLY )
        #endif
#elif defined (MXD_OS_ANDROID)
        #if !defined(S_IREAD)
            #define S_IREAD     O_RDONLY
        #endif
        #if !defined(S_IWRITE)
            #define S_IWRITE    O_WRONLY
        #endif
        #if !defined(S_IEXEC)
            #define S_IEXEC     ( O_RDONLY | O_WRONLY )
        #endif
        #if !defined(S_IRWXU)
            #define S_IRWXU     ( O_RDONLY | O_WRONLY )
        #endif
#endif

//M5T_INTERNAL_USE_END

    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    // Defines the possible file modes.
    enum EMode
    {
        // Description:
        // Read mode.
        eUSER_R     =  S_IREAD,
        // Description:
        // Write mode.
        eUSER_W     =  S_IWRITE,
        // Description:
        // Execute mode.
        eUSER_X     =  S_IEXEC,
        // Description:
        // Read/Write/Execute mode.
        eUSER_RWX   =  S_IRWXU
    };

    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    // Defines the possible available seek positions.
    enum ESeekPositions
    {
        // Description:
        // Offset is relative to the start of the file
        eSEEK_SET   =  SEEK_SET,
        // Description:
        // Offset is relative to the current position in the file.
        eSEEK_CUR   =  SEEK_CUR,
        // Description:
        // Offset is relative to the end of the file.
        eSEEK_END   =  SEEK_END
    };

    // <GROUP KERNEL_STRUCTS>
    //
    // Summary:
    // Structure holding the information related to a file.
    // Not all data members will be filled on all operating systems.
    struct SStat
    {
        // Description:
        // Device of the file.
        uint32_t uDevice;
        // Description:
        // Inode of the file.
        uint32_t uInode;
        // Description:
        // Mode of the file.
        uint16_t uMode;
        // Description:
        // Link count of the file.
        int16_t nLinkCount;
        // Description:
        // User ID of the file.
        int16_t nUserId;
        // Description:
        // Group ID of the file.
        int16_t nGroupId;
        // Description:
        // Device type of the file.
        uint32_t uDeviceType;
        // Description:
        // Size of the file in bytes.
        int32_t nSize;
        // Description:
        // Access time of the file.
        time_t nAccessTime;
        // Description:
        // Modification time of the file.
        time_t nModificationTime;
        // Description:
        // Change time of the file.
        time_t nChangeTime;
        // Description:
        // Block size of the file.
        int32_t nBlockSize;
        // Description:
        // Block count of the file.
        int32_t nBlockCount;
        // Description:
        // Attributes of the file.
        uint8_t uAttribute;
    };

    //  Summary:
    //  Opens a file in the file system.
    //---------------------------------
    mxt_result Open(IN const char* pszPathName, IN EFlags eFlags, IN EMode eMode);

    //  Summary:
    //  Closes the file.  Returns resS_OK if the operation succeeded.
    //-------------------------------------------------------------
    mxt_result Close();

    //  Summary:
    //  Reads bytes from the file.
    //-----------------------------
    int Read(OUT void* uBuffer, IN unsigned int uSizeInBytes);

    //  Summary:
    //  Writes the buffer in the file.
    //---------------------------------
    int Write(IN const void* uBuffer, IN unsigned int uSizeInBytes);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    //  Summary:
    //  Reads bytes from the file into a blob.
    mxt_result Read(OUT CBlob* pBlob, IN unsigned int uSizeInBytes);

    //  Summary:
    //  Reads the entire file into a blob.
    mxt_result Read(OUT CBlob* pBlob);

    //  Summary:
    //  Writes bytes from a blob into the file.
    mxt_result Write(IN const CBlob& rBlob, IN unsigned int uSizeInBytes);

    //  Summary:
    //  Writes the entire blob into the file.
    mxt_result Write(IN const CBlob& rBlob);
#endif

    //  Summary:
    //  Changes the value of the offset of the file descriptor.
    //----------------------------------------------------------
    int Seek(IN int nOffsetInBytes, IN ESeekPositions eSeekPosition);

    //  Summary:
    //  Gets the file information.
    //-----------------------------
    mxt_result Stat(OUT struct SStat* pstStat);

    //  Summary:
    //  Truncates a file.
    //--------------------
    mxt_result Truncate(IN int nOffsetInBytes);

    //  Summary:
    //  Gets the file descriptor.
    //----------------------------
    mxt_fd GetFileDescriptor();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CFile(IN const CFile& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CFile& operator=(IN const CFile& rSrc);

    void Reset();

    //-- << Helper methods >>

#if defined(MXD_OS_WINDOWS_CE)

    // Summary:
    //  Converts the time from a filetime structure to a time_t structure.
    static time_t Convert_FILETIME_To_time_t(IN FILETIME* pFiletime);

#endif // #if defined(MXD_OS_WINDOWS_CE)

#if defined(MXD_OS_SYMBIAN)

    static mxt_result InitializeCFile();
    static void FinalizeCFile();

// Friends declarations
//----------------------
    friend mxt_result InitializeCFile();
    friend void FinalizeCFile();

#endif // #if defined(MXD_OS_SYMBIAN)

//-- Hidden Data Members
//------------------------
protected:
private:
    mxt_fd m_currentFileDescriptor;
    EFlags m_eFlags;
    EMode m_eMode;

#if defined(MXD_OS_WINDOWS_CE) || \
    defined(MXD_OS_SYMBIAN) || \
    defined(MXD_OS_VXWORKS) || \
    defined(MXD_OS_NUCLEUS)

    // Summary:
    //  Contains the path and name of current file. It must be stored for
    //  theses operating systems because sometimes they need to close the
    //  file and reopen it in order to accomplish an operation like Stat
    //  and Truncate. Theses methods must carefully make a copy of this
    //  pathname before closing the file because Close method clean all
    //  internal data.
    char* m_pszPathName;

#endif // #if defined(MXD_OS_WINDOWS_CE) ||
       //     defined(MXD_OS_SYMBIAN) ||
       //     defined(MXD_OS_VXWORKS) ||
       //     defined(MXD_OS_NUCLEUS)

#if defined(MXD_OS_SYMBIAN)

    // Summary:
    //  File object used for all operations on file.
    RFile m_hFile;

#endif // #if defined(MXD_OS_SYMBIAN)

#if defined(MXD_OS_NUCLEUS)

    // Summary:
    //  Positions for read and write in the file. This is useful because
    //  seeking in Nucleus returns the number of bytes we seeked
    //  instead of the position in the file. So we need to always keep
    //  up to date this variable in order to be able to return the current
    //  position when seeking with SEEK_CUR flag.
    int m_nPosition;

#endif // #if defined(MXD_OS_NUCLEUS)

#if defined(MXD_OS_SYMBIAN) || \
    defined(MXD_OS_NUCLEUS) || \
    defined(MXD_OS_WINDOWS_CE)

    // Summary:
    //  Number of bytes to extend the file on next write operation.
    //  This number is set when seeking beyond end of file and file
    //  extention is done on next write only.
    int m_nNumberOfBytesToExtend;

#endif // #if defined(MXD_OS_SYMBIAN) ||
       //     defined(MXD_OS_NUCLEUS) ||
       //     defined(MXD_OS_WINDOWS_CE)

//M5T_INTERNAL_USE_END
};

// Forward declarations


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
#if !defined(MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION)

//==SDOC========================================================================
//==
//==  GetFileDescriptor
//==
//==============================================================================
//
//  Returns:
//      The file descriptor or a pointer to the RFile object. Returns -1
//      for an invalid file descriptor.
//
//  Description:
//      Gets the file descriptor of the currently opened file.
//
//      The file descriptor returned should not be kept as it can be invalidated
//      by the following methods:
//      - Truncate
//      - Close
//      - Stat // Windows CE
//
//==EDOC========================================================================
inline mxt_fd CFile::GetFileDescriptor()
{
#if defined(MXD_OS_SYMBIAN)

    // On Symbian there is no file descriptor.
    // The RFile object serves as a file descriptor. Thus we
    // return the address of this object.
    if (m_currentFileDescriptor != INVALID_FILE_DESCRIPTOR)
    {
        return (mxt_fd)&m_hFile;
    }
    else
    {
        return -1;
    }

#else

    return m_currentFileDescriptor;

#endif
}

//==SDOC========================================================================
//==
//==  MxRemove
//==
//==============================================================================
//<GROUP KERNEL_FUNCTIONS>
//  <FLAG Deprecated since v2.1.5>
//
//  Summary:
//      Removes the specified file.
//
//  Parameters:
//      pszPathName:
//          Name and path of the file to remove.
//
//  Returns:
//      - true: File removed successfully.
//      - false: Cannot remove file. It may be already opened or does not exist.
//
//  Description:
//      Removes the specified file.
//
//      The use of this method is deprecated in favor of the new method
//      CFileTools::Remove().
//
//  See Also:
//      Kernel/CFileTools.h
//
//==EDOC========================================================================
bool MxRemove(IN const char* pszPathName);

//==SDOC========================================================================
//==
//==  MxRename
//==
//==============================================================================
//<GROUP KERNEL_FUNCTIONS>
//
//  <FLAG Deprecated since v2.1.5>
//
//  Summary:
//      Renames the specified file.
//
//  Parameters:
//      pszOldPathName:
//          Original name and path of the file.
//
//      pszNewPathName:
//          New name and path of the file.
//
//  Returns:
//      - true: File renamed successfully.
//      - false: Cannot rename file. It may be already opened or does not exist.
//
//  Description:
//      Renames the specified file. It will also move it to another directory
//      if the path has been changed.
//
//      The use of this method is deprecated in favor of the new method
//      CFileTools::Rename().
//
//  See Also:
//      Kernel/CFileTools.h
//
//==EDOC========================================================================
bool MxRename(IN const char* pszOldPathName, IN const char* pszNewPathName);

//==SDOC========================================================================
//==
//==  MxChmod
//==
//==============================================================================
//<GROUP KERNEL_FUNCTIONS>
//
//  <FLAG Deprecated since v2.1.5>
//
//  Summary:
//      Changes the file's mode.
//
//  Parameters:
//      pszPathName:
//          Name and path of the file to change permissions.
//
//      eMode:
//          Mode representing the premissions to set for the file.
//
//  Returns:
//      - true: File's permissions changed.
//      - false: Cannot change file's permissions. It may be already opened
//        or does not exist.
//
//  Description:
//      Changes the permissions on the specified file.
//
//      This function may have a different behavior on different operating
//      systems. Please refer to the SetFileAttributes method documentation for
//      the Windows operating system and the chmod method documentation for
//      other operating systems.
//
//      The use of this method is deprecated in favor of the new method
//      CFileTools::ChangeMode().
//
//  See Also:
//      Kernel/CFileTools.h
//
//==EDOC========================================================================
bool MxChmod(IN const char* pszPathName, IN CFile::EMode eMode);

#endif  // #if !defined(MXD_FILESYSTEM_PROPRIETARY_IMPLEMENTATION)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_OS_WINDOWS)    ||
       //     defined(MXD_OS_WINDOWS_CE) ||
       //     defined(MXD_OS_LINUX)      ||
       //     defined(MXD_OS_SOLARIS)    ||
       //     defined(MXD_OS_DARWIN)     ||
       //     defined(MXD_OS_VXWORKS)    ||
       //     defined(MXD_OS_SYMBIAN)    ||
       //     defined(MXD_OS_NUCLEUS)    ||
       //     defined(MXD_OS_ANDROID)

#endif //-- #ifndef MXG_CFILE_H
