/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>
#include <ncs_version.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include <modem/nrf_modem_lib.h>
#include <modem/lte_lc.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>

#include <math.h>


/* STEP 2.3 - Include the header file for the MQTT Library*/
#include <zephyr/net/mqtt.h>

#include "mqtt_connection.h"

#define INPUT_PIN 15

#define INPUT_NODE DT_NODELABEL(gpio0)

#define LED0_NODE DT_ALIAS(led0)

#define LED2_NODE DT_ALIAS(led2)

const struct device *gpio_dev = DEVICE_DT_GET(INPUT_NODE);

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);


// We'll use a Zephyr timer to blink the LED
static struct k_timer blink_timer;
static bool blinking = false;

/* Timer callback that toggles the LED */
static void blink_timer_handler(struct k_timer *timer)
{
    gpio_pin_toggle_dt(&led2);
}

static void check_alarm(){

	int input_state = gpio_pin_get_dt(&led0);
	
	printk("Input state: %d\n", input_state);

    if (input_state == 1 && !blinking) {
        blinking = true;
		gpio_pin_set(gpio_dev, INPUT_PIN, 1);
		int ret = gpio_pin_set(gpio_dev, INPUT_PIN, 1);
		if (ret < 0) {
			printk("Failed to set pin %d high (ret = %d)\n", INPUT_PIN, ret);
		}
        k_timer_start(&blink_timer, K_MSEC(500), K_MSEC(500));
        printk("Input is HIGH: starting LED blink\n");
    } else if (input_state == 0 && blinking) {
        blinking = false;
        k_timer_stop(&blink_timer);
        // Turn LED off (or set to 1 if it's active-low)
        gpio_pin_set_dt(&led2, 0);
        printk("Input is LOW: stopping LED blink\n");
    }
}


/* The mqtt client struct */
static struct mqtt_client client;
/* File descriptor */
static struct pollfd fds;

static K_SEM_DEFINE(lte_connected, 0, 1);

LOG_MODULE_REGISTER(Lesson4_Exercise1, LOG_LEVEL_INF);

static void lte_handler(const struct lte_lc_evt *const evt)
{
     switch (evt->type) {
     case LTE_LC_EVT_NW_REG_STATUS:
        if ((evt->nw_reg_status != LTE_LC_NW_REG_REGISTERED_HOME) &&
            (evt->nw_reg_status != LTE_LC_NW_REG_REGISTERED_ROAMING)) {
            break;
        }
		LOG_INF("Network registration status: %s",
				evt->nw_reg_status == LTE_LC_NW_REG_REGISTERED_HOME ?
				"Connected - home network" : "Connected - roaming");
		k_sem_give(&lte_connected);
        break;
	case LTE_LC_EVT_RRC_UPDATE:
		LOG_INF("RRC mode: %s", evt->rrc_mode == LTE_LC_RRC_MODE_CONNECTED ?
				"Connected" : "Idle");
		break;
     default:
             break;
     }
}

static int modem_configure(void)
{
	int err;

	LOG_INF("Initializing modem library");
	err = nrf_modem_lib_init();
	if (err) {
		LOG_ERR("Failed to initialize the modem library, error: %d", err);
		return err;
	}
	
	LOG_INF("Connecting to LTE network");
	err = lte_lc_connect_async(lte_handler);
	if (err) {
		LOG_ERR("Error in lte_lc_connect_async, error: %d", err);
		return err;
	}

	k_sem_take(&lte_connected, K_FOREVER);
	LOG_INF("Connected to LTE network");
	dk_set_led_on(DK_LED2);

	return 0;
}

/*static void button_handler(uint32_t button_state, uint32_t has_changed)
{
	switch (has_changed) {
	case DK_BTN1_MSK:
		if (button_state & DK_BTN1_MSK){	
			int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				   CONFIG_BUTTON_EVENT_PUBLISH_MSG, sizeof(CONFIG_BUTTON_EVENT_PUBLISH_MSG)-1);
			if (err) {
				LOG_INF("Failed to send message, %d", err);
				return;	
			}
		}
		break;
	}
}
*/

static void button_handler(uint32_t button_state, uint32_t has_changed)
{
    // Håndter Button 1
    if (has_changed & DK_BTN1_MSK) {
        if (button_state & DK_BTN1_MSK) {    
            int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
                                   "LED1ON", sizeof("LED1ON") - 1);  // Send "LED1ON" meldingen
            if (err) {
                LOG_INF("Failed to send message, %d", err);
                return;  
            }
        }
    }

    // Håndter Button 2
    if (has_changed & DK_BTN2_MSK) {
        if (button_state & DK_BTN2_MSK) {    
            int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
                                   "LED1OFF", sizeof("LED1OFF") - 1);  // Send "LED1OFF" meldingen
            if (err) {
                LOG_INF("Failed to send message, %d", err);
                return;  
            }
        }
    }
}





int main(void)
{
	int err;
	uint32_t connect_attempt = 0;

	if (dk_leds_init() != 0) {
		LOG_ERR("Failed to initialize the LED library");
	}

	err = modem_configure();
	if (err) {
		LOG_ERR("Failed to configure the modem");
		return 0;
	}

	if (dk_buttons_init(button_handler) != 0) {
		LOG_ERR("Failed to initialize the buttons library");
	}

	if (!device_is_ready(gpio_dev)) {
		printk("gpio0 device not ready\n");
		return 0;
	}
	gpio_pin_configure(gpio_dev, INPUT_PIN, GPIO_OUTPUT_INACTIVE);	

	err = client_init(&client);
	if (err) {
		LOG_ERR("Failed to initialize MQTT client: %d", err);
		return 0;
	}


do_connect:
	if (connect_attempt++ > 0) {
		LOG_INF("Reconnecting in %d seconds...",
			CONFIG_MQTT_RECONNECT_DELAY_S);
		k_sleep(K_SECONDS(CONFIG_MQTT_RECONNECT_DELAY_S));
	}
	err = mqtt_connect(&client);
	if (err) {
		LOG_ERR("Error in mqtt_connect: %d", err);
		goto do_connect;
	}

	err = fds_init(&client,&fds);
	if (err) {
		LOG_ERR("Error in fds_init: %d", err);
		return 0;
	}

	gpio_pin_configure_dt(&led0, GPIO_OUTPUT | GPIO_INPUT);

	k_timer_init(&blink_timer, blink_timer_handler, NULL);
	

	while (1) {
		//mqtt_keepalive_time_left(&client)
		err = poll(&fds, 1, 5000);
		if (err < 0) {
			LOG_ERR("Error in poll(): %d", errno);
			break;
		}

		err = mqtt_live(&client);
		if ((err != 0) && (err != -EAGAIN)) {
			LOG_ERR("Error in mqtt_live: %d", err);
			break;
		}

		if ((fds.revents & POLLIN) == POLLIN) {
			err = mqtt_input(&client);
			if (err != 0) {
				LOG_ERR("Error in mqtt_input: %d", err);
				break;
			}
		}

		if ((fds.revents & POLLERR) == POLLERR) {
			LOG_ERR("POLLERR");
			break;
		}

		if ((fds.revents & POLLNVAL) == POLLNVAL) {
			LOG_ERR("POLLNVAL");
			break;
		}
		check_alarm();
	}

	LOG_INF("Disconnecting MQTT client");

	err = mqtt_disconnect(&client);
	if (err) {
		LOG_ERR("Could not disconnect MQTT client: %d", err);
	}
	goto do_connect;

	/* This is never reached */
	return 0;
}
