/*
 *  linux/mm/mmu_notifier.c
 *
 *  Copyright (C) 2008  Qumranet, Inc.
 *  Copyright (C) 2008  SGI
 *             Christoph Lameter <clameter@sgi.com>
 *
 *  This work is licensed under the terms of the GNU GPL, version 2. See
 *  the COPYING file in the top-level directory.
 */

#include <linux/rculist.h>
#include <linux/mmu_notifier.h>
#include <linux/export.h>
#include <linux/mm.h>
#include <linux/err.h>
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
#include <linux/srcu.h>
#endif
#include <linux/rcupdate.h>
#include <linux/sched.h>
#include <linux/slab.h>

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
/* global SRCU for all MMs */
static struct srcu_struct srcu;

#endif
/*
 * This function can't run concurrently against mmu_notifier_register
 * because mm->mm_users > 0 during mmu_notifier_register and exit_mmap
 * runs with mm_users == 0. Other tasks may still invoke mmu notifiers
 * in parallel despite there being no task using this mm any more,
 * through the vmas outside of the exit_mmap context, such as with
 * vmtruncate. This serializes against mmu_notifier_unregister with
 * the mmu_notifier_mm->lock in addition to RCU and it serializes
 * against the other mmu notifiers with RCU. struct mmu_notifier_mm
 * can't go away from under us as exit_mmap holds an mm_count pin
 * itself.
 */
void __mmu_notifier_release(struct mm_struct *mm)
{
	struct mmu_notifier *mn;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	struct hlist_node *n;
#else
	int id;
#endif

	/*
	 * RCU here will block mmu_notifier_unregister until
	 * ->release returns.
	 */
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist)
		/*
		 * if ->release runs before mmu_notifier_unregister it
		 * must be handled as it's the only way for the driver
		 * to flush all existing sptes and stop the driver
		 * from establishing any more sptes before all the
		 * pages in the mm are freed.
		 */
		if (mn->ops->release)
			mn->ops->release(mn, mm);
	rcu_read_unlock();

#else
	id = srcu_read_lock(&srcu);
#endif
	spin_lock(&mm->mmu_notifier_mm->lock);
	while (unlikely(!hlist_empty(&mm->mmu_notifier_mm->list))) {
		mn = hlist_entry(mm->mmu_notifier_mm->list.first,
				 struct mmu_notifier,
				 hlist);
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)

#endif
		/*
		 * We arrived before mmu_notifier_unregister so
		 * mmu_notifier_unregister will do nothing other than
		 * to wait ->release to finish and
		 * mmu_notifier_unregister to return.
		 */
		hlist_del_init_rcu(&mn->hlist);
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
		spin_unlock(&mm->mmu_notifier_mm->lock);

		/*
		 * Clear sptes. (see 'release' description in mmu_notifier.h)
		 */
		if (mn->ops->release)
			mn->ops->release(mn, mm);

		spin_lock(&mm->mmu_notifier_mm->lock);
#endif
	}
	spin_unlock(&mm->mmu_notifier_mm->lock);

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	/*
	 * synchronize_rcu here prevents mmu_notifier_release to
	 * return to exit_mmap (which would proceed freeing all pages
	 * in the mm) until the ->release method returns, if it was
	 * invoked by mmu_notifier_unregister.
	 *
	 * The mmu_notifier_mm can't go away from under us because one
	 * mm_count is hold by exit_mmap.
	 */
	synchronize_rcu();
#else
	/*
	 * All callouts to ->release() which we have done are complete.
	 * Allow synchronize_srcu() in mmu_notifier_unregister() to complete
	 */
	srcu_read_unlock(&srcu, id);

	/*
	 * mmu_notifier_unregister() may have unlinked a notifier and may
	 * still be calling out to it.	Additionally, other notifiers
	 * may have been active via vmtruncate() et. al. Block here
	 * to ensure that all notifier callouts for this mm have been
	 * completed and the sptes are really cleaned up before returning
	 * to exit_mmap().
	 */
	synchronize_srcu(&srcu);
#endif
}

/*
 * If no young bitflag is supported by the hardware, ->clear_flush_young can
 * unmap the address and return 1 or 0 depending if the mapping previously
 * existed or not.
 */
int __mmu_notifier_clear_flush_young(struct mm_struct *mm,
					unsigned long address)
{
	struct mmu_notifier *mn;
	struct hlist_node *n;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	int young = 0;
#else
	int young = 0, id;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
#else
	id = srcu_read_lock(&srcu);
#endif
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist) {
		if (mn->ops->clear_flush_young)
			young |= mn->ops->clear_flush_young(mn, mm, address);
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_unlock();
#else
	srcu_read_unlock(&srcu, id);
#endif

	return young;
}

