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

//GPIO-nummer for utgangen som skal brukes
#define INPUT_PIN 15

//Henter GPIO-enheten (gpio0)
#define INPUT_NODE DT_NODELABEL(gpio0)


#define _USE_MATH_DEFINES

//Referanser til GPIO
const struct device *gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

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

//Definerer variabler til posisjons simulering
bool if_checked = false;
bool led1_checked = false;

#define LOOP_RADIUS 30.0
#define CENTER_LAT 63.4575
#define CENTER_LON 10.3890


static double t = 0.0;

// Konverterer meter til grader (latitude og longitude)
double meters_to_degrees_lat(double meters) {
    return meters / 111320.0;
}

double meters_to_degrees_lon(double meters, double lat) {
    return meters / (40075000.0 * cos(lat * 3.14 / 180.0) / 360.0);
}

//Simulerer posisjon i en uendelig sløyfe
void generate_infinity_position(double *lat_out, double *lon_out) {
    t += 0.05;

    double x = sin(t);
    double y = sin(t) * cos(t);

    double dx = x * LOOP_RADIUS;
    double dy = y * LOOP_RADIUS;

    double delta_lat = meters_to_degrees_lat(dy);
    double delta_lon = meters_to_degrees_lon(dx, CENTER_LAT);

    *lat_out = CENTER_LAT + delta_lat;
    *lon_out = CENTER_LON + delta_lon;
}

//Sjekker inngangsstatus og publiserer posisjon
static void check_input(void) {
    int input_state = gpio_pin_get(gpio_dev, INPUT_PIN);

    printk("Pin P0.15 state: %d\n", input_state);

	if (input_state == 0 && led1_checked) {
		led1_checked = false;
	}

    // Hvis input er 0 – send posisjoner kontinuerlig
    if (input_state == 1) {
        double lat, lon;
        generate_infinity_position(&lat, &lon);

		if (!led1_checked){
			led1_checked = true;
			int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
				CONFIG_BUTTON_EVENT_PUBLISH_MSG, sizeof(CONFIG_BUTTON_EVENT_PUBLISH_MSG)-1);
			if (err) {
				LOG_INF("Failed to send message, %d", err);
				return;	
			}
		}

		//Lager en JSON-streng for posisjonen
        char json_str[100];
        int lat_i = (int)(lat * 1e6);
		int lon_i = (int)(lon * 1e6);

		snprintf(json_str, sizeof(json_str),
        	 "{\"lat\": %d.%06d, \"lon\": %d.%06d}",
         	lat_i / 1000000, abs(lat_i % 1000000),
         	lon_i / 1000000, abs(lon_i % 1000000));

        int err = data_publish(&client, MQTT_QOS_1_AT_LEAST_ONCE,
                               json_str, strlen(json_str));

        if (err) {
            LOG_INF("Feil ved sending av posisjon: %d", err);
        } else {
            LOG_INF("Sendte ∞-posisjon: %s", json_str);
        }
		k_sleep(K_MSEC(2000));
    }
	
}

int main(void)
{
	int err;
	uint32_t connect_attempt = 0;

	//Initialiserer LED, modem, knapper
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
        printk("GPIO device not ready\n");
        return 0;
    }

	//Initialiserer MQTT
	err = client_init(&client);
	if (err) {
		LOG_ERR("Failed to initialize MQTT client: %d", err);
		return 0;
	}
	//Konfigurer GPIO som utganger
	gpio_pin_configure(gpio_dev, INPUT_PIN, GPIO_INPUT | GPIO_PULL_UP);

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

		//Sjekker om posisjoner skal sendes
		check_input();
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
