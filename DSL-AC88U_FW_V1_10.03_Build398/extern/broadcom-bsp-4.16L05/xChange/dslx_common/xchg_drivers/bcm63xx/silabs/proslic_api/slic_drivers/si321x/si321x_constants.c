/*
Copyright (c) 2008 Silicon Laboratories, Inc.
2008-04-03 17:59:58 */
/*ProSLIC API Tool Rev0.31 Alpha*/


#include "proslic.h"
#include "si321x.h"

Si321x_General_Cfg Si321x_General_Configuration = {
	0xff,
		0xff,
		0xff
};

#define AUDIOGAIN_0DB	0

Si321x_audioGain_Cfg Si321x_AudioGain_Presets [] = {
	{AUDIOGAIN_0DB,0x4000}
};
Si321x_PulseMeter_Cfg Si321x_PulseMeter_Presets [] = {
	{0,0,0,0,0}
};


/*Ring Cadences- To be confirmed. Set 0x18 value to 0x0 to disable cadence.*/

Si321x_Ring_Cfg Si321x_Ring_Presets[] ={

          { 0x194, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Australia*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Austria*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Belgium*/
          { 0x1A7, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Brazil*/
          { 0x1A7, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Chile*/
          { 0x190, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*China*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Czech*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Denmark*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*ETSI*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Finland*/
          { 0x2D2, 0x0, 0x0, 0x79bc, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0x5, 0x6a8 }, /*France*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Germany*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Hungary*/
          { 0x152, 0x0, 0x0, 0x7Efd, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xb, 0x320 }, /*India -- DUMMY*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Italy*/
          { 0x152, 0x0, 0x0, 0x7EFD, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xB, 0x320 }, /*Japan*/
          { 0x152, 0x0, 0x0, 0x7Efd, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xb, 0x320 }, /*Mexico (Copy of NA) */
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Netherlands*/
          { 0x17F, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*New Zealand*/
          { 0x152, 0x0, 0x0, 0x7Efd, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xb, 0x320 }, /*North America*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Spain*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*sweden*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Norway*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Switzerland*/
          { 0x152, 0x0, 0x0, 0x7Efd, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xB, 0x320 }, /*TR57*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*UK*/
          { 0x1A7, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Taiwan*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Poland -- DUMMY*/  
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Ireland -- DUMMY*/ 
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Slovakia -- DUMMY*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Romania -- DUMMY*/ 
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }, /*Slovenia -- DUMMY*/
          { 0x167, 0x0, 0x0, 0x7E6C, 0x80, 0x3E, 0x0, 0x7D, 0x18, 0xA, 0x360 }  /*United Arab Emirates (Copy of UK) */

};

Si321x_DCfeed_Cfg Si321x_Dcfeed_Presets[] = {

    /* inputs: ilim=20.000, voc=48.000, vcm=3.000 */
    { 0x0, 0x20, 0x2 },
    /* inputs: ilim=41.000, voc=48.000, vcm=6.000 */
    { 0x7, 0x20, 0x4 }
};

Si321x_Zsynth_Cfg Si321x_Zsynth_Presets [] ={
      { 0x0, 0xD  }, /*Australia*/
      { 0x0, 0xC  }, /*Austria*/
      { 0x0, 0xC  }, /*Belgium*/
      { 0x0, 0x9  }, /*Brazil*/
      { 0x0, 0x8  }, /*Chile*/
      { 0x0, 0xF  }, /*CHina*/
      { 0x0, 0xC  }, /*Czech*/
      { 0x0, 0xC  }, /*Denmark*/
      { 0x0, 0xC  }, /*ETSI*/
      { 0x0, 0xC  }, /*Finland*/
      { 0x0, 0xC  }, /*France*/
      { 0x0, 0xC  }, /*Germany*/
      { 0x0, 0xC  }, /*Hungary*/
      { 0x0, 0x8  }, /*India -- DUMMY*/
      { 0x0, 0xC  }, /*Italy*/
      { 0x0, 0xA  }, /*Japan*/
      { 0x0, 0x8  }, /*Mexico (Copy of NA) */
      { 0x0, 0xC  }, /*Netherlands*/
      { 0x0, 0xD  }, /*New Zealand*/
      { 0x0, 0x8  }, /*North America*/
      { 0x0, 0xC  }, /*Spain*/
      { 0x0, 0xC  }, /*Sweden*/
      { 0x0, 0xC  }, /*Norway*/
      { 0x0, 0xC  }, /*switzerland*/
      { 0x0, 0xB  }, /*TR57*/
      { 0x0, 0xC  }, /*UK*/
      { 0x0, 0x8  }, /*Taiwan -- DUMMY*/
      { 0x0, 0xC  }, /*Poland -- DUMMY*/    
      { 0x0, 0xC  }, /*Ireland -- DUMMY*/    
      { 0x0, 0xC  }, /*Slovakia -- DUMMY*/     
      { 0x0, 0xC  }, /*Romania -- DUMMY*/    
      { 0x0, 0xC  }, /*Slovenia -- DUMMY*/ 
      { 0x0, 0xC  }  /*United Arab Emirates (Copy of UK) */
                
};

Si321x_FSK_Cfg Si321x_FSK_Presets[] ={

    /* inputs: mark freq=1200.000, space freq2200.000, amp=0.220, baud=1200.000 */
    { 0x1119L, 0x3BE1L, 0x1E0L, 0x100L, 0x35B0L, 0x3CE0L }
};

/*
** Tone Generator configuration
*/
#define OSC_TOPHONE 0x2
#define OSC_350HZ_MINUS18DBM_8KHZ	0x7b30,0x63,0
#define OSC_350HZ_MINUS18DBM_16KHZ	0x7ed0,0x31,0
#define OSC_NOTIME				0,0
#define OSC_0_5SEC				0xa0,0xf
#define OSC_0_3SEC				0x60,0x9
#define OSC_0_2SEC				0x40,0x6
#define OSC_4SEC				0x0,0x7d
#define	OSC_2SEC				0x80,0x3e
#define OSC_440HZ_MINUS18DBM_8KHZ	0x7870,0x7d,0 
#define OSC_440HZ_MINUS18DBM_16KHZ	0x7e20,0x3d,0 
#define OSC_480HZ_MINUS18DBM_8KHZ	0x7700,0x89,0
#define OSC_480HZ_MINUS18DBM_16KHZ	0x7dc0,0x43,0
#define OSC_620HZ_MINUS18DBM_8KHZ	0x7120,0xb2,0
#define OSC_620HZ_MINUS18DBM_16KHZ	0x7c40,0x57,0

/*Si3210 Tone generator (8kHz) */
Si321x_Tone_Cfg Si321x_Tone_Presets [] = {
{{OSC_350HZ_MINUS18DBM_8KHZ,OSC_NOTIME,OSC_NOTIME},{OSC_440HZ_MINUS18DBM_8KHZ,OSC_NOTIME,OSC_NOTIME},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_0_5SEC,OSC_0_5SEC},{OSC_620HZ_MINUS18DBM_8KHZ,OSC_0_5SEC,OSC_0_5SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_0_2SEC,OSC_0_3SEC},{OSC_620HZ_MINUS18DBM_8KHZ,OSC_0_2SEC,OSC_0_3SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_2SEC,OSC_4SEC},{OSC_440HZ_MINUS18DBM_8KHZ,OSC_2SEC,OSC_4SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_8KHZ,OSC_0_3SEC,OSC_0_2SEC},{OSC_620HZ_MINUS18DBM_8KHZ,OSC_0_3SEC,OSC_0_2SEC},OSC_TOPHONE,OSC_TOPHONE}
};

Si321x_Tone_Cfg Si3215_Tone_Presets [] = {
{{OSC_350HZ_MINUS18DBM_16KHZ,OSC_NOTIME,OSC_NOTIME},{OSC_440HZ_MINUS18DBM_16KHZ,OSC_NOTIME,OSC_NOTIME},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_0_5SEC,OSC_0_5SEC},{OSC_620HZ_MINUS18DBM_16KHZ,OSC_0_5SEC,OSC_0_5SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_0_2SEC,OSC_0_3SEC},{OSC_620HZ_MINUS18DBM_16KHZ,OSC_0_2SEC,OSC_0_3SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_2SEC,OSC_4SEC},{OSC_440HZ_MINUS18DBM_16KHZ,OSC_2SEC,OSC_4SEC},OSC_TOPHONE,OSC_TOPHONE},
{{OSC_480HZ_MINUS18DBM_16KHZ,OSC_0_3SEC,OSC_0_2SEC},{OSC_620HZ_MINUS18DBM_16KHZ,OSC_0_3SEC,OSC_0_2SEC},OSC_TOPHONE,OSC_TOPHONE}
};


Si321x_PCM_Cfg Si321x_Pcm_Presets[] ={

    /* inputs:  a-law narrowband positive  */
    { 0x0, 0x0, 0x0 },
    /* inputs:  16 bit linear narrowband negative  */
    { 0x7, 0x0, 0x1 },
    /* inputs:  a-law narrowband negative  */
    { 0x0, 0x0, 0x1 },
    /* inputs:  u-law narrowband negative  */
    { 0x1, 0x0, 0x1 },
    /* inputs:  16 bit linear wideband negative  */
    { 0x7, 0x1, 0x1 }
};
