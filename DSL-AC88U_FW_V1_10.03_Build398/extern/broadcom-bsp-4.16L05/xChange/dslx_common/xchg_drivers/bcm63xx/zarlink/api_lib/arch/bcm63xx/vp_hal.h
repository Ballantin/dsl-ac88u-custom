/* vp_hal.h
 *
 * This file defines the interface between the VoicePath API and the Hardware
 * Abstraction Layer (HAL).  The types and functions declared in this file are
 * platform-dependent.  The functions are defined in ve_hal.c.  Porting the
 * VoicePath API to a new host processor consists of modifying the typedefs
 * in this file, setting the HBI_PINCONFIG define below, and modifying the
 * function implementations in vp_hal.c.
 *
 * Copyright (c) 2011, Microsemi Corporation
 */

#ifndef _VP_HAL_H
#define _VP_HAL_H

#include "vp_api_types.h"
#include "hbi_hal.h"
#include "mpi_hal.h"
#include "sys_service.h"

/* The following definitions have been added to redirect the output of
 * debug printf statements from the application and the API */
#define term_printf printf

#define debug_printf printf

uint8p SpiRawCmd(
	uint8p data,
	uint16 length);

#endif /* _VP_HAL_H */





