/*
 * drivers/base/power/domain_governor.c - Governors for device PM domains.
 *
 * Copyright (C) 2011 Rafael J. Wysocki <rjw@sisk.pl>, Renesas Electronics Corp.
 *
 * This file is released under the GPLv2.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/pm_domain.h>
#include <linux/pm_qos.h>
#include <linux/hrtimer.h>

#ifdef CONFIG_PM_RUNTIME

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
static int dev_update_qos_constraint(struct device *dev, void *data)
{
	s64 *constraint_ns_p = data;
	s32 constraint_ns = -1;

	if (dev->power.subsys_data && dev->power.subsys_data->domain_data)
		constraint_ns = dev_gpd_data(dev)->td.effective_constraint_ns;

	if (constraint_ns < 0) {
		constraint_ns = dev_pm_qos_read_value(dev);
		constraint_ns *= NSEC_PER_USEC;
	}
	if (constraint_ns == 0)
		return 0;

	/*
	 * constraint_ns cannot be negative here, because the device has been
	 * suspended.
	 */
	if (constraint_ns < *constraint_ns_p || *constraint_ns_p == 0)
		*constraint_ns_p = constraint_ns;

	return 0;
}

#endif
/**
 * default_stop_ok - Default PM domain governor routine for stopping devices.
 * @dev: Device to check.
 */
bool default_stop_ok(struct device *dev)
{
	struct gpd_timing_data *td = &dev_gpd_data(dev)->td;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	unsigned long flags;
	s64 constraint_ns;
#endif

	dev_dbg(dev, "%s()\n", __func__);

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	if (dev->power.max_time_suspended_ns < 0 || td->break_even_ns == 0)
		return true;
#else
	spin_lock_irqsave(&dev->power.lock, flags);

	if (!td->constraint_changed) {
		bool ret = td->cached_stop_ok;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	return td->stop_latency_ns + td->start_latency_ns < td->break_even_ns
		&& td->break_even_ns < dev->power.max_time_suspended_ns;
#else
		spin_unlock_irqrestore(&dev->power.lock, flags);
		return ret;
	}
	td->constraint_changed = false;
	td->cached_stop_ok = false;
	td->effective_constraint_ns = -1;
	constraint_ns = __dev_pm_qos_read_value(dev);

	spin_unlock_irqrestore(&dev->power.lock, flags);

	if (constraint_ns < 0)
		return false;

	constraint_ns *= NSEC_PER_USEC;
	/*
	 * We can walk the children without any additional locking, because
	 * they all have been suspended at this point and their
	 * effective_constraint_ns fields won't be modified in parallel with us.
	 */
	if (!dev->power.ignore_children)
		device_for_each_child(dev, &constraint_ns,
				      dev_update_qos_constraint);

	if (constraint_ns > 0) {
		constraint_ns -= td->start_latency_ns;
		if (constraint_ns == 0)
			return false;
	}
	td->effective_constraint_ns = constraint_ns;
	td->cached_stop_ok = constraint_ns > td->stop_latency_ns ||
				constraint_ns == 0;
	/*
	 * The children have been suspended already, so we don't need to take
	 * their stop latencies into account here.
	 */
	return td->cached_stop_ok;
#endif
}

/**
 * default_power_down_ok - Default generic PM domain power off governor routine.
 * @pd: PM domain to check.
 *
 * This routine must be executed under the PM domain's lock.
 */
