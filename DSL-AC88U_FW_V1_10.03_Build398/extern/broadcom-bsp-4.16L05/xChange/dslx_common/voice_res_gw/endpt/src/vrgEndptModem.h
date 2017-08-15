#ifndef __VRGENDPTMODEM_H__
#define __VRGENDPTMODEM_H__

#define AUDIO_BUFF_SIZE       256
/* Only for 63138/63148 platforms using 50MHz clock*/
#define GET_TIME_STAMP()      ((*(volatile unsigned int*)0xFCFE809C) | 0xC0000000)
/* Sign extend the time difference */
#define GET_TIME_DIFF(a)      (((a) & 0x20000000) ? ((a) | 0xC0000000) : ((a) & 0x3fffffff))

#define array_size(a)         sizeof(a)/sizeof(a[0])

typedef char                  int8;
typedef short int             int16;
typedef int                   int32;
typedef long long             int64;

typedef unsigned char         uint8;
typedef unsigned short int    uint16;
typedef unsigned int          uint32;
typedef unsigned long long    uint64;

typedef unsigned char         bits8;
typedef unsigned short int    bits16;
typedef unsigned int          bits32;
typedef unsigned long long    bits64;

typedef int (l2mux_audio_fn)(unsigned char *buffer, size_t size, uint8_t phonet_dev_id);

extern int l2mux_audio_tx (unsigned char *buffer, int size, char phonet_dev_id);
extern int l2mux_audio_rx_register(l2mux_audio_fn *fn);
extern int l2mux_audio_rx_unregister(int handle);
//extern void sendAudioEncodeNotification (char* payload, unsigned short audioHeader, unsigned short subBlockLength);
extern void sendAudioEncodeNotification (void);
extern void swapShortWord (unsigned char* ptr, unsigned short length);

extern volatile unsigned int gAudioULStart;
extern volatile unsigned int gAudioULReady;
extern volatile int gCSCnxId;

/* Big Endian read/write macros.
 *******************************
 * These macros read data from or write data to Big Endian formatted
 * message (or other data), i.e. MSByte in lowest address, regardless 
 * of the local endianess (=used processor endianess).
 * 
 * Big Endian macros shall be used for example with ISI messages.
 *
 * Examples: PUTB16(&isi_msg->big_endian_16bit_elem, uint16_value);
 *           uint32_value = GETB16(&isi_msg->big_endian_32bit_elem);
 * where isi_msg is defined to have elements in big endian format
 */


/* GETB8 macro returns byte value from given address.
 * This macro is used mainly by other GETBXX macros.
 */
#define GETB8(p) \
    (*(uint8 *)(p))


/* GETB16 macro reads 16-bit value in Big Endian format
 * from given address. 16-bit value is read as two separate
 * bytes, and so this macro can read 16-bit value from whatever
 * address, regardless of the processor alignment restrictions
 */
#define GETB16(p) \
    (((uint16) GETB8(p)<<8) | (uint16) GETB8((uint8 *)(p)+1))


/* GETB32 macro reads 32-bit value in Big Endian format
 * from given address. 32-bit value is read as four separate 
 * bytes, and so this macro can read 32-bit value from whatever
 * address, regardless of the processor alignment restrictions
 */
#define GETB32(p) \
    (((uint32) GETB16(p)<<16) | (uint32) GETB16((uint8 *)(p)+2))


/* GETB64 macro reads 64-bit value in Big Endian format
 * from given address. 64-bit value is read as eight separate 
 * bytes, and so this macro can read 64-bit value from whatever
 * address, regardless of the processor alignment restrictions
 */
#define GETB64(p) \
    (((uint64) GETB32(p)<<32) | (uint64) GETB32((uint8 *)(p)+4))


/* GETB64_MS/LS32BITS macros read 32 MSBits or 32 LSBits of 
 * 64-bit value that is in Big Endian format
 */

#define GETB64_MS32BITS(p) GETB32(p)
#define GETB64_LS32BITS(p) GETB32(((uint8 *)(p))+4)




/* PUTB8 macro writes byte value to given address.
 * This macro is used mainly by other PUTBXX macros.
 */
#define PUTB8(p,v) \
    {(*(uint8 *)(p)) = ((uint8)(v));}


/* PUTB16 macro writes 16-bit value in Big Endian format
 * to given address. 16-bit value is written as two separate
 * bytes, and so this macro can write 16-bit value to whatever
 * address, regardless of the processor alignment restrictions
 */
#define PUTB16(p,v) /*lint -e{572,778}*/ \
    {PUTB8((p),(uint16)(v)>>8); PUTB8((uint8*)(p)+1,v);}


