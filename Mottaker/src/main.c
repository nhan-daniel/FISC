/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/*Koden har tatt utgangspunkt fra Lesson 4, oppgave 1 i Cellular IoT Fundamentals i DevAcademy*/

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
#include <zephyr/net/mqtt.h>
#include "mqtt_connection.h"

//GPIO-nummer for utgangene som skal brukes
#define INPUT_PIN 15
#define INPUT_PIN2 16

//Henter GPIO-enheten (gpio0)
#define INPUT_NODE DT_NODELABEL(gpio0)

//Henter LED-definisjoner fra device tree
#define LED0_NODE DT_ALIAS(led0)
#define LED2_NODE DT_ALIAS(led2)

//Referanser til GPIO og LEDs
const struct device *gpio_dev = DEVICE_DT_GET(INPUT_NODE);
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

//Tre forskjellige timere for å blinking og lyd
static struct k_timer blink_timer_lys;
static struct k_timer blink_timer_alarm;
static struct k_timer blink_timer_diode;

//Timer-handler for LED3
static void blink_timer_handler_lys(struct k_timer *timer)
{
    gpio_pin_toggle_dt(&led2);
}
//Timer-handler for GPIO-pin 15
static void blink_timer_handler_alarm(struct k_timer *timer)
{
	gpio_pin_toggle(gpio_dev, INPUT_PIN);

}
//Timer-handler for GPIO-pin 16
static void blink_timer_handler_diode(struct k_timer *timer)
{
	gpio_pin_toggle(gpio_dev, INPUT_PIN2);

}

//Funksjon som sjekker om alarm er aktivert
static bool blinking = false;
static void check_alarm(){

	int input_state = gpio_pin_get_dt(&led0);
	
	printk("Input state: %d\n", input_state);

    if (input_state == 1 && !blinking) {
        blinking = true;

		//Starter GPIO-pin 15 og 16 høye
		int ret = gpio_pin_set(gpio_dev, INPUT_PIN, 1);
		if (ret < 0) {
			printk("Failed to set pin %d high (ret = %d)\n", INPUT_PIN, ret);
		}
		int ret2 = gpio_pin_set(gpio_dev, INPUT_PIN2, 1);
		if (ret2 < 0) {
			printk("Failed to set pin %d high (ret = %d)\n", INPUT_PIN2, ret2);
		}

		//Starter alarm, lyd og lys
        k_timer_start(&blink_timer_alarm, K_USEC(100), K_USEC(100));
		k_timer_start(&blink_timer_lys, K_MSEC(300), K_MSEC(300));
		k_timer_start(&blink_timer_diode, K_MSEC(300), K_MSEC(300));

        printk("Input is HIGH: starting LED blink\n");
    } else if (input_state == 0 && blinking) {
        blinking = false;

		//Setter GPIO-pin 15, 16 og LED3 lave
		gpio_pin_set(gpio_dev, INPUT_PIN, 0);
		gpio_pin_set(gpio_dev, INPUT_PIN2, 0);
		gpio_pin_set_dt(&led2, 0);

		//Sjekker for feil
		int ret = gpio_pin_set(gpio_dev, INPUT_PIN, 0);
		if (ret > 0) {
			printk("Failed to set pin %d low (ret = %d)\n", INPUT_PIN, ret);
		}

		int ret2 = gpio_pin_set(gpio_dev, INPUT_PIN2, 0);
		if (ret2 > 0) {
			printk("Failed to set pin %d low (ret = %d)\n", INPUT_PIN2, ret2);
		}

		//Stopper timere
        k_timer_stop(&blink_timer_lys);
		k_timer_stop(&blink_timer_alarm);
		k_timer_stop(&blink_timer_diode);
        printk("Input is LOW: stopping LED blink\n");
    }
}


//MQTT-klient og poll-struktur
static struct mqtt_client client;
static struct pollfd fds;

//Semaphore som venter på at LTE er tilkoblet
static K_SEM_DEFINE(lte_connected, 0, 1);

//Aktiverer logging
LOG_MODULE_REGISTER(Lesson4_Exercise1, LOG_LEVEL_INF);

//Håndterer LTE-hendelser (f.eks. tilkobling)
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

//Initierer modem og LTE-tilkobling
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
	
	//Venter på tilkobling
	k_sem_take(&lte_connected, K_FOREVER);
	LOG_INF("Connected to LTE network");
	//Slår på LED2 som indikerer tilkobling
	dk_set_led_on(DK_LED2); 

	return 0;
}


//Knappetrykk: for å testing
static void button_handler(uint32_t button_state, uint32_t has_changed)
{
	//Knapp som starter alarmen
    if (has_changed & DK_BTN1_MSK) {
        if (button_state & DK_BTN1_MSK) {    
            int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
                                   "LED1ON", sizeof("LED1ON") - 1); 
            if (err) {
                LOG_INF("Failed to send message, %d", err);
                return;  
            }
        }
    }

	//Knapp som stopper og restarter alarmen
    if (has_changed & DK_BTN2_MSK) {
        if (button_state & DK_BTN2_MSK) {    
            int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
                                   "LED1OFF", sizeof("LED1OFF") - 1); 
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

	// Initialiserer LED, modem, knapper
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

	//Initialiserer MQTT
	err = client_init(&client);
	if (err) {
		LOG_ERR("Failed to initialize MQTT client: %d", err);
		return 0;
	}

	//Konfigurer GPIO som utganger
	gpio_pin_configure(gpio_dev, INPUT_PIN, GPIO_OUTPUT_INACTIVE);	
	gpio_pin_configure(gpio_dev, INPUT_PIN2, GPIO_OUTPUT_INACTIVE);

//Forsøker å koble til MQTT-server
do_connect:
	//Forsinkelse ved reconnecting
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

	//Konfigurer LED1
	gpio_pin_configure_dt(&led0, GPIO_OUTPUT | GPIO_INPUT);

	//Initialiserer timere
	k_timer_init(&blink_timer_alarm, blink_timer_handler_alarm, NULL);
	k_timer_init(&blink_timer_lys, blink_timer_handler_lys, NULL);
	k_timer_init(&blink_timer_diode, blink_timer_handler_diode, NULL);

	//Hovedløkke
	while (1) {
		err = poll(&fds, 1, 5000);
		if (err < 0) {
			LOG_ERR("Error in poll(): %d", errno);
			break;
		}

		//Oppdater MQTT-klient
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
		//Sjekk om alarmen skal utløses
		check_alarm();
	}

	//Koble fra MQTT hvis løkka avsluttes
	LOG_INF("Disconnecting MQTT client");
	err = mqtt_disconnect(&client);
	if (err) {
		LOG_ERR("Could not disconnect MQTT client: %d", err);
	}

	//Prøver å koble til på nytt
	goto do_connect;


	return 0;
}
