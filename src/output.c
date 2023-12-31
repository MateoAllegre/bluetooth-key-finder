#include <errno.h>
#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <stdbool.h>
#include <zephyr/types.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#include "output.h"

#define LOG_LEVEL CONFIG_BT_OUTPUT_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(output);

static bool led_state = false;
static int buzzer_value = 0;

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0});

// En l'absence de buzzer, la led 0 fera office de buzzer (en PWM)
static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

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

    // Controle de la LED en fonction de led_state
    gpio_pin_set_dt(&led, led_state);

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

    // Controle du buzzer en fonction de buzzer_value
	//printk("%d", buzzer_value);
    pwm_set_dt(&pwm_led0, buzzer_value, buzzer_value / 2);

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
    int ret;

    if (led.port && !device_is_ready(led.port)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}
	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}

    if (!device_is_ready(pwm_led0.dev)) {
		printk("Error: PWM device %s is not ready\n",
		       pwm_led0.dev->name);
		return 0;
	}

    led_state = false;
    gpio_pin_set_dt(&led, led_state);

    buzzer_value = 0U;
    pwm_set_dt(&pwm_led0, buzzer_value, buzzer_value / 2);

    return ret;
}

void output_set_led(bool state)
{
    led_state = state;

    gpio_pin_set_dt(&led, led_state);
}

void output_set_buzzer(int value)
{
    buzzer_value = value;

    pwm_set_dt(&pwm_led0, buzzer_value, buzzer_value / 2);
}

SYS_INIT(output_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);