/* PUTB32 macro writes 32-bit value in Big Endian format
 * to given address. 32-bit value is written as four separate
 * bytes, and so this macro can write 32-bit value to whatever
 * address, regardless of the processor alignment restrictions
 */
#define PUTB32(p,v) /*lint -e{572,778}*/ \
    {PUTB16((p),(uint32)(v)>>16); PUTB16((uint8*)(p)+2,(uint32)(v));}


/* PUTB64 macro writes 64-bit value in Big Endian format
 * to given address. 64-bit value is written as eight separate
 * bytes, and so this macro can write 64-bit value to whatever
 * address, regardless of the processor alignment restrictions
 */
#define PUTB64(p,v) /*lint -e{572,778}*/ \
    {PUTB32((p),(uint64)(v)>>32); PUTB32((uint8*)(p)+4,(uint64)(v));}


/* ----------------------------------------------------------------------- */
/* Message ID's                                                            */
/* ----------------------------------------------------------------------- */

#define MODEM_AUDIO_SPEECH_CODEC_REQ             0x03
#define MODEM_AUDIO_SPEECH_CODEC_RESP            0x04
#define MODEM_AUDIO_SPC_TIMING_REQ               0x0B
#define MODEM_AUDIO_SPC_TIMING_RESP              0x0C
#define MODEM_AUDIO_SPC_TIMING_NEEDED_NTF        0x2F
#define MODEM_AUDIO_SPC_DECODER_DATA_REQ         0xB0
#define MODEM_AUDIO_SPC_ENCODER_DATA_NTF         0xB2

/* ----------------------------------------------------------------------- */
/* Subblock ID's                                                           */
/* ----------------------------------------------------------------------- */

#define MODEM_AUDIO_SB_SPC_CODEC_FRAME           0x007B


/* phonet message header */
typedef struct
{
    uint8_t media;
    uint8_t receiver;
    uint8_t sender;
    uint8_t function;
    uint16_t length;
    uint8_t receiver_obj;
    uint8_t sender_obj;
} pn_msg_head;

typedef pn_msg_head pn_msg_header;

/* phonet message header */
typedef struct
{
    uint8   TransactionId;
    uint8   MessageId;
}isi_msg_head;

typedef isi_msg_head isi_msg_header;


/* phonet and ISI message header */
typedef struct
{
    pn_msg_header    phonet;
    isi_msg_header   isi;
} pn_isi_msg_head;

typedef pn_isi_msg_head pn_isi_msg_header;

/* ----------------------------------------------------------------------- */
/* Message: AUDIO_CTRL_READY_REQ                                           */
/* ----------------------------------------------------------------------- */
/* This message is used by the modem to request DSP is READY.
*/

typedef struct
{
     pn_msg_header pn_hdr;
     uint8   trans_id;
     uint8   message_id;
     uint8   type;
     uint8   sub_type;
} AudioControlReadyRequest;

typedef AudioControlReadyRequest tISI_AudioCtrlReadyReq;


/* ----------------------------------------------------------------------- */
/* Message: AUDIO_CTRL_READY_RESP                                          */
/* ----------------------------------------------------------------------- */
/* This message is used to inform the modem that speech
   codec DSP is READY.
*/

typedef struct
{
     pn_msg_header pn_hdr;
     uint8   trans_id;
     uint8   message_id;
     uint8   type;
     uint8   sub_type;
     uint8   fill[3];
     /* Values from the constant table AUDIO_CTRL_RESP_REASON */
     uint8   status;
} AudioControlReadyResponse;

typedef AudioControlReadyResponse tISI_AudioCtrlReadyRsp;

/* ----------------------------------------------------------------------- */
/* Constant Table: AUDIO_CTRL_RESP_REASON                                  */
/* ----------------------------------------------------------------------- */

#define READY_RESP_OK                            0x00
#define READY_RESP_FAIL                          0x01


/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPEECH_CODEC_REQ                                   */
/* ----------------------------------------------------------------------- */
/* This message is used by the modem to request  the setup, modification or
   release of the audio codecs. The message specifies the codec to be used,
   and related parameters.
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    int16   spare;
    /* Values from the constant table MODEM_AUDIO_SPC_USE */
    uint16  SpeechCodecUse;
    uint16  SpeechCodec; /* Values from the constant table MODEM_AUDIO_SPC */
    /* xxxxx-----------  Reserved1
       -----x----------  Audio Activity Control Flag
       ------x---------  NSYNC control
       -------xxx------  Speech frame number
       ----------xxxx--  AMR encoder
       --------------x-  Reserved2
       ---------------x  DTX use
    */
    bits16  SpeechEncoderConfiguration;
    /* xxxxxx----------  Filler
       ------xxxx------  Reserved3
       ----------xxxx--  AMR decoder
       --------------x-  Reserved4
       ---------------x  Filler
    */
    bits16  SpeechDecoderConfiguration;
    /* Values from the constant table MODEM_AUDIO_SPC_CONF_INIT */
    bits16  SpeechCodecInit;
    } ModemAudioSpeechCodecReq;

