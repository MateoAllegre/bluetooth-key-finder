/** @file
 *  @brief Custom Input Service
 */

#include <errno.h>
#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <stdbool.h>
#include <zephyr/types.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

#include "input.h"

#define LOG_LEVEL CONFIG_BT_INPUT_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(input);

static uint8_t button_pressed = 0;

static void blvl_ccc_cfg_changed(const struct bt_gatt_attr *attr,
				       uint16_t value)
{
	ARG_UNUSED(attr);

	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("Button Notifications %s", notif_enabled ? "enabled" : "disabled");
}

static ssize_t read_blvl(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	uint8_t lvl8 = button_pressed;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &lvl8,
				 sizeof(lvl8));
}

BT_GATT_SERVICE_DEFINE(input,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(0x1402)),
	BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x1405),
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, read_blvl, NULL,
			       &button_pressed),
	BT_GATT_CCC(blvl_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

static int input_init(void)
{
	return 0;
}

uint8_t bt_input_get_button(void)
{
	return button_pressed;
}

int bt_input_set_button(uint8_t pressed)
{
	int rc;

	if (pressed > 1U) {
		return -EINVAL;
	}

	button_pressed = pressed;

	rc = bt_gatt_notify(NULL, &input.attrs[1], &pressed, sizeof(pressed));

	return rc == -ENOTCONN ? 0 : rc;
}

SYS_INIT(input_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
