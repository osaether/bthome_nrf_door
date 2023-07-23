/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/drivers/gpio.h>

#define SERVICE_UUID            0xfcd2		// BTHome service UUID

#define DOOR_PIN1               2
#define DOOR_PIN2               26

static const struct device *gpio_port0_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));


#define ADV_PARAM BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY, \
					     BT_GAP_ADV_SLOW_INT_MIN, \
					     BT_GAP_ADV_SLOW_INT_MAX, NULL)

static uint8_t service_data[] = { 
	BT_UUID_16_ENCODE(SERVICE_UUID),
	0x40,
	0x1A,	// Door
	0x00
};

static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
	BT_DATA(BT_DATA_SVC_DATA16, service_data, ARRAY_SIZE(service_data))
};

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	/* Start advertising */
	err = bt_le_adv_start(ADV_PARAM, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}
}

void init(void)
{
	int err;

	gpio_pin_configure(gpio_port0_dev, DOOR_PIN1, GPIO_INPUT | GPIO_PULL_UP);
	gpio_pin_configure(gpio_port0_dev, DOOR_PIN2, GPIO_OUTPUT);
	
	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
}

void main(void)
{
	int err;

	printk("Starting BTHome door sensor\n");

	init();

	for (;;) {
		gpio_pin_set(gpio_port0_dev, DOOR_PIN2, 0);
		k_sleep(K_MSEC(5));
		int door_state = gpio_pin_get(gpio_port0_dev, DOOR_PIN1);
		gpio_pin_set(gpio_port0_dev, DOOR_PIN2, 1);
		service_data[4] = door_state & 0xff;
		err = bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL, 0);
		if (err) {
			printk("Failed to update advertising data (err %d)\n", err);
		}
		k_sleep(K_MSEC(BT_GAP_ADV_FAST_INT_MAX_2));
	}
}