int __mmu_notifier_test_young(struct mm_struct *mm,
			      unsigned long address)
{
	struct mmu_notifier *mn;
	struct hlist_node *n;
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	int young = 0;
#else
	int young = 0, id;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
#else
	id = srcu_read_lock(&srcu);
#endif
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist) {
		if (mn->ops->test_young) {
			young = mn->ops->test_young(mn, mm, address);
			if (young)
				break;
		}
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_unlock();
#else
	srcu_read_unlock(&srcu, id);
#endif

	return young;
}

void __mmu_notifier_change_pte(struct mm_struct *mm, unsigned long address,
			       pte_t pte)
{
	struct mmu_notifier *mn;
	struct hlist_node *n;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	int id;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
#else
	id = srcu_read_lock(&srcu);
#endif
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist) {
		if (mn->ops->change_pte)
			mn->ops->change_pte(mn, mm, address, pte);
		/*
		 * Some drivers don't have change_pte,
		 * so we must call invalidate_page in that case.
		 */
		else if (mn->ops->invalidate_page)
			mn->ops->invalidate_page(mn, mm, address);
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_unlock();
#else
	srcu_read_unlock(&srcu, id);
#endif
}

void __mmu_notifier_invalidate_page(struct mm_struct *mm,
					  unsigned long address)
{
	struct mmu_notifier *mn;
	struct hlist_node *n;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	int id;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
#else
	id = srcu_read_lock(&srcu);
#endif
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist) {
		if (mn->ops->invalidate_page)
			mn->ops->invalidate_page(mn, mm, address);
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_unlock();
#else
	srcu_read_unlock(&srcu, id);
#endif
}

void __mmu_notifier_invalidate_range_start(struct mm_struct *mm,
				  unsigned long start, unsigned long end)
{
	struct mmu_notifier *mn;
	struct hlist_node *n;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	int id;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
#else
	id = srcu_read_lock(&srcu);
#endif
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist) {
		if (mn->ops->invalidate_range_start)
			mn->ops->invalidate_range_start(mn, mm, start, end);
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_unlock();
#else
	srcu_read_unlock(&srcu, id);
#endif
}

void __mmu_notifier_invalidate_range_end(struct mm_struct *mm,
				  unsigned long start, unsigned long end)
{
	struct mmu_notifier *mn;
	struct hlist_node *n;
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	int id;
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_lock();
#else
	id = srcu_read_lock(&srcu);
#endif
	hlist_for_each_entry_rcu(mn, n, &mm->mmu_notifier_mm->list, hlist) {
		if (mn->ops->invalidate_range_end)
			mn->ops->invalidate_range_end(mn, mm, start, end);
	}
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	rcu_read_unlock();
#else
	srcu_read_unlock(&srcu, id);
#endif
}

static int do_mmu_notifier_register(struct mmu_notifier *mn,
				    struct mm_struct *mm,
				    int take_mmap_sem)
{
	struct mmu_notifier_mm *mmu_notifier_mm;
	int ret;

	BUG_ON(atomic_read(&mm->mm_users) <= 0);

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	/*
	* Verify that mmu_notifier_init() already run and the global srcu is
	* initialized.
	*/
	BUG_ON(!srcu.per_cpu_ref);

#endif
	ret = -ENOMEM;
	mmu_notifier_mm = kmalloc(sizeof(struct mmu_notifier_mm), GFP_KERNEL);
	if (unlikely(!mmu_notifier_mm))
		goto out;

	if (take_mmap_sem)
		down_write(&mm->mmap_sem);
	ret = mm_take_all_locks(mm);
	if (unlikely(ret))
		goto out_cleanup;

	if (!mm_has_notifiers(mm)) {
		INIT_HLIST_HEAD(&mmu_notifier_mm->list);
		spin_lock_init(&mmu_notifier_mm->lock);
		mm->mmu_notifier_mm = mmu_notifier_mm;
		mmu_notifier_mm = NULL;
	}
	atomic_inc(&mm->mm_count);

	/*
	 * Serialize the update against mmu_notifier_unregister. A
	 * side note: mmu_notifier_release can't run concurrently with
	 * us because we hold the mm_users pin (either implicitly as
	 * current->mm or explicitly with get_task_mm() or similar).
	 * We can't race against any other mmu notifier method either
	 * thanks to mm_take_all_locks().
	 */
	spin_lock(&mm->mmu_notifier_mm->lock);
	hlist_add_head(&mn->hlist, &mm->mmu_notifier_mm->list);
	spin_unlock(&mm->mmu_notifier_mm->lock);

	mm_drop_all_locks(mm);
out_cleanup:
	if (take_mmap_sem)
		up_write(&mm->mmap_sem);
	/* kfree() does nothing if mmu_notifier_mm is NULL */
	kfree(mmu_notifier_mm);
out:
	BUG_ON(atomic_read(&mm->mm_users) <= 0);
	return ret;
}

