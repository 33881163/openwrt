/*
 * Aerohive HiveAP-370 Board Setup
 *
 * Copyright (C) 2017 Chris Blake <chrisrblake93@gmail.com>
 *
 * Based on:
 *   mpc85xx_rdb.c:
 *      MPC85xx RDB Board Setup
 *      Copyright 2013 Freescale Semiconductor Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include "smp.h"

#include "mpc85xx.h"

void __init hiveap_370_pic_init(void)
{
	struct mpic *mpic;

	mpic = mpic_alloc(NULL, 0,
	  MPIC_BIG_ENDIAN |
	  MPIC_SINGLE_DEST_CPU,
	  0, 256, " OpenPIC  ");

	BUG_ON(mpic == NULL);
	mpic_init(mpic);
}

/*
 * Setup the architecture
 */
static void __init hiveap_370_setup_arch(void)
{
	if (ppc_md.progress)
		ppc_md.progress("hiveap_370_setup_arch()", 0);

	mpc85xx_smp_init();

	fsl_pci_assign_primary();

	printk(KERN_INFO "HiveAP-370 board from Aerohive\n");
}

machine_arch_initcall(hiveap_370, mpc85xx_common_publish_devices);

/*
 * Called very early, device-tree isn't unflattened
 */
static int __init hiveap_370_probe(void)
{
	if (of_machine_is_compatible("aerohive,hiveap-370"))
		return 1;
	return 0;
}

define_machine(hiveap_370) {
	.name			= "P1020 RDB",
	.probe			= hiveap_370_probe,
	.setup_arch		= hiveap_370_setup_arch,
	.init_IRQ		= hiveap_370_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
	.pcibios_fixup_phb      = fsl_pcibios_fixup_phb,
#endif
	.get_irq		= mpic_get_irq,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};
