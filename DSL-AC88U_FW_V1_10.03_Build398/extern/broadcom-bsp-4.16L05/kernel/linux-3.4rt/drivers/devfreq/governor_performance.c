/*
 *  linux/drivers/devfreq/governor_performance.c
 *
 *  Copyright (C) 2011 Samsung Electronics
 *	MyungJoo Ham <myungjoo.ham@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/devfreq.h>
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
#include "governor.h"
#endif

static int devfreq_performance_func(struct devfreq *df,
				    unsigned long *freq)
{
	/*
	 * target callback should be able to get floor value as
	 * said in devfreq.h
	 */
	if (!df->max_freq)
		*freq = UINT_MAX;
	else
		*freq = df->max_freq;
	return 0;
}

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
static int performance_init(struct devfreq *devfreq)
{
	return update_devfreq(devfreq);
}

#endif
const struct devfreq_governor devfreq_performance = {
	.name = "performance",
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	.init = performance_init,
#endif
	.get_target_freq = devfreq_performance_func,
	.no_central_polling = true,
};
