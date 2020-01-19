// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2020 Marcin Sloniewski <marcin.sloniewski@gmail.com>
 *
 * Authors:
 *   Marcin Sloniewski <marcin.sloniewski@gmail.com>
 */

#include <common.h>
#include <dm.h>
#include <linux/delay.h>
#include <linux/io.h>

int misc_init_r(void)
{
	return 0;
}

int board_init(void)
{
	/* For now nothing to do here. */

	return 0;
}
