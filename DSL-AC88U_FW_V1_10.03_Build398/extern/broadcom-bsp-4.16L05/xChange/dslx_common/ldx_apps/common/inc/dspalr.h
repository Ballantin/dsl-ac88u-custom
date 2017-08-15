/**************************************************************************
*
* dspalr.h - DSP Application Load Record definitions
*
* Purpose:  This header file contains definitions for the DSP Application
*           Load Record (ALR) used by the DSP bootloader.
*
* Notes:
*
*  1.  DSP APPLICATION LOAD RECORD OUTPUT FORMAT:
*
*      The elements of the C arrays in the DSP application load records are
*      16-bit unsigned words.  There is a short header, followed by the
*      ELF sections.  The binary will be output in the byte endian format
*      selected, while the C array generated is machine agnostic.
*
*     DSP application load record format:
*
*     {
*        === DSP record header starts ===
*
*           <header> (see note 2)
*
*        === ELF sections start ===
* 
*           <application data> (see note 3)
*
*     }
*
*  2. DSP APPLICATION LOAD RECORD HEADER
*
*      The first four entries in the array (8 bytes) constitute the header
*      portion of the array.  This header contains the byte length and byte
*      checksum of the application data portion of the array. Note that the
*      values in the header are independent of the header itself since they
*      are calculated only on the application data following the header. Each
*      value in the header is a 32-bit value, output as 16-bit halves, with
*      the high word preceding the low word.
*
*     DSP application load record format:
*
*     {
*        === DSP record header starts ===
*
*           LENGTH_HI, LENGTH_LO, CHECKSUM_HI, CHECKSUM_LO
*
*        === ELF sections start ===
* 
*           <application data> (see note 3)
*
*     }
*        The DSP record header consists of:
*              LENGTH_HI/LO   - high/low 16-bit word of 32-bit byte length
*                               calculated over the application data following
*                               the DSP record header.
*              CHECKSUM_HI/LO - high/low 16-bit word of 32-bit byte checksum
*                               calculated over the application data following
*                               the DSP record header.
*
*  3.  DSP APPLICATION LOAD RECORD APPLICATION DATA
*
*     Depending on the type of load record being generated,
*     the application data comes in two formats:
*
*     bootloader-type DSP load record format:
*
*        In the case of a bootloader record, the application data
*        contains the 16-bit binary opcodes for the bootloader application.
*
*     {
*        === DSP record header starts ===
*
*           <header> (see previous note)
*
*        === ELF sections start ===
*
*           bootloader/JTAG monitor instruction opcodes...
*
*     }
*        Note that the bootloader-type record contains only one program 
*        section with no section header.  The branch and load address
*        for the bootloader are determined by the hardware platform;
*        therefore, none are specified in the DSP record.
*
*     application-type DSP load record format:
*
*     In the case of a non-bootloader application, the application data
*     consists of the data that is sent to the DSP bootloader.  This
*     data must be interpreted by the bootloader and consists of a series
*     of ELF sections followed by a branch address.  Each ELF section is
*     preceded by a header, while the branch address information is
*     transmitted as part of the branch section header.
*
*     {
*        === DSP record header starts ===
*
*           <header> (see note 2)
*
*        === ELF sections start ===
*
*           ID, ADDR_HI, ADDR_LO, LENGTH,  <-- prog/data ELF section header 1
*           section contents...
*
*           ID, ADDR_HI, ADDR_LO, LENGTH,  <-- prog/data ELF section header 2
*           section contents...
*
*              :  :
*
*           ID, ADDR_HI, ADDR_LO, LENGTH,  <-- prog/data ELF section header N
*           section contents...
*
*           ID, BRANCH_HI, BRANCH_LO       <-- branch section header
*     }
*
*        The ELF section header consists of:
*              ID      - identifies program or data section
*              ADDR_HI - load address, high 16-bit word
*              ADDR_LO - load address, low 16-bit word
*              LENGTH  - length of section, in 16-bit words
*
*        Note that the contents of each section is preceded by a header
*        containing information for the bootloader regarding the action
*        to take with the section contents.  The final section is not
*        truly describing an ELF section but the branch address for
*        exiting the bootloader and running the loaded DSP application.
*
*              Copyright (c) 2004, Broadcom Canada Ltd.
*                          All Rights Reserved
*
**************************************************************************/

#ifndef DSPALR_H
#define DSPALR_H

/*
** DSP Application Load Record types
*/
typedef unsigned char  DSPALR_UINT8;
typedef unsigned short DSPALR_UINT16;
typedef unsigned int   DSPALR_UINT32;

/*
** DSP Application Load Record section header IDs.
*/
#define DSPALR_PROG_SECTION_ID    0xa500
#define DSPALR_DATA_SECTION_ID    0xa501
#define DSPALR_BRANCH_SECTION_ID  0xa502
#define DSPALR_OTHER_SECTION_ID   0xa503

/*
** DSP Application Load Record maximum number of sections
*/
#define DSPALR_MAX_SECTIONS    100

/*
** DSP Application Load Record output mode
*/
typedef enum
{
  DSPALR_BIG_ENDIAN    = 0x000b,
  DSPALR_LITTLE_ENDIAN = 0x000e

} DSPALR_ENDIAN;

/*
** DSP Application Load Record public functions
*/

int dspalrCreateRecord( char *binFileName,
                        char *textFileName,
                        char *arrayName,
                        DSPALR_ENDIAN outputMode );

int dspalrCloseRecord( void );

int dspalrWriteElfHeader( DSPALR_UINT16 sectionId,
                          DSPALR_UINT32 sectionAddress,
                          DSPALR_UINT16 sectionSize );

int dspalrWriteData( DSPALR_UINT16 data );

int dspalrWriteBranchHeader( DSPALR_UINT32 entry_point );

DSPALR_UINT32 dspalrGetByteChecksum( void );
DSPALR_UINT32 dspalrGetByteLength( void );

#endif /* DSPALR_H */
