// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2004, Marcin Sloniewski <marcin.sloniewski@gmail.com>
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <serial.h>
#include <fdtdec.h>
#include <asm/io.h>
#include <clk.h>
#include <debug_uart.h>

DECLARE_GLOBAL_DATA_PTR;

/* offsets */
#define OFF_RXTX        0x00
#define OFF_TXFULL      0x04
#define OFF_RXEMPTY     0x08
#define OFF_EV_STATUS   0x0c
#define OFF_EV_PENDING  0x10
#define OFF_EV_ENABLE   0x14

/* events */
#define UART_EV_TX           0x1
#define UART_EV_RX           0x2


struct litex_uart_regs {
	u32	rxtxdata;
	u32	txfull;
	u32	rxempty;
	u32	ev_status;
	u32	ev_pending;
	u32	ev_enable;
};

struct litex_uart_platdata {
	struct litex_uart_regs *regs;
};

static int litex_uart_setbrg(struct udevice *dev, int baudrate)
{
	return 0;
}

static int litex_uart_putc(struct udevice *dev, const char ch)
{
	struct litex_uart_platdata *plat = dev->platdata;
	struct litex_uart_regs *const regs = plat->regs;

	while (readl(&regs->txfull));

	writel(ch, &regs->rxtxdata);
	writel(UART_EV_TX, &regs->ev_pending);

	return 0;
}

static int litex_uart_getc(struct udevice *dev)
{
	struct litex_uart_platdata *plat = dev->platdata;
	struct litex_uart_regs *const regs = plat->regs;

	if (readl(&regs->rxempty))
		return -EAGAIN;

	int character = readl(&regs->rxtxdata) & 0xff;
	writel(UART_EV_RX, &regs->ev_pending);

	return character;
}

static int litex_uart_probe(struct udevice *dev)
{
	struct litex_uart_platdata *plat = dev_get_platdata(dev);
	struct litex_uart_regs *const regs = plat->regs;

	writel(readl(&regs->ev_status), &regs->ev_pending);
	writel(UART_EV_TX | UART_EV_RX, &regs->ev_enable);
	return 0;
}

static int litex_uart_ofdata_to_platdata(struct udevice *dev)
{
	struct litex_uart_platdata *plat = dev_get_platdata(dev);

	plat->regs = (struct litex_uart_regs *)dev_read_addr(dev);

	return 0;
}

static const struct dm_serial_ops litex_uart_ops = {
	.putc = litex_uart_putc,
	.pending = NULL,
	.getc = litex_uart_getc,
	.setbrg = litex_uart_setbrg,
};

static const struct udevice_id litex_uart_ids[] = {
	{ .compatible = "litex,liteuart" },
	{}
};

U_BOOT_DRIVER(serial_litex) = {
	.name	= "serial_litex",
	.id	= UCLASS_SERIAL,
	.of_match = litex_uart_ids,
	.ofdata_to_platdata = litex_uart_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct litex_uart_platdata),
	.probe = litex_uart_probe,
	.ops	= &litex_uart_ops,
};

#ifdef CONFIG_DEBUG_UART_LITEX_UART

#include <debug_uart.h>

static inline void _debug_uart_init(void)
{
	struct litex_uart_regs *regs = (void *)CONFIG_DEBUG_UART_BASE;

	writel(readl(&regs->ev_status), &regs->ev_pending);
	writel(UART_EV_TX | UART_EV_RX, &regs->ev_enable);
}

static inline void _debug_uart_putc(int ch)
{
	struct litex_uart_regs *regs = (void *)CONFIG_DEBUG_UART_BASE;

	while (readl(&regs->txfull));

	writel(ch, &regs->rxtxdata);
	writel(UART_EV_TX, &regs->ev_pending);
}

DEBUG_UART_FUNCS

#endif
