/*
 * Copyright (C) 2013 Stefan Roese <sr@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/mxc_i2c.h>
#include <mmc.h>
#include <fsl_esdhc.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |	\
			PAD_CTL_DSE_40ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define USDHC_PAD_CTRL (PAD_CTL_PUS_47K_UP  | PAD_CTL_SPEED_LOW |	\
			PAD_CTL_DSE_80ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define ENET_PAD_CTRL  (PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED	  |	\
			PAD_CTL_DSE_40ohm   | PAD_CTL_HYS)

#define I2C_PAD_CTRL	(PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |	\
			 PAD_CTL_DSE_40ohm | PAD_CTL_HYS |		\
			 PAD_CTL_ODE | PAD_CTL_SRE_FAST)

#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)

struct i2c_pads_info i2c_pad_info0_6q = {
	.scl = {
		.i2c_mode = MX6Q_PAD_EIM_D21__I2C1_SCL | PC,
		.gpio_mode = MX6Q_PAD_EIM_D21__GPIO3_IO21 | PC,
		.gp = IMX_GPIO_NR(3, 21)
	},
	.sda = {
		.i2c_mode = MX6Q_PAD_EIM_D28__I2C1_SDA | PC,
		.gpio_mode = MX6Q_PAD_EIM_D28__GPIO3_IO28 | PC,
		.gp = IMX_GPIO_NR(3, 28)
	}
};

static struct i2c_pads_info i2c_pad_info1_6q = {
	.scl = {
		.i2c_mode = MX6Q_PAD_EIM_EB2__I2C2_SCL | PC,
		.gpio_mode = MX6Q_PAD_EIM_EB2__GPIO2_IO30 | PC,
		.gp = IMX_GPIO_NR(2, 30)
	},
	.sda = {
		.i2c_mode = MX6Q_PAD_EIM_D16__I2C2_SDA | PC,
		.gpio_mode = MX6Q_PAD_EIM_D16__GPIO3_IO16 | PC,
		.gp = IMX_GPIO_NR(3, 16)
	}
};


struct i2c_pads_info i2c_pad_info0_6dl = {
	.scl = {
		.i2c_mode = MX6DL_PAD_CSI0_DAT9__I2C1_SCL | PC,
		.gpio_mode = MX6DL_PAD_CSI0_DAT9__GPIO5_IO27 | PC,
		.gp = IMX_GPIO_NR(5, 27)
	},
	.sda = {
		.i2c_mode = MX6DL_PAD_CSI0_DAT8__I2C1_SDA | PC,
		.gpio_mode = MX6DL_PAD_CSI0_DAT8__GPIO5_IO26 | PC,
		.gp = IMX_GPIO_NR(5, 26)
	}
	/*
	.scl = {
		.i2c_mode = MX6DL_PAD_EIM_D21__I2C1_SCL | PC,
		.gpio_mode = MX6DL_PAD_EIM_D21__GPIO3_IO21 | PC,
		.gp = IMX_GPIO_NR(3, 21)
	},
	.sda = {
		.i2c_mode = MX6DL_PAD_EIM_D28__I2C1_SDA | PC,
		.gpio_mode = MX6DL_PAD_EIM_D28__GPIO3_IO28 | PC,
		.gp = IMX_GPIO_NR(3, 28)
	}
	*/
};

static struct i2c_pads_info i2c_pad_info1_6dl = {
	.scl = {
		.i2c_mode = MX6DL_PAD_EIM_EB2__I2C2_SCL | PC,
		.gpio_mode = MX6DL_PAD_EIM_EB2__GPIO2_IO30 | PC,
		.gp = IMX_GPIO_NR(2, 30)
	},
	.sda = {
		.i2c_mode = MX6DL_PAD_EIM_D16__I2C2_SDA | PC,
		.gpio_mode = MX6DL_PAD_EIM_D16__GPIO3_IO16 | PC,
		.gp = IMX_GPIO_NR(3, 16)
	}
};