typedef ModemAudioSpeechCodecReq tISI_ModemAudioSpeechCodecReq;

/* Merge mask constants */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved1_OFFSET                0xBu  /*       11 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved1_WIDTH                 0x5u  /*        5 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved1_MASK               0xF800u  /*    63488 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_audio_activity_control_OFFSET          0xAu  /*       10 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_audio_activity_control_WIDTH          0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_audio_activity_control_MASK        0x400u  /*     1024 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_nsync_OFFSET                    0x9u  /*        9 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_nsync_WIDTH                     0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_nsync_MASK                    0x200u  /*      512 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_SpeechFrameNumber_OFFSET          0x6u  /*        6 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_SpeechFrameNumber_WIDTH          0x3u  /*        3 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_SpeechFrameNumber_MASK        0x1C0u  /*      448 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_encoder_OFFSET              0x2u  /*        2 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_encoder_WIDTH               0x4u  /*        4 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_encoder_MASK               0x3Cu  /*       60 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved2_OFFSET                0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved2_WIDTH                 0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved2_MASK                  0x2u  /*        2 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_DTX_use_OFFSET                  0x0u  /*        0 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_DTX_use_WIDTH                   0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_DTX_use_MASK                    0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved3_OFFSET                0x6u  /*        6 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved3_WIDTH                 0x4u  /*        4 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved3_MASK                0x3C0u  /*      960 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_decoder_OFFSET              0x2u  /*        2 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_decoder_WIDTH               0x4u  /*        4 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_decoder_MASK               0x3Cu  /*       60 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved4_OFFSET                0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved4_WIDTH                 0x1u  /*        1 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_reserved4_MASK                  0x2u  /*        2 */
/* Value constants */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_SpeechFrameNumber_MIN           0x0u  /*        0 */
#define MODEM_AUDIO_SPEECH_CODEC_REQ_SpeechFrameNumber_MAX           0x7u  /*        7 */


/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPEECH_CODEC_RESP                                  */
/* ----------------------------------------------------------------------- */
/* This message is used for informing the modem of whether the speech call
   can be started successfully or not.
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    int16   spare;
    /* Values from the constant table MODEM_AUDIO_SPC_REASON */
    uint16  Reason;
    } ModemAudioSpeechCodecResp;

typedef ModemAudioSpeechCodecResp tISI_ModemAudioSpeechCodecResp;

#define SPC_RESP_OK                              0x0000
#define SPC_RESP_FAIL_NO_RESOURCES               0x0001

/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPC_TIMING_REQ                                     */
/* ----------------------------------------------------------------------- */
/* 3GPP TS 43.050 specifies the maximum speech delay in the uplink direction.
   Audio subsystem needs to know the latest timing  when the encoded speech
   frame must be sent to the cellular subsystem and also the processing time
   reserved for transmission by the cellular subsystem.
   This message is used for controlling the timing of encoded uplink speech
   data transmitted from the audio sub system to external interface of the
   modem. Timing field contains information about when the data has to, at
   the latest, be on the upper interface of the modem.The timing request will
   only be sent when the speech encoding is started or when the timing needs
   to be corrected. The timing change request need not be synchronized with
   the natural timing of audio sub system, and may be sent at any time. After
   starting, the data is sent at the natural rate of system ie. every 20 ms
   in GSM and WCDMA.
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    /* Unsigned value which tells the number of microsends from when the
       timing message was sent until the time the data has to be on the upper
       interface of the modem 
    */
    uint16  timing_high;
    uint16  timing_low;
    /* Maximum time in microseconds needed for processing by GSM cellular
       subsystem, defined as Tencode + Trftx where
       - Tencode: Time required for channel encoder to perform channel
         encoding
       - Trftx: Time required for transmission of a TCH radio interface frame
         over the air due to interleaving and de-interleaving
    */
    uint16  processing_time;
    /* This parameter may be used for audio delay calculations in audio
       component if cellular subsystems have different audio delay
       requirements.
       A subset of values from the constant table MODEM_AUDIO_SPC_USE
    */
    uint16  subsystem;
    uint8   fill1[2];
    } ModemAudioSpcTimingReq;

typedef ModemAudioSpcTimingReq tISI_ModemAudioSpcTimingReq;

