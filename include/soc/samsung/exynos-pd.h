/*
 * Exynos PM domain support.
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com
 *
 * Implementation of Exynos specific power domain control which is used in
 * conjunction with runtime-pm. Support for both device-tree and non-device-tree
 * based power domain support is included.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __EXYNOS_PD_H
#define __EXYNOS_PD_H __FILE__

#include <linux/io.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pm_domain.h>
#include <linux/pm_runtime.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/mutex.h>

#include <linux/mfd/samsung/core.h>
#include <soc/samsung/bcm.h>

#include "../../../drivers/soc/samsung/pwrcal/pwrcal.h"

#include <soc/samsung/exynos-powermode.h>
#include <soc/samsung/exynos-pm.h>
#include <soc/samsung/exynos-devfreq.h>
#include <soc/samsung/bts.h>

#define EXYNOS_PD_PREFIX	"EXYNOS-PD: "
#define EXYNOS_PD_DBG_PREFIX	"EXYNOS-PD-DBG: "

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif

#ifdef CONFIG_EXYNOS_PM_DOMAIN_DEBUG
#define DEBUG_PRINT_INFO(fmt, ...) printk(PM_DOMAIN_PREFIX pr_fmt(fmt), ##__VA_ARGS__)
#else
#define DEBUG_PRINT_INFO(fmt, ...)
#endif

/* In Exynos, the number of MAX_POWER_DOMAIN is less than 15 */
#define MAX_PARENT_POWER_DOMAIN	15

struct exynos_pm_domain;

struct exynos_pm_domain {
	struct generic_pm_domain genpd;
	char *name;
	unsigned int cal_pdid;
	struct device_node *of_node;
	int (*pd_control)(unsigned int cal_id, int on);
	int (*check_status)(struct exynos_pm_domain *pd);
	unsigned int bts;
	int devfreq_index;
	struct mutex access_lock;
	int idle_ip_index;
#if defined(CONFIG_EXYNOS_BCM)
	struct bcm_info *bcm;
#endif
	bool check_cp_status;
};

struct exynos_pd_dbg_info {
	struct device *dev;
#ifdef CONFIG_DEBUG_FS
	struct dentry *d;
	struct file_operations fops;
#endif
};

#ifdef CONFIG_EXYNOS_PD
struct exynos_pm_domain *exynos_pd_lookup_name(const char *domain_name);
#else
static inline struct exynos_pm_domain *exynos_pd_lookup_name(const char *domain_name)
{
	return NULL;
}
#endif

#endif /* __EXYNOS_PD_H */