static iomux_v3_cfg_t const uart2_pads_6q[] = {
	MX6Q_PAD_EIM_D26__UART2_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6Q_PAD_EIM_D27__UART2_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const uart2_pads_6dl[] = {
	MX6DL_PAD_EIM_D26__UART2_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6DL_PAD_EIM_D27__UART2_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};


static iomux_v3_cfg_t usdhc2_pads_6q[] = {
	MX6Q_PAD_SD2_CLK__SD2_CLK    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD2_CMD__SD2_CMD    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD2_DAT0__SD2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD2_DAT1__SD2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD2_DAT2__SD2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD2_DAT3__SD2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_GPIO_4__GPIO1_IO04  | MUX_PAD_CTRL(NO_PAD_CTRL), /* CD */
};

static iomux_v3_cfg_t const usdhc3_pads_6q[] = {
	MX6Q_PAD_SD3_CLK__SD3_CLK    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD3_CMD__SD3_CMD    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD3_DAT0__SD3_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD3_DAT1__SD3_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD3_DAT2__SD3_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6Q_PAD_SD3_DAT3__SD3_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

static iomux_v3_cfg_t usdhc2_pads_6dl[] = {
	MX6DL_PAD_SD2_CLK__SD2_CLK    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD2_CMD__SD2_CMD    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD2_DAT0__SD2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD2_DAT1__SD2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD2_DAT2__SD2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD2_DAT3__SD2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_GPIO_4__GPIO1_IO04  | MUX_PAD_CTRL(NO_PAD_CTRL), /* CD */
};

static iomux_v3_cfg_t const usdhc3_pads_6dl[] = {
	MX6DL_PAD_SD3_CLK__SD3_CLK    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD3_CMD__SD3_CMD    | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD3_DAT0__SD3_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD3_DAT1__SD3_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD3_DAT2__SD3_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6DL_PAD_SD3_DAT3__SD3_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};


#define URTX0           0x40            /* Transmitter Register */

#define UCR1            0x80            /* Control Register 1 */
#define UCR1_UARTEN     (1 << 0)        /* UART enabled */

#define USR2            0x98            /* Status Register 2 */
#define USR2_TXDC       (1 << 3)        /* Transmitter complete */

static inline void putc_ll(int c)
{
        void __iomem *base = (void *)UART2_BASE;

        if (!base)
                return;

        if (!(readl(base + UCR1) & UCR1_UARTEN))
                return;

        while (!(readl(base + USR2) & USR2_TXDC));

        writel(c, base + URTX0);
}

static void early_uart_init(void)
{
	void __iomem *ccmbase = (void *)CCM_BASE_ADDR;
	void __iomem *uartbase = (void *)UART2_BASE;

	if (is_cpu_type(MXC_CPU_MX6Q))
		imx_iomux_v3_setup_multiple_pads(uart2_pads_6q,
			ARRAY_SIZE(uart2_pads_6q));
	else
		imx_iomux_v3_setup_multiple_pads(uart2_pads_6dl,
			ARRAY_SIZE(uart2_pads_6dl));

	writel(0xffffffff, ccmbase + 0x68);
	writel(0xffffffff, ccmbase + 0x6c);
	writel(0xffffffff, ccmbase + 0x70);
	writel(0xffffffff, ccmbase + 0x74);
	writel(0xffffffff, ccmbase + 0x78);
	writel(0xffffffff, ccmbase + 0x7c);
	writel(0xffffffff, ccmbase + 0x80);
	
	writel(readl(ccmbase + 0x7c) | 0x0f000000, ccmbase + 0x7c); // ungate clock

	writel(0x00000000, uartbase + 0x80);
	writel(0x00004027, uartbase + 0x84);
	writel(0x00000704, uartbase + 0x88);
	writel(0x00000a81, uartbase + 0x90);
	writel(0x0000002b, uartbase + 0x9c);
	writel(0x00013880, uartbase + 0xb0);
	writel(0x0000047f, uartbase + 0xa4);
	writel(0x0000c34f, uartbase + 0xa8);
	writel(0x00000001, uartbase + 0x80);
}

void print_a() {
  early_uart_init();
  while(1) {
    putc_ll('x');
    //    *((unsigned int *) 0x10000000) = 0;
  }

}

void basic_test() {
  volatile unsigned int dummy;

  return;  // don't execute this for now
  early_uart_init();

  /*  
	printf("Showing clocks:\n");
	do_mx6_showclocks(NULL, 0, 0, NULL);

	printf("Dumping CCM registers:\n");
	for (i = 0x020c4000; i <= 0x020c4088; i += 4)
		printf("\t0x%08x: 0x%08x\n", i, *((volatile uint32_t *)i));

	printf("Dumping CCM analog:\n");
	for (i = 0x020c8000; i <= 0x020c8100; i += 0x10)
		printf("\t0x%08x: 0x%08x\n", i, *((volatile uint32_t *)i));
  */

  dummy = 1;

  while(1) {
    putc_ll('a');
    putc_ll('a');
    putc_ll('a');
    putc_ll('a');
    *(unsigned int *) 0x10000000 = dummy;
  }
}


#ifdef CONFIG_USB_EHCI_MX6
int board_ehci_hcd_init(int port)
{
	return 0;
}

#endif

#ifdef CONFIG_FSL_ESDHC
struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC3_BASE_ADDR},
	{USDHC2_BASE_ADDR},
};

