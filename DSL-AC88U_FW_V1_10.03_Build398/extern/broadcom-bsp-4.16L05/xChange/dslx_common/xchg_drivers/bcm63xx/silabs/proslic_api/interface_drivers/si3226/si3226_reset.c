#include <bcmChip.h>
#include <xdrvLog.h>
#include "si3226_reset.h"

#include "board.h"

static XDRV_UINT16 resetPin; 

void Si3226SetResetGpio( XDRV_UINT16 resetGpioPin )
{
   resetPin = resetGpioPin;
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"Setting SLIC reset GPIO to: %d", resetPin ));
}

void Si3226Reset( XDRV_UINT16 status )
{
	if (status)
	{
		kerSysSetGpio( resetPin, kGpioActive );
		XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"SLIC in reset using GPIO: %d", resetPin ));
	}
	else
	{
		kerSysSetGpio( resetPin, kGpioInactive );
		XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"SLIC out of reset using GPIO: %d", resetPin ));
	}
}