/* Value constants */
#define MODEM_AUDIO_SPC_TIMING_REQ_timing_high_MIN                   0x0u  /*        0 */
#define MODEM_AUDIO_SPC_TIMING_REQ_timing_high_MAX                0xFFFFu  /*    65535 */
#define MODEM_AUDIO_SPC_TIMING_REQ_timing_low_MIN                    0x0u  /*        0 */
#define MODEM_AUDIO_SPC_TIMING_REQ_timing_low_MAX                 0xFFFFu  /*    65535 */


/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPC_TIMING_RESP                                    */
/* ----------------------------------------------------------------------- */
/* This message is sent as a response to a request for setting the timing of
   uplink speech data.
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    /* Values from the constant table MODEM_AUDIO_SPC_TIMING_REASON */
    bits16  status;
    } ModemAudioSpcTimingResp;

typedef ModemAudioSpcTimingResp tISI_ModemAudioSpcTimingResp;


/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPC_TIMING_NEEDED_NTF                              */
/* ----------------------------------------------------------------------- */
/* This message is sent to cellular subsystem when new uplink timing is
   needed. Cellular subsystem will send a new MODEM_AUDIO_SPC_TIMING_REQ
   message with updated timing information when it receives this
   notification.
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    uint8   fill1[2];
    } ModemAudioSpcTimingNeededNtf;

typedef ModemAudioSpcTimingNeededNtf tISI_ModemAudioSpcTimingNeededNtf;


/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPC_DECODER_DATA_REQ                               */
/* ----------------------------------------------------------------------- */
/* This decoder message contains downlink speech data. The amount of actual
   data depends on codec.
   Only one subblock of audio data should be included in this version of the
   specification.
   See /6/ for the data format descriptions. 
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    bits16  data_format; /* Values from the constant table MODEM_AUDIO_SPC */
    uint8   fill1[2];
    uint16  num_sb;
    /* Subblock list:
       MODEM_AUDIO_SB_SPC_CODEC_FRAME
    */
    } ModemAudioSpcDecoderDataReq;

typedef ModemAudioSpcDecoderDataReq tISI_ModemAudioSpcDecoderDataReq;


/* ----------------------------------------------------------------------- */
/* Message: MODEM_AUDIO_SPC_ENCODER_DATA_NTF                               */
/* ----------------------------------------------------------------------- */
/* This message contains encoded uplink speech data. It should be sent to the
   ISA entity which sent the MODEM_AUDIO_SPEECH_CODEC_REQ. The amount of
   actual data in the message can vary depending on codec frame used. 
   See /6/ for the data format descriptions 
*/

typedef struct
    {
/* PhoNet header */
    uint8   media;
    uint8   receiver_dev;
    uint8   sender_dev;
    uint8   res;
    uint16  length;
    uint8   receiver_obj;
    uint8   sender_obj;
/* Message data */
    uint8   TransactionId;
    uint8   MessageId;
    bits16  data_format; /* Values from the constant table MODEM_AUDIO_SPC */
    uint8   fill1[2];
    uint16  num_sb;
    /* Subblock list:
       MODEM_AUDIO_SB_SPC_CODEC_FRAME
    */
    } ModemAudioSpcEncoderDataNtf;

typedef ModemAudioSpcEncoderDataNtf tISI_ModemAudioSpcEncoderDataNtf;




/* ----------------------------------------------------------------------- */
/* Subblock: MODEM_AUDIO_SB_SPC_CODEC_FRAME                                */
/* ----------------------------------------------------------------------- */
/* This sub block includes speech codec frame data. The audio frame starts
   after the subblock header. Its length depends on the used codec. 
*/

typedef struct
    {
    uint16  sb_id;
    uint16  sb_len;
    } MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR;




/* ----------------------------------------------------------------------- */
/* Sequence: MODEM_AUDIO_SPC_DATA_UNI_GSM_FR                               */
/* ----------------------------------------------------------------------- */

typedef struct
    {
    /* reserved for future use. Field should be set to zero in this version
       of the specification.
    */
    uint16  reserved;
    /* xxxxxxxxxx------  Filler
       ----------x-----  Reserved1
       -----------x----  SP
       ------------xx--  SID
       --------------x-  TAF
       ---------------x  BFI
    */
    uint16  header;
    /* Start of audio data, length of data is variable */
    } MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_STR;


typedef MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_STR MODEM_AUDIO_HEADER;

#define SIZE_MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_STR   (2*sizeof(uint16))

/* Merge mask constants */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_reserved1_OFFSET             0x5u  /*        5 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_reserved1_WIDTH              0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_reserved1_MASK              0x20u  /*       32 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sp_OFFSET                    0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sp_WIDTH                     0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sp_MASK                     0x10u  /*       16 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sid_OFFSET                   0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sid_WIDTH                    0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sid_MASK                     0xCu  /*       12 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_taf_OFFSET                   0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_taf_WIDTH                    0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_taf_MASK                     0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_bfi_OFFSET                   0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_bfi_WIDTH                    0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_bfi_MASK                     0x1u  /*        1 */


