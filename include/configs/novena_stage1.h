/*
 * Copyright (C) 2013 Stefan Roese <sr@denx.de>
 *
 * Configuration settings for the ProjectionDesign / Barco
 * Titanium board.
 *
 * Based on mx6qsabrelite.h which is:
 * Copyright (C) 2010-2011 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "mx6_common.h"
#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>

#define CONFIG_MX6
#define CONFIG_MX6QDL
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(2 * 1024 * 1024)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_MISC_INIT_R
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART2_BASE

/* I2C Configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000

/* MMC Configs */
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_USDHC_NUM	2

#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
//#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION

/* allow to overwrite serial and ethaddr */
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

#define CONFIG_BOOTDELAY		3

#define CONFIG_LOADADDR			0x10008000

//#define CONFIG_SYS_TEXT_BASE		0x009073e0  // to OCRAM
#define CONFIG_SYS_TEXT_BASE		0x20000000  // to DDR3, at 512MB up

/* The i.MX6DL has 128 KiB of RAM, so put the stack at the end */
#define CONFIG_SYS_INIT_SP_OFFSET	(0x30020000 - 8)
#define CONFIG_SYS_INIT_SP_ADDR		(0x30920000 - 8)


#define CONFIG_SYS_MEMTEST_START	0x10000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + (500 << 20))

#define CONFIG_HOSTNAME			novena

#define CONFIG_BOOTCOMMAND		"i2c dev 1; i2c mw 08 66 48; mmc dev 1; mmc read 0x10008000 0x100 0x8000; bootz 0x10008000" // ; go 0x10800000
//#define CONFIG_BOOTCOMMAND		"run foo"

/* Miscellaneous configurable options */
#define CONFIG_SYS_PROMPT		"Novena > "
#define CONFIG_SYS_CONSOLE_INFO_QUIET	/* don't print console @ startup */

#define CONFIG_SYS_CBSIZE		256

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					 sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_SIZE			(512 << 20)

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

/* Environment in MMC */
#define CONFIG_ENV_SIZE			(8 << 10)
//#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_OFFSET		(6 * 64 * 1024)
#define CONFIG_SYS_MMC_ENV_DEV		0


//#ifndef CONFIG_SYS_DCACHE_OFF
//#define CONFIG_CMD_CACHE
//#endif

#define CONFIG_CMD_BOOTZ
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_CMD_BOOTM

/* Disable some things to shrink size */
#undef CONFIG_GZIP
#undef CONFIG_BOOTM_PLAN9
#undef CONFIG_BOOTM_VXWORKS
#undef CONFIG_CMD_CRC32
#undef CONFIG_LMB
#endif			       /* __CONFIG_H */
