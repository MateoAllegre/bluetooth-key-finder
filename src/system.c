#include <errno.h>
#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <string.h>
#include <zephyr/types.h>
#include <stddef.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>

#include "system.h"

#define LOG_LEVEL CONFIG_BT_SYSTEM_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(system);

static const char system_version[] = "1.4.2.3";
static char system_debug[] = "Generic error message goes here";

static ssize_t system_read_version(struct bt_conn *conn,
                            const struct bt_gatt_attr *attr, void *buf,
                            uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             system_version, sizeof(system_version));
}

static ssize_t system_read_debug(struct bt_conn *conn,
                          const struct bt_gatt_attr *attr, void *buf,
                          uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             system_debug, sizeof(system_debug));
}

static ssize_t system_notify_debug(struct bt_conn *conn,
                            const struct bt_gatt_attr *attr, void *buf,
                            uint16_t len, uint16_t offset)
{
    // Notify current debug message
    return bt_gatt_notify(NULL, attr, system_debug, sizeof(system_debug));
}

BT_GATT_SERVICE_DEFINE(system,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_16(0x1401)),
                       BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x1404),
                                              BT_GATT_CHRC_READ, BT_GATT_PERM_READ,
                                              system_read_version, NULL, NULL),
                       BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_16(0x1408),
                                              BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                                              BT_GATT_PERM_READ, system_read_debug,
                                              NULL, system_notify_debug),
);

int system_init(void)
{
    return 0;
}

void system_set_debug(const char *debug_message)
{
    // Update debug message
    strncpy(system_debug, debug_message, sizeof(system_debug));
}

SYS_INIT(system_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);