#define MODEM_AUDIO_FR_RX_FLAG_BFI                                  0x0004
#define MODEM_AUDIO_FR_RX_FLAG_TAF                                  0x0002
#define MODEM_AUDIO_FR_RX_FLAG_SID                                  0x0001

/* ----------------------------------------------------------------------- */
/* Sequence: MODEM_AUDIO_SPC_DATA_UNI_GSM_HR                               */
/* ----------------------------------------------------------------------- */

typedef struct
    {
    /* reserved for future use. Field should be set to zero in this version
       of the specification.
    */
    uint16  reserved;
    /* xxxxxxxxx-------  Filler
       ---------x------  Reserved1
       ----------x-----  SP
       -----------xx---  SID
       -------------x--  TAF
       --------------x-  UFI
       ---------------x  BFI
    */
    uint16  header;
    /* Start of audio data, length of data is variable */
    } MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_STR;

#define SIZE_MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_STR   (2*sizeof(uint16))

/* Merge mask constants */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_reserved1_OFFSET             0x6u  /*        6 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_reserved1_WIDTH              0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_reserved1_MASK              0x40u  /*       64 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sp_OFFSET                    0x5u  /*        5 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sp_WIDTH                     0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sp_MASK                     0x20u  /*       32 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sid_OFFSET                   0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sid_WIDTH                    0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sid_MASK                    0x18u  /*       24 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_taf_OFFSET                   0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_taf_WIDTH                    0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_taf_MASK                     0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_ufi_OFFSET                   0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_ufi_WIDTH                    0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_ufi_MASK                     0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_bfi_OFFSET                   0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_bfi_WIDTH                    0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_bfi_MASK                     0x1u  /*        1 */


#define MODEM_AUDIO_HR_FRAME_TX_SPEECH                               2
#define MODEM_AUDIO_HR_FRAME_TX_VAD                                  1
#define MODEM_AUDIO_HR_FRAME_TX_TAF                                  4

#define MODEM_AUDIO_HR_FRAME_RX_TYPE_MASK                            0x70
#define MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT                           4
#define MODEM_AUDIO_HR_FRAME_RX_SPEECH                               0
#define MODEM_AUDIO_HR_FRAME_RX_SID                                  4
#define MODEM_AUDIO_HR_FRAME_RX_TAF                                  8
#define MODEM_AUDIO_HR_FRAME_RX_UFI                                  2
#define MODEM_AUDIO_HR_FRAME_RX_BFI                                  1
#define MODEM_AUDIO_HR_FRAME_RX_NO_DATA                              7
/* ----------------------------------------------------------------------- */
/* Sequence: MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR                              */
/* ----------------------------------------------------------------------- */

typedef struct
    {
    /* reserved for future use. Field should be set to zero in this version
       of the specification.
    */
    uint16  reserved;
    /* xxxxxxxxxxx-----  Filler
       -----------x----  SP
       ------------xx--  SID
       --------------x-  TAF
       ---------------x  BFI
    */
    uint16  header;
    /* Start of audio data, length of data is variable */
    } MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_STR;

#define SIZE_MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_STR   (2*sizeof(uint16))

/* Merge mask constants */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sp_OFFSET                   0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sp_WIDTH                    0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sp_MASK                    0x10u  /*       16 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sid_OFFSET                  0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sid_WIDTH                   0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sid_MASK                    0xCu  /*       12 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_taf_OFFSET                  0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_taf_WIDTH                   0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_taf_MASK                    0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_bfi_OFFSET                  0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_bfi_WIDTH                   0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_bfi_MASK                    0x1u  /*        1 */

#define MODEM_AUDIO_EFR_RX_FLAG_BFI   0x4
#define MODEM_AUDIO_EFR_RX_FLAG_TAF   0x2
#define MODEM_AUDIO_EFR_RX_FLAG_SID   0x1

/* ----------------------------------------------------------------------- */
/* Sequence: MODEM_AUDIO_SPC_DATA_UNI_AMR                                  */
/* ----------------------------------------------------------------------- */

typedef struct
    {
    /* reserved for future use. Field should be set to zero in this version
       of the specification.
    */
    uint16  reserved;
    /* xxx-------------  RX Type
       ---xxx----------  SFN
       ------xx--------  TX Type
       --------x-------  Padding1
       ---------xxxx---  Frame Type
       -------------x--  Quality
       --------------x-  Padding2
       ---------------x  Padding3
    */
    uint16  header;
    /* Start of audio data, length of data is variable */
    } MODEM_AUDIO_SPC_DATA_UNI_AMR_STR;

