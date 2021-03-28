/*
 * Atheros MY-TENDA-O3_V2 reference board support
 *
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
 * Copyright (c) 2012 Gabor Juhos <juhosg@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define MY-TENDA-O3_V2_GPIO_LED_WAN		11
#define MY-TENDA-O3_V2_GPIO_LED_LAN		11
#define MY-TENDA-O3_V2_GPIO_LED_LINK1		12
#define MY-TENDA-O3_V2_GPIO_LED_LINK2		13
#define MY-TENDA-O3_V2_GPIO_LED_LINK3		14

#define MY-TENDA-O3_V2_GPIO_BTN_RESET		17

#define MY-TENDA-O3_V2_KEYS_POLL_INTERVAL	20	/* msecs */
#define MY-TENDA-O3_V2_KEYS_DEBOUNCE_INTERVAL	(3 * MY-TENDA-O3_V2_KEYS_POLL_INTERVAL)

#define MY-TENDA-O3_V2_MAC0_OFFSET		0
#define MY-TENDA-O3_V2_MAC1_OFFSET		6
#define MY-TENDA-O3_V2_WMAC_CALDATA_OFFSET	0x1000

static struct gpio_keys_button my-tenda-o3_v2_gpio_keys[] __initdata = {
	{
		.desc		= "Reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = MY-TENDA-O3_V2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= MY-TENDA-O3_V2_GPIO_BTN_RESET,
		.active_low	= 1,
	},
};

static struct gpio_led MY-TENDA-O3_V2_v5_leds_gpio[] __initdata = {

	{
		.name		= "my-tenda-o3-v2:green:lan1",
		.gpio		= MY-TENDA-O3_V2_GPIO_LED_LAN1,
		.active_low	= 1,
	}, {
		.name		= "my-tenda-o3-v2:green:lan2",
		.gpio		= MY-TENDA-O3_V2_GPIO_LED_LAN2,
		.active_low	= 1,
	}, {
		.name		= "my-tenda-o3-v2:green:wlan",
		.gpio		=  MY-TENDA-O3_V2_GPIO_LED_WLAN,
		.active_low	= 1,
	},{
		.name		= "my-tenda-o3-v2:red:link1",
		.gpio		= MY-TENDA-O3_V2_GPIO_LED_LINK1,
		.active_low	= 1,
	}, {
		.name		= "my-tenda-o3-v2:blue:link2",
		.gpio		= MY-TENDA-O3_V2_GPIO_LED_LINK2,
		.active_low	= 1,
	}, {
		.name		= "my-tenda-o3-v2:green:link3",
		.gpio		= MY-TENDA-O3_V2_GPIO_LED_LINK3,
		.active_low	= 1,
	},  
};

static void __init MY-TENDA-O3_V2_gpio_led_setup(void)
{
	ath79_gpio_direction_select(MY-TENDA-O3_V2_GPIO_GPIO_LED_LAN1, true);
	ath79_gpio_direction_select(MY-TENDA-O3_V2_GPIO_GPIO_LED_WLAN, true);
	ath79_gpio_direction_select(MY-TENDA-O3_V2_GPIO_GPIO_LED_LINK1, true);
	ath79_gpio_direction_select(MY-TENDA-O3_V2_GPIO_GPIO_LED_LINK2, true);
	ath79_gpio_direction_select(MY-TENDA-O3_V2_GPIO_GPIO_LED_LINK3, true);

		/* Mute LEDs on boot */
	gpio_set_value(MY-TENDA-O3_V2_GPIO_LED_WLAN, 1);
	gpio_set_value(MY-TENDA-O3_V2_GPIO_LED_LAN1, 1);
	gpio_set_value(MY-TENDA-O3_V2_GPIO_LED_LINK1, 1);
	gpio_set_value(MY-TENDA-O3_V2_GPIO_LED_LINK2, 1);
	gpio_set_value(MY-TENDA-O3_V2_GPIO_LED_LINK3, 1);

	ath79_gpio_output_select(MY-TENDA-O3_V2_GPIO_LED_WLAN, 0);
	ath79_gpio_output_select(MY-TENDA-O3_V2_GPIO_LED_LAN1, 0);
	ath79_gpio_output_select(MY-TENDA-O3_V2_GPIO_LED_LINK1, 0);
	ath79_gpio_output_select(MY-TENDA-O3_V2_GPIO_LED_LINK2, 0);
	ath79_gpio_output_select(MY-TENDA-O3_V2_GPIO_LED_LINK3, 0);
	
	ath79_register_leds_gpio(-1, ARRAY_SIZE(my-tenda-o3-v2_leds_gpio),
			my-tenda-o3-v2_leds_gpio);
}

static void __init my-tenda-o3-v2_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);

	/* Disable JTAG (enables GPIO0-3) */
	ath79_gpio_function_enable(AR934X_GPIO_FUNC_JTAG_DISABLE);

	ath79_register_gpio_keys_polled(-1, my-tenda-o3-v2_KEYS_POLL_INTERVAL,
			ARRAY_SIZE(my-tenda-o3-v2_gpio_keys),
			my-tenda-o3-v2_gpio_keys);

	my-tenda-o3-v2_gpio_led_setup();

	//ath79_register_usb();

	ath79_wmac_set_led_pin(MY-TENDA-O3_V2_GPIO_LED_WLAN);
	ath79_register_wmac(art + MY-TENDA-O3_V2_WMAC_CALDATA_OFFSET, NULL);

	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + MY-TENDA-O3_V2_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + MY-TENDA-O3_V2_MAC1_OFFSET, 0);

	/* WAN port */
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.speed = SPEED_100;
	ath79_eth0_data.duplex = DUPLEX_FULL;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_register_eth(0);

	/* LAN ports */
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;
	ath79_switch_data.phy_poll_mask |= BIT(4);
	ath79_switch_data.phy4_mii_en = 1;
	ath79_register_eth(1);
}

MIPS_MACHINE(ATH79_MACH_MY-TENDA-O3_V2, "MY-TENDA-O3_V2", "MY-TENDA-O3_V2 reference board",
	     MY-TENDA-O3_V2_setup);
