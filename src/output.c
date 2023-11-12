#include <errno.h>
#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <stdbool.h>
#include <zephyr/types.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

#include "output.h"

#define LOG_LEVEL CONFIG_BT_OUTPUT_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(output);

static bool led_state = false;
static int buzzer_value = 0;

static ssize_t write_led(struct bt_conn *conn,
                         const struct bt_gatt_attr *attr,
                         const void *buf, uint16_t len, uint16_t offset)
{
    if (len != sizeof(bool))
    {
        printk("error");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }

    led_state = *((bool *)buf);

    // TODO Code pour contrôler la LED en fonction de led_state
	printk("%d", led_state);

    return len;
}

static ssize_t write_buzzer(struct bt_conn *conn,
                            const struct bt_gatt_attr *attr,
                            const void *buf, uint16_t len, uint16_t offset)
{
    if (len != sizeof(int))
    {
        printk("error");
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }

    buzzer_value = *((int *)buf);

    // TODO Code pour contrôler le buzzer en fonction de buzzer_value
	printk("%d", buzzer_value);

    return len;
}

BT_GATT_SERVICE_DEFINE(output,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(0x1403)),
                       BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x1406),
                                              BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE,
                                              NULL, write_buzzer, NULL),
                       BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x1407),
                                              BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE,
                                              NULL, write_led, NULL),
);

int output_init(void)
{
    return 0;
}

void output_set_led(bool state)
{
    led_state = state;

    // TODO: Code pour contrôler la LED en fonction de led_state
}

void output_set_buzzer(int value)
{
    buzzer_value = value;

    // TODO: Code pour contrôler le buzzer en fonction de buzzer_value
}

SYS_INIT(output_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);