#ifndef _MSPIDEF_H_
#define _MSPIDEF_H_

#include "bcmSpiRes.h"

#ifdef CONFIG_BCM96816
#define MSPI_CLK_FREQUENCY    781000
#else
#define MSPI_CLK_FREQUENCY    6250000
#endif


#if defined(CONFIG_BCM96328) || defined(CONFIG_BCM963268) || defined(CONFIG_BCM96828) || defined(CONFIG_BCM96818) || defined(CONFIG_BCM96838) || defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148) || defined(CONFIG_BCM963381) || defined(CONFIG_BCM96848)
#define MSPI_BUS_ID         HS_SPI_BUS_NUM
#elif defined(CONFIG_BCM96816)
#define MSPI_BUS_ID         spi_bus_num_special
#else
#define MSPI_BUS_ID         LEG_SPI_BUS_NUM
#endif

#endif