#define SIZE_MODEM_AUDIO_SPC_DATA_UNI_AMR_STR   (2*sizeof(uint16))

/* Merge mask constants */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_rx_type_OFFSET                  0xDu  /*       13 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_rx_type_WIDTH                   0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_rx_type_MASK                 0xE000u  /*    57344 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_sfn_OFFSET                      0xAu  /*       10 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_sfn_WIDTH                       0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_sfn_MASK                     0x1C00u  /*     7168 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_OFFSET                  0x8u  /*        8 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_WIDTH                   0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_MASK                  0x300u  /*      768 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding1_OFFSET                 0x7u  /*        7 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding1_WIDTH                  0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding1_MASK                  0x80u  /*      128 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET               0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_WIDTH                0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_MASK                0x78u  /*      120 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_quality_OFFSET                  0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_quality_WIDTH                   0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_quality_MASK                    0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding2_OFFSET                 0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding2_WIDTH                  0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding2_MASK                   0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding3_OFFSET                 0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding3_WIDTH                  0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_padding3_MASK                   0x1u  /*        1 */

/* Value constants */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_sfn_MIN                         0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_AMR_sfn_MAX                         0x7u  /*        7 */


/* ----------------------------------------------------------------------- */
/* Constant Table: MODEM_AUDIO_SPC_AMR_FRAME_TYPE                          */
/* ----------------------------------------------------------------------- */
/* AMR frame types. Only one type at a time can be used in the frametype
   field of the speech codec data.
*/
#define MODEM_AUDIO_AMR_475                      0x00  /* AMR 4.75 frame */
#define MODEM_AUDIO_AMR_515                      0x01  /* AMR 5.15 frame */
#define MODEM_AUDIO_AMR_590                      0x02  /* AMR 5.90 frame */
#define MODEM_AUDIO_AMR_670                      0x03  /* AMR 6.70 frame */
#define MODEM_AUDIO_AMR_740                      0x04  /* AMR 7.40 frame */
#define MODEM_AUDIO_AMR_795                      0x05  /* AMR 7.95 frame */
#define MODEM_AUDIO_AMR_102                      0x06  /* AMR 10.2 frame */
#define MODEM_AUDIO_AMR_122                      0x07  /* AMR 12.2 frame */
/* AMR comfort noise frame */
#define MODEM_AUDIO_AMR_CN                       0x08
#define MODEM_AUDIO_NO_RX_TX                     0x0F  /* No data */

#define MODEM_AUDIO_TX_TYPE_SPC                  0x0
#define MODEM_AUDIO_TX_TYPE_SID                  0x1
#define MODEM_AUDIO_TX_TYPE_SID_UPDATE           0x2
#define MODEM_AUDIO_TX_TYPE_NO_DATA              0x3


#define MODEM_AUDIO_RX_SPEECH_GOOD               0x0
#define MODEM_AUDIO_RX_SPEECH_DEGRADED           0x1
#define MODEM_AUDIO_RX_ONSET                     0x2
#define MODEM_AUDIO_RX_SPEECH_BAD                0x3
#define MODEM_AUDIO_RX_SID_FIRST                 0x4
#define MODEM_AUDIO_RX_SID_UPDATE                0x5
#define MODEM_AUDIO_RX_SID_BAD                   0x6
#define MODEM_AUDIO_RX_NO_DATA                   0x7




/* Number of bits in the AMR codec core frame. i.e. the data component */
/* 95, 103, 118, 134, 148, 159, 204, 244, 39, 43, 38, 37 */
#define AMR_CORE_FRM_NUM_BITS_AMR_475        95
#define AMR_CORE_FRM_NUM_BITS_AMR_515       103
#define AMR_CORE_FRM_NUM_BITS_AMR_590       118
#define AMR_CORE_FRM_NUM_BITS_AMR_670       134
#define AMR_CORE_FRM_NUM_BITS_AMR_740       148
#define AMR_CORE_FRM_NUM_BITS_AMR_795       159
#define AMR_CORE_FRM_NUM_BITS_AMR_102       204
#define AMR_CORE_FRM_NUM_BITS_AMR_122       244
#define AMR_CORE_FRM_NUM_BITS_AMR_SID        39
#define AMR_CORE_FRM_NUM_BITS_GSM_EFR_SID    43
#define AMR_CORE_FRM_NUM_BITS_TDMA_EFR_SID   38
#define AMR_CORE_FRM_NUM_BITS_PDC_EFR_SID    37
#define AMR_CORE_FRM_NUM_BITS_RESERVED12      0
#define AMR_CORE_FRM_NUM_BITS_RESERVED13      0
#define AMR_CORE_FRM_NUM_BITS_RESERVED14      0
#define AMR_CORE_FRM_NUM_BITS_NO_DATA         0