/*
 * Must not hold mmap_sem nor any other VM related lock when calling
 * this registration function. Must also ensure mm_users can't go down
 * to zero while this runs to avoid races with mmu_notifier_release,
 * so mm has to be current->mm or the mm should be pinned safely such
 * as with get_task_mm(). If the mm is not current->mm, the mm_users
 * pin should be released by calling mmput after mmu_notifier_register
 * returns. mmu_notifier_unregister must be always called to
 * unregister the notifier. mm_count is automatically pinned to allow
 * mmu_notifier_unregister to safely run at any time later, before or
 * after exit_mmap. ->release will always be called before exit_mmap
 * frees the pages.
 */
int mmu_notifier_register(struct mmu_notifier *mn, struct mm_struct *mm)
{
	return do_mmu_notifier_register(mn, mm, 1);
}
EXPORT_SYMBOL_GPL(mmu_notifier_register);

/*
 * Same as mmu_notifier_register but here the caller must hold the
 * mmap_sem in write mode.
 */
int __mmu_notifier_register(struct mmu_notifier *mn, struct mm_struct *mm)
{
	return do_mmu_notifier_register(mn, mm, 0);
}
EXPORT_SYMBOL_GPL(__mmu_notifier_register);

/* this is called after the last mmu_notifier_unregister() returned */
void __mmu_notifier_mm_destroy(struct mm_struct *mm)
{
	BUG_ON(!hlist_empty(&mm->mmu_notifier_mm->list));
	kfree(mm->mmu_notifier_mm);
	mm->mmu_notifier_mm = LIST_POISON1; /* debug */
}

/*
 * This releases the mm_count pin automatically and frees the mm
 * structure if it was the last user of it. It serializes against
 * running mmu notifiers with RCU and against mmu_notifier_unregister
 * with the unregister lock + RCU. All sptes must be dropped before
 * calling mmu_notifier_unregister. ->release or any other notifier
 * method may be invoked concurrently with mmu_notifier_unregister,
 * and only after mmu_notifier_unregister returned we're guaranteed
 * that ->release or any other method can't run anymore.
 */
void mmu_notifier_unregister(struct mmu_notifier *mn, struct mm_struct *mm)
{
	BUG_ON(atomic_read(&mm->mm_count) <= 0);

#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
	spin_lock(&mm->mmu_notifier_mm->lock);
#endif
	if (!hlist_unhashed(&mn->hlist)) {
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		/*
		 * RCU here will force exit_mmap to wait ->release to finish
		 * before freeing the pages.
		 */
		rcu_read_lock();
#else
		int id;
#endif

		/*
		 * exit_mmap will block in mmu_notifier_release to
		 * guarantee ->release is called before freeing the
		 * pages.
		 */
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)
		id = srcu_read_lock(&srcu);

		hlist_del_rcu(&mn->hlist);
		spin_unlock(&mm->mmu_notifier_mm->lock);

#endif
		if (mn->ops->release)
			mn->ops->release(mn, mm);
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		rcu_read_unlock();
#endif

#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
		spin_lock(&mm->mmu_notifier_mm->lock);
		hlist_del_rcu(&mn->hlist);
#else
		/*
		 * Allow __mmu_notifier_release() to complete.
		 */
		srcu_read_unlock(&srcu, id);
	} else
#endif
		spin_unlock(&mm->mmu_notifier_mm->lock);
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	}
#endif

	/*
	 * Wait any running method to finish, of course including
	 * ->release if it was run by mmu_notifier_relase instead of us.
	 */
#if !defined(CONFIG_BCM_KF_ANDROID) || !defined(CONFIG_BCM_ANDROID)
	synchronize_rcu();
#else
	synchronize_srcu(&srcu);
#endif

	BUG_ON(atomic_read(&mm->mm_count) <= 0);

	mmdrop(mm);
}
EXPORT_SYMBOL_GPL(mmu_notifier_unregister);
#if defined(CONFIG_BCM_KF_ANDROID) && defined(CONFIG_BCM_ANDROID)

static int __init mmu_notifier_init(void)
{
	return init_srcu_struct(&srcu);
}

module_init(mmu_notifier_init);
#endif
