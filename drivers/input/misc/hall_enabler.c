/*
 * drivers/input/misc/hall_enabler.c
 *
 * Copyright (C) 2015, Levin Calado <levincalado@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/input.h>
#include <linux/hrtimer.h>
#include <asm-generic/cputime.h>
#include <linux/input/hall_enabler.h>

unsigned hall_switch = 0;

int hall_enable(void)
{

	if (hall_switch == 0) {
		return 0;

	} else if (hall_switch == 1) {
	    return 1;

	} else {
	    return 0;

	}
}

static ssize_t hall_enabler_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", hall_switch);
}

static ssize_t hall_enabler_set(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned int val = 0;

	sscanf(buf, "%u\n", &val);

	if ( ( val == 0 ) || ( val == 1 ) )
		hall_switch = val;

	return size;
}

static DEVICE_ATTR(enable, 0755,
		hall_enabler_show, hall_enabler_set);

static struct attribute *hall_enabler_attributes[] =
{
	&dev_attr_enable.attr,
	NULL
};

static struct attribute_group hall_enabler_group =
{
	.attrs  = hall_enabler_attributes,
};

#define HALL_ENABLER_DECLARED
#ifdef HALL_ENABLER_DECLARED
extern struct kobject *hall_enabler_kobj;
#else
struct kobject *hall_enabler_kobj;
EXPORT_SYMBOL_GPL(hall_enabler_kobj);
#endif

static int hall_enabler_init_sysfs(void) {

	int rc = 0;

	struct kobject *hall_enabler_kobj;
	hall_enabler_kobj = kobject_create_and_add("hall_enabler", NULL);

	rc = sysfs_create_group(hall_enabler_kobj,
			&hall_enabler_group);

	if (unlikely(rc < 0))
		pr_err("hall_enabler: sysfs_create_group failed: %d\n", rc);

	return rc;

}

static int hall_enabler_init(void) {

	int rc = 0;

	rc = hall_enabler_init_sysfs();

	return rc;

}

late_initcall(hall_enabler_init);
