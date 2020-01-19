/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Authors:
 *   Marcin Sloniewski <marcin.sloniewski@gmail.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#define CONFIG_SYS_SDRAM_BASE		0xC0000000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + SZ_2M)

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + SZ_2M)

#define CONFIG_SYS_MALLOC_LEN		SZ_8M

#define CONFIG_SYS_BOOTM_LEN		SZ_64M

#define CONFIG_STANDALONE_LOAD_ADDR	0xC0000000

/* Environment options */
#define BOOT_TARGET_DEVICES(func)

#include <config_distro_bootcmd.h>

#endif /* __CONFIG_H */