/* ----------------------------------------------------------------------- */
/* Sequence: MODEM_AUDIO_SPC_DATA_UNI_WB_AMR                               */
/* ----------------------------------------------------------------------- */

typedef struct
    {
    /* reserved for future use. Field should be set to zero in this version
       of the specification.
    */
    uint16  reserved;
    /* xxx-------------  RX Type
       ---xxx----------  SFN
       ------xx--------  TX Type
       --------x-------  Padding1
       ---------xxxx---  Frame Type
       -------------x--  Quality
       --------------x-  Padding2
       ---------------x  Padding3
    */
    uint16  header;
    /* Start of audio data, length of data is variable */
    } MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_STR;

#define SIZE_MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_STR   (2*sizeof(uint16))

/* Merge mask constants */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_rx_type_OFFSET               0xDu  /*       13 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_rx_type_WIDTH                0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_rx_type_MASK              0xE000u  /*    57344 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_sfn_OFFSET                   0xAu  /*       10 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_sfn_WIDTH                    0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_sfn_MASK                  0x1C00u  /*     7168 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_tx_type_OFFSET               0x8u  /*        8 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_tx_type_WIDTH                0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_tx_type_MASK               0x300u  /*      768 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding1_OFFSET              0x7u  /*        7 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding1_WIDTH               0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding1_MASK               0x80u  /*      128 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET            0x3u  /*        3 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_WIDTH             0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_MASK             0x78u  /*      120 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_quality_OFFSET               0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_quality_WIDTH                0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_quality_MASK                 0x4u  /*        4 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding2_OFFSET              0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding2_WIDTH               0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding2_MASK                0x2u  /*        2 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding3_OFFSET              0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding3_WIDTH               0x1u  /*        1 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_padding3_MASK                0x1u  /*        1 */

/* Value constants */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_sfn_MIN                      0x0u  /*        0 */
#define MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_sfn_MAX                      0x7u  /*        7 */


/* ----------------------------------------------------------------------- */
/* Constant Table: MODEM_AUDIO_SPC_WB_AMR_FRAME_TYPE                       */
/* ----------------------------------------------------------------------- */
/* AMR-WB frame types. Only one type at a time can be used in the frametype
   field of the speech codec data.
*/
/* AMR-WB 6.60 frame */
#define MODEM_AUDIO_WB_AMR_660                   0x00
/* AMR-WB 8.85 frame */
#define MODEM_AUDIO_WB_AMR_885                   0x01
/* AMR-WB 12.65 frame */
#define MODEM_AUDIO_WB_AMR_1265                  0x02
/* AMR-WB 14.25 frame */
#define MODEM_AUDIO_WB_AMR_1425                  0x03
/* AMR-WB 15.85 frame */
#define MODEM_AUDIO_WB_AMR_1585                  0x04
/* AMR-WB 18.25 frame */
#define MODEM_AUDIO_WB_AMR_1825                  0x05
/* AMR-WB 19.85 frame */
#define MODEM_AUDIO_WB_AMR_1985                  0x06
/* AMR-WB 23.05 frame */
#define MODEM_AUDIO_WB_AMR_2305                  0x07
/* AMR-WB 23.85 frame */
#define MODEM_AUDIO_WB_AMR_2385                  0x08
/* AMR-WB SID (comfort noise frame) */
#define MODEM_AUDIO_WB_AMR_SID                   0x09
/* AMR-WB speech lost */
#define MODEM_AUDIO_WB_AMR_SPEECH_LOST           0x0E
/* No data (No Rx/Tx) */
#define MODEM_AUDIO_WB_AMR_NO_RX_TX              0x0F


#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_660    132
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_885    177
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_1265   253
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_1425   285
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_1585   317
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_1825   365
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_1985   397
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_2305   461
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_2385   477
#define AMRWB_CORE_FRM_NUM_BITS_AMRWB_SID     40

#define AMRWB_CORE_FRM_NUM_BITS_SPEECH_LOST  0
#define AMRWB_CORE_FRM_NUM_BITS_NO_DATA      0

/* ----------------------------------------------------------------------- */
/* Constant Table: MODEM_AUDIO_SPC                                         */
/* ----------------------------------------------------------------------- */

