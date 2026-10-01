/* SPDX-License-Identifier: GPL-2.0-only */
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <mt-plat/mt_gpio.h>
#include "lcm_drv.h"
#include "disp_dts_gpio.h"

enum u10_action_kind { U10_DELAY, U10_RESET, U10_BIAS, U10_DCS, U10_QUEUE };
struct u10_panel_action {
	unsigned int kind, value, count;
	unsigned char data[64];
	unsigned int words[16];
};
struct u10_panel_profile {
	LCM_PARAMS params;
	const struct u10_panel_action *init, *suspend;
	unsigned int init_count, suspend_count;
	unsigned int id_command, id_count, id_offset, id_value;
	int module_id;
	unsigned int identify_reset_delay;
};

#include <u10_panel_resources.h>

static LCM_UTIL_FUNCS u10_util;
static const struct u10_panel_profile *u10_profile = &u10_profiles[U10_PANEL_INDEX];

static void u10_set_util(const LCM_UTIL_FUNCS *util)
{
	memcpy(&u10_util, util, sizeof(u10_util));
}

static void u10_get_params(LCM_PARAMS *params)
{
	/* The resource uses this tree's named fields, never a stock ABI blob. */
	*params = u10_profile->params;
}

static bool u10_run(const struct u10_panel_action *actions, unsigned int count)
{
	unsigned int i;
	for (i = 0; i < count; i++) {
		const struct u10_panel_action *a = &actions[i];
		switch (a->kind) {
		case U10_DELAY:
			(u10_util.mdelay)(a->value);
			break;
		case U10_RESET:
			u10_util.set_reset_pin(a->value);
			break;
		case U10_BIAS:
			if (disp_dts_gpio_select_state(a->value ?
				DTS_GPIO_STATE_LCD_BIAS_ENP : DTS_GPIO_STATE_LCD_BIAS_ENN))
				return false;
			break;
		case U10_DCS:
			u10_util.dsi_set_cmdq_V2(a->value, a->count,
				(unsigned char *)a->data, 1);
			break;
		case U10_QUEUE:
			u10_util.dsi_set_cmdq((unsigned int *)a->words, a->count, 1);
			break;
		default:
			return false;
		}
	}
	return true;
}

static int u10_strap(unsigned long pin)
{
	int low, high, state;
	if (mt_set_gpio_mode(pin, GPIO_MODE_00) ||
	    mt_set_gpio_dir(pin, GPIO_DIR_IN) ||
	    mt_set_gpio_pull_enable(pin, GPIO_PULL_ENABLE) ||
	    mt_set_gpio_pull_select(pin, GPIO_PULL_DOWN))
		return -1;
	(u10_util.mdelay)(100);
	low = mt_get_gpio_in(pin);
	if (mt_set_gpio_pull_select(pin, GPIO_PULL_UP))
		return -1;
	(u10_util.mdelay)(100);
	high = mt_get_gpio_in(pin);
	state = low == high ? (low == 0 ? 0 : low == 1 ? 1 : 3) : 2;
	if (mt_set_gpio_pull_select(pin, state == 1 ? GPIO_PULL_UP : GPIO_PULL_DOWN))
		return -1;
	return state;
}

static unsigned int u10_compare_id(void)
{
	unsigned char id[4] = { 0 };
	unsigned int queue, value;
	int id0, id1;
	if (disp_dts_gpio_select_state(DTS_GPIO_STATE_LCD_BIAS_ENP))
		return 0;
	(u10_util.mdelay)(10);
	u10_util.set_reset_pin(1);
	if (u10_profile->identify_reset_delay)
		(u10_util.mdelay)(10);
	u10_util.set_reset_pin(0);
	(u10_util.mdelay)(u10_profile->identify_reset_delay ? 10 : 1);
	u10_util.set_reset_pin(1);
	(u10_util.mdelay)(150);
	queue = (u10_profile->id_command == 4 ? 0x00033700 : 0x00023700);
	u10_util.dsi_set_cmdq(&queue, 1, 1);
	u10_util.dsi_dcs_read_lcm_reg_v2(u10_profile->id_command, id,
		u10_profile->id_count);
	value = ((unsigned int)id[u10_profile->id_offset] << 8) |
		id[u10_profile->id_offset + 1];
	if (value != u10_profile->id_value)
		return 0;
	if (u10_profile->module_id < 0)
		return 1;
	id0 = u10_strap(82UL | 0x80000000UL);
	id1 = u10_strap(81UL | 0x80000000UL);
	return id0 >= 0 && id1 >= 0 &&
		(id0 | (id1 << 2)) == u10_profile->module_id;
}

static void u10_init(void)
{
	if (!u10_run(u10_profile->init, u10_profile->init_count))
		pr_err("U10 panel initialization resource failed\n");
}

static void u10_suspend(void)
{
	if (!u10_run(u10_profile->suspend, u10_profile->suspend_count))
		pr_err("U10 panel suspend resource failed\n");
}

LCM_DRIVER U10_PANEL_SYMBOL = {
	.name = U10_PANEL_NAME,
	.set_util_funcs = u10_set_util,
	.get_params = u10_get_params,
	.init = u10_init,
	.suspend = u10_suspend,
	.resume = u10_init,
	.compare_id = u10_compare_id,
};