int board_mmc_getcd(struct mmc *mmc)
{
  int i;
  printf( "reporting CD is always true\n" );
  return 1; // just fail, we will only go from default environment
}

int board_mmc_init(bd_t *bis)
{
	s32 status = 0;
	u32 index = 0;

	printf( "board_mmc_init(): \n" );
	for (index = 0; index < CONFIG_SYS_FSL_USDHC_NUM; ++index) {
		switch (index) {
		case 0:
			if (is_cpu_type(MXC_CPU_MX6Q)) {
			  printf( "setting up usdhc3 pads, q\n" );
				imx_iomux_v3_setup_multiple_pads(
					usdhc3_pads_6q, ARRAY_SIZE(usdhc3_pads_6q));
					usdhc_cfg[index].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
			}
			else {
			  printf( "setting up usdhc3 pads, dl\n" );
				imx_iomux_v3_setup_multiple_pads(
					usdhc3_pads_6dl, ARRAY_SIZE(usdhc3_pads_6dl));
					usdhc_cfg[index].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
			}
			break;
		case 1:
			if (is_cpu_type(MXC_CPU_MX6Q)) {
			  printf( "setting up usdhc2 pads, q\n" );
				imx_iomux_v3_setup_multiple_pads(
					usdhc2_pads_6q, ARRAY_SIZE(usdhc2_pads_6q));
					usdhc_cfg[index].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
			}
			else {
			  printf( "setting up usdhc2 pads, dl\n" );
				imx_iomux_v3_setup_multiple_pads(
					usdhc2_pads_6dl, ARRAY_SIZE(usdhc2_pads_6dl));
					usdhc_cfg[index].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
			}
			break;
		default:
			printf("Warning: you configured more USDHC controllers"
				"(%d) then supported by the board (%d)\n",
				index + 1, CONFIG_SYS_FSL_USDHC_NUM);
			return status;
		}

		status |= fsl_esdhc_initialize(bis, &usdhc_cfg[index]);
	}

	return status;
}
#endif

int board_early_init_f(void)
{
  int i;
	early_uart_init();
	/*
	printf( "set analog_misc_2\n" );
	*((unsigned int *) 0x20c8170) = 0x272727;
	*((unsigned int *) 0x20c8140) = 0x5c2e17;
	*/
	//	*((unsigned int *) 0x20c4018) = 0x820324; // dubious

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	if (is_cpu_type(MXC_CPU_MX6Q)) {
	  setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info0_6q);
	  setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1_6q);
	} else {
	  setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info0_6dl);
	  setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1_6dl);
	}
	return 0;
}

int checkboard(void)
{
	puts("Board: Novena (stage1)\n");

	return 0;
}

int misc_init_r(void)
{
	return 0;
}