/* No speech codec in use. */
#define SPC_NO                                   0x0000
/* GSM full rate speech codec (unified format) used. */
#define SPC_UNI_GSM_FR                           0x0012 // TODO: Not available
#define SPC_UNI_GSM_FR_LEN                       42
/* GSM Half rate speech codec (unified format) used. */
#define SPC_UNI_GSM_HR                           0x0013 // TODO: Not available
#define SPC_UNI_GSM_HR_LEN                       26
/* GSM enhanced full rate speech codec (unified format) used. */
#define SPC_UNI_GSM_EFR                          0x0014
#define SPC_UNI_GSM_EFR_LEN                      42
/* NB AMR adaptive multi rate speech codec (unified format) used. */
#define SPC_UNI_AMR                              0x0015
#define SPC_UNI_AMR_LEN                          40
/* WB AMR adaptive multirate speech codec (unified format) used. */
#define SPC_UNI_WB_AMR                           0x0016
#define SPC_UNI_WB_AMR_LEN                       68


/* Phonet devices */
#define PN_DEV_MODEM_HOST_1             0x5C /* [D] 92  Device for modem host 1 */
#define PN_DEV_MODEM                    0x60 /* [D] 96  Modem device number 1   */
#define PN_DEV_MODEM_L1_CPU             0x64 /* [D] 100 Modem L1 CPU            */
#define PN_DEV_MODEM_2                  0x68 /* [D] 104 Modem device number 2   */
#define PN_DEV_SOS                      0x6C /* [D] 108 Device for SOS media module (autogen needs this) */

#define PN_DEV_REMOTE_HOST              0x74 /* [D] 116, 'remote' host in host/host communication */
#define PN_DEV_DSP_AUX                  0x78 /* [D] 120, Auxiliary DSP processor (autogen needs this) */
#define PN_DEV_DSP_MAIN                 0x7C /* [D] 124, Main DSP processor      */

#define PN_DEV_HOST                     0x00 /* [D] 0, Host processor (router) */




/* === DSP resources === */
#define PN_DSP_CORE                     230 /* 0xE6  [DR] */
#define PN_DSP_VIDEO                    231 /* 0xE7  [DR] */
#define PN_DSP_VOICE                    232 /* 0xE8  [DR] */
#define PN_DSP_AUDIO                    243 /* 0xF3  [DR] */
#define PN_NO_ROUTING                   255 /* 0xFF */


/* === Objects === */
#define PN_OBJ_ROUTING_REQ              0x00 /* Resource-based routing                      */
#define PN_OBJ_VOICE_ENGINE             0x10 /* Voice Engine                                */

#define SPC_TIMING_OK                   0x0000
#define SPC_TIMING_FAIL                 0x0001

/* ----------------------------------------------------------------------- */
/* Message ID's                                                            */
/* ----------------------------------------------------------------------- */

#define AUDIO_CTRL_READY_REQ                     0x21
#define AUDIO_CTRL_RELEASE_NTF                   0x22
#define AUDIO_CTRL_SYSTEM_ERROR_NTF              0x23
#define AUDIO_CTRL_READY_RESP                    0x31

#define PN_MEDIA_HOST_SSI                        0x26 /* [M] Media Host SSI   */

/* constant table MODEM_AUDIO_SPC_USE */

/* Speech codec not used. This symbol can be used when disabling previously
   activated codec.
*/
#define SPC_USE_NO                               0x0000
/* Speech codec used in WGModem circuit switched GSM speech call,
   synchronisation time provided in uS
*/
#define SPC_USE_GSM_MODEM                        0x0008
/* Speech codec used in WGModem circuit switched WCDMA speech call,
   synchronisation time provided in uS
*/
#define SPC_USE_WCDMA_MODEM                      0x0009
/* Speech codec used in LTE Modem packet VoLTE speech call */
#define SPC_USE_LTE_MODEM                        0x000A
/* Speech codec used in TD-SCDMA speech call */
#define SPC_USE_TDSCDMA_MODEM                    0x000B


typedef struct
{
      uint8   transactionId;
      bits16  dataFormatRequest;
      bits16  encodeConfig;
      bits16  codecUse;
      bits16  rxSeqCount;
      bits16  hapiCodec;
      bits16  hapiVad;
      uint8   codecUpdated;
      uint8   modemSenderDev;
      uint8   modemReceiverDev;
      uint8   modemSenderObj;
      uint8   modemReceiverObj;
      uint8   sidStart;
} MODEM_STREAM_INFO;

#define MODEM_MAX_AUDIO_STREAM      1       // Number of media streams supported 
#define MHI_L2MUX_HEAD              4       // Extra space needed for L2MUX PCIe driver

extern volatile MODEM_STREAM_INFO gCSMedia[MODEM_MAX_AUDIO_STREAM];

#define rdclcnt(a) __asm__ __volatile__ ("mrc p15, 0, %0, c9, c13, 0": "=r"(a))
#define getcpuid(a) __asm__ __volatile__ ("mrc p15, 0, %0, c0, c0, 5": "=r"(a))


#endif /* __VRGENDPTMODEM_H__ */
