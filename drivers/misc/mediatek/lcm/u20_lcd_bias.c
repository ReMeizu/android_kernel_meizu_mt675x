/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include "u20_lcd_bias.h"

static DEFINE_MUTEX(bias_lock);
static struct i2c_client *bias_client;

int u20_lcd_bias_write(unsigned char reg, unsigned char value)
{
	unsigned char data[2] = { reg, value };
	int rc;

	mutex_lock(&bias_lock);
	if (!bias_client) {
		rc = -ENODEV;
	} else {
		rc = i2c_master_send(bias_client, data, sizeof(data));
		if (rc >= 0)
			rc = rc == sizeof(data) ? 0 : -EIO;
	}
	mutex_unlock(&bias_lock);

	return rc;
}

static int bias_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int rc = 0;

	if (client->adapter->nr != 0 || client->addr != 0x3e)
		return -ENODEV;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -EOPNOTSUPP;

	mutex_lock(&bias_lock);
	if (bias_client)
		rc = -EBUSY;
	else
		bias_client = client;
	mutex_unlock(&bias_lock);

	return rc;
}

static int bias_remove(struct i2c_client *client)
{
	mutex_lock(&bias_lock);
	if (bias_client == client)
		bias_client = NULL;
	mutex_unlock(&bias_lock);

	return 0;
}

static const struct of_device_id bias_of_match[] = {
	{ .compatible = "mediatek,i2c_lcd_bias" },
	{ }
};
MODULE_DEVICE_TABLE(of, bias_of_match);

static const struct i2c_device_id bias_id[] = {
	{ "u20-lcd-bias", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, bias_id);

static struct i2c_driver bias_driver = {
	.driver = {
		.name = "u20-lcd-bias",
		.of_match_table = bias_of_match
	},
	.probe = bias_probe,
	.remove = bias_remove,
	.id_table = bias_id,
};
module_i2c_driver(bias_driver);
MODULE_LICENSE("GPL");
