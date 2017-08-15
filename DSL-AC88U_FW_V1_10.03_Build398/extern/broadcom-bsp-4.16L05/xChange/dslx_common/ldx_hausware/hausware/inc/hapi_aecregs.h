/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*           Broadcom
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  hapi_aecregs.h
*
*  PURPOSE:
*
*     This file implements the AEC parameters for the 91101 reference phone
* 
*  NOTES:
*
****************************************************************************/

/* 
 * This file can either be editted in place (in hausware/inc), or placed
 * in the application/inc directory (which will override the version in
 * hausware/inc) or read in to the host code and passed down via setregs.
 * The latter method is preferred, in which case DEF_REGS should be replaced
 * by <PHONEID>_REGS.
 */
#define AEC_DEF_REGS \
{  HAPI_AEC_DEFAULT_OPTIONS,   /* [0]: options */                                                             \
   HAPI_AEC_DEFAULT_EVENTMASK, /* [1]: event mask */                                                          \
   HAPI_AEC_DEFAULT_VOLUME,    /* [2]: volume */                                                              \
   HAPI_AEC_DEFAULT_GAIN,      /* [3]: gain */                                                                \
   {6*Q_DB,0*Q_DB,-15*Q_DB,0*Q_DB,32*Q_DB},  /* [4-8]: vol compressor */                                      \
   {0*Q_DB,0*Q_DB,-15*Q_DB,0*Q_DB,32*Q_DB},  /* [9-13]: gain compressor noise suppression off */              \
   {-10*Q_DB,3*Q_DB},           /* [14,15]: erl range */                                                      \
   {0,20*Q_DB},                 /* [16,17]: erle range */                                                     \
   {3*Q_DB,12*Q_DB},            /* [18,19]: suppressor limits, egress */                                      \
   {6*Q_DB,20*Q_DB},            /* [20,21]: suppressor limits, ingress */                                     \
   0,               /* [22]: receive bias */                                                                  \
   6*Q_DB,          /* [23]: high level limit */                                                              \
   6*Q_DB,         /* [24]: whitened non-linearity threshold */                                               \
   6*Q_DB,          /* [25]: whitened non-linearity threshold (high level) */                                 \
   6*Q_DB,          /* [26]: nlp high level thresh */                                                         \
   -40*Q_DB,        /* [27]: minimum Sin level before declaring Sin active */                                 \
   -36*Q_DB,        /* [28]: minimum Rout level before declaring Rout active */                               \
   -30*Q_DB,        /* [29]: minimum Sin level before declaring Sin very active (long hangover) */            \
   -24*Q_DB,        /* [30]: minimum Rout level before declaring Rout very active (long hangover) */          \
   {-1,-1,-1,-1},   /* [31-34]: highpass parameters on receive path */                                        \
   {5,5},           /* [35,36]: hang over counters for near end detection (NLP)                               \
                       (short,long in 5 msec ticks) */                                                        \
   {5,50},          /* [37,38]: hang over counters for near end detection (adapt)                             \
                       (short,long in 5 msec ticks) */                                                        \
   {10,20},         /* [39,40]: hang over counters for far end detection (short,long in 5 msec ticks) */      \
   {0*Q_DB,0*Q_DB,10*Q_DB},           /* [41-43]:  NLP low level thresholds */                                \
   {20*Q_DB,12*Q_DB},                 /* [44,45]:  NLP thresh on ERLE (breakin and singletalk) */             \
   {20*Q_DB,40*Q_DB},                 /* [46,47]:  NLP thresh on Acom (breakin and singletalk) */             \
   {-10*Q_DB,-10*Q_DB},               /* [48,49]:  NLP thresh on ERLE (in doubletalk)  */                     \
   {-40*Q_DB,20*Q_DB},                /* [50,51]:  NLP thresh on Acom (in doubletalk)  */                     \
   /* [52-63]: default decay profiles */                                                                      \
   {36*Q_DB, 60*Q_DB, 5*Q_DB, 36*Q_DB, 60*Q_DB, 5*Q_DB, 36*Q_DB, 60*Q_DB, 5*Q_DB, 36*Q_DB, 60*Q_DB, 5*Q_DB},  \
   /* [64-71] subband ERL */                                                                                  \
   {-10*Q_DB, -10*Q_DB, -10*Q_DB, -10*Q_DB, -10*Q_DB, -10*Q_DB, -10*Q_DB, -10*Q_DB},                          \
   /* [72-79] subband high level thresholds */                                                                \
   {10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB},                                  \
   /* [80-87] subband non-linearity thresholds */                                                             \
   {10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB},                                  \
   /* [88-95] subband non-linearity thresholds (upper) */                                                     \
   {10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB, 10*Q_DB},                                  \
   /* [96-103] subband non-linear thresholds high level */                                                    \
   {4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB},                                          \
   /* [104-111] subband non-linearity thesholds high level upper */                                           \
   {4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB, 4*Q_DB},                                          \
   {0*Q_DB, 0*Q_DB, 0*Q_DB},         /* [112-114] erl offset */                                             \
   {-2*Q_DB, -4*Q_DB, -4*Q_DB, -5*Q_DB, -7*Q_DB}, /* [115-119]: erlMv  */                                     \
   {27000,28000,29000,30000,31000},    /* [120-124] mv thresh */                                              \
   {13*Q_DB,10*Q_DB},    /* [125-126]: converged, diverged thresholds */                                      \
   5000,                 /* [127]: state interval (ms) */                                                     \
   -1,                   /* [128]: tone index */                                                              \
   32767                 /* [129]: debug parameter */                                                         \
}