static bool default_power_down_ok(struct dev_pm_domain *pd)
{
	struct generic_pm_domain *genpd = pd_to_genpd(pd);
	struct gpd_link *link;
	struct pm_domain_data *pdd;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	s64 min_dev_off_time_ns;
#else
	s64 min_off_time_ns;
#endif
	s64 off_on_time_ns;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	ktime_t time_now = ktime_get();
#else

	if (genpd->max_off_time_changed) {
		struct gpd_link *link;

		/*
		 * We have to invalidate the cached results for the masters, so
		 * use the observation that default_power_down_ok() is not
		 * going to be called for any master until this instance
		 * returns.
		 */
		list_for_each_entry(link, &genpd->slave_links, slave_node)
			link->master->max_off_time_changed = true;

		genpd->max_off_time_changed = false;
		genpd->cached_power_down_ok = false;
		genpd->max_off_time_ns = -1;
	} else {
		return genpd->cached_power_down_ok;
	}
#endif

	off_on_time_ns = genpd->power_off_latency_ns +
				genpd->power_on_latency_ns;
	/*
	 * It doesn't make sense to remove power from the domain if saving
	 * the state of all devices in it and the power off/power on operations
	 * take too much time.
	 *
	 * All devices in this domain have been stopped already at this point.
	 */
	list_for_each_entry(pdd, &genpd->dev_list, list_node) {
		if (pdd->dev->driver)
			off_on_time_ns +=
				to_gpd_data(pdd)->td.save_state_latency_ns;
	}

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	min_off_time_ns = -1;
#endif
	/*
	 * Check if subdomains can be off for enough time.
	 *
	 * All subdomains have been powered off already at this point.
	 */
	list_for_each_entry(link, &genpd->master_links, master_node) {
		struct generic_pm_domain *sd = link->slave;
		s64 sd_max_off_ns = sd->max_off_time_ns;

		if (sd_max_off_ns < 0)
			continue;

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		sd_max_off_ns -= ktime_to_ns(ktime_sub(time_now,
						       sd->power_off_time));
#endif
		/*
		 * Check if the subdomain is allowed to be off long enough for
		 * the current domain to turn off and on (that's how much time
		 * it will have to wait worst case).
		 */
		if (sd_max_off_ns <= off_on_time_ns)
			return false;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)

		if (min_off_time_ns > sd_max_off_ns || min_off_time_ns < 0)
			min_off_time_ns = sd_max_off_ns;
#endif
	}

	/*
	 * Check if the devices in the domain can be off enough time.
	 */
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	min_dev_off_time_ns = -1;
#endif
	list_for_each_entry(pdd, &genpd->dev_list, list_node) {
		struct gpd_timing_data *td;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		struct device *dev = pdd->dev;
		s64 dev_off_time_ns;
#else
		s64 constraint_ns;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		if (!dev->driver || dev->power.max_time_suspended_ns < 0)
#else
		if (!pdd->dev->driver)
#endif
			continue;

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
		/*
		 * Check if the device is allowed to be off long enough for the
		 * domain to turn off and on (that's how much time it will
		 * have to wait worst case).
		 */
#endif
		td = &to_gpd_data(pdd)->td;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		dev_off_time_ns = dev->power.max_time_suspended_ns -
			(td->start_latency_ns + td->restore_state_latency_ns +
				ktime_to_ns(ktime_sub(time_now,
						dev->power.suspend_time)));
		if (dev_off_time_ns <= off_on_time_ns)
			return false;

		if (min_dev_off_time_ns > dev_off_time_ns
		    || min_dev_off_time_ns < 0)
			min_dev_off_time_ns = dev_off_time_ns;
	}
#else
		constraint_ns = td->effective_constraint_ns;
		/* default_stop_ok() need not be called before us. */
		if (constraint_ns < 0) {
			constraint_ns = dev_pm_qos_read_value(pdd->dev);
			constraint_ns *= NSEC_PER_USEC;
		}
		if (constraint_ns == 0)
			continue;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	if (min_dev_off_time_ns < 0) {
#endif
		/*
		 * There are no latency constraints, so the domain can spend
		 * arbitrary time in the "off" state.
		 */
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		genpd->max_off_time_ns = -1;
		return true;
#else
		constraint_ns -= td->restore_state_latency_ns;
		if (constraint_ns <= off_on_time_ns)
			return false;

		if (min_off_time_ns > constraint_ns || min_off_time_ns < 0)
			min_off_time_ns = constraint_ns;
#endif
	}

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	genpd->cached_power_down_ok = true;

#endif
	/*
	 * The difference between the computed minimum delta and the time needed
	 * to turn the domain on is the maximum theoretical time this domain can
	 * spend in the "off" state.
	 */
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	min_dev_off_time_ns -= genpd->power_on_latency_ns;
#else
	if (min_off_time_ns < 0)
		return true;
#endif

	/*
	 * If the difference between the computed minimum delta and the time
	 * needed to turn the domain off and back on on is smaller than the
	 * domain's power break even time, removing power from the domain is not
	 * worth it.
	 */
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	if (genpd->break_even_ns >
	    min_dev_off_time_ns - genpd->power_off_latency_ns)
		return false;

	genpd->max_off_time_ns = min_dev_off_time_ns;
#else
	genpd->max_off_time_ns = min_off_time_ns - genpd->power_on_latency_ns;
#endif
	return true;
}

static bool always_on_power_down_ok(struct dev_pm_domain *domain)
{
	return false;
}

#else /* !CONFIG_PM_RUNTIME */

bool default_stop_ok(struct device *dev)
{
	return false;
}

#define default_power_down_ok	NULL
#define always_on_power_down_ok	NULL

#endif /* !CONFIG_PM_RUNTIME */

struct dev_power_governor simple_qos_governor = {
	.stop_ok = default_stop_ok,
	.power_down_ok = default_power_down_ok,
};

/**
 * pm_genpd_gov_always_on - A governor implementing an always-on policy
 */
struct dev_power_governor pm_domain_always_on_gov = {
	.power_down_ok = always_on_power_down_ok,
	.stop_ok = default_stop_ok,
};
