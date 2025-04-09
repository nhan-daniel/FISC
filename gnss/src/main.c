#include <modem/lte_lc.h>
#include <modem/nrf_modem_lib.h>
#include <nrf_modem_gnss.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>

#define SERVER_HOSTNAME "udp-echo.nordicsemi.academy"
#define SERVER_PORT	"2444"
#define MESSAGE_SIZE	256

LOG_MODULE_REGISTER(Lesson6_Exercise2, LOG_LEVEL_INF);

static struct nrf_modem_gnss_pvt_data_frame pvt_data;
static int64_t gnss_start_time;
static bool first_fix = false;
static uint8_t gps_data[MESSAGE_SIZE];
static int sock;
static struct sockaddr_storage server;
static uint8_t recv_buf[MESSAGE_SIZE];
static K_SEM_DEFINE(lte_connected, 0, 1);

static int server_resolve(void)
{
	struct addrinfo *result;
	struct addrinfo hints = {.ai_family = AF_INET, .ai_socktype = SOCK_DGRAM};
	int err = getaddrinfo(SERVER_HOSTNAME, SERVER_PORT, &hints, &result);

	if (err != 0) {
		LOG_INF("%d", err);
		return err;
	}
	if (result == NULL) {
		LOG_INF("ERROR: Address not found");
		return -ENOENT;
	}

	struct sockaddr_in *server4 = ((struct sockaddr_in *)&server);
	server4->sin_addr.s_addr = ((struct sockaddr_in *)result->ai_addr)->sin_addr.s_addr;
	server4->sin_family = AF_INET;
	server4->sin_port = ((struct sockaddr_in *)result->ai_addr)->sin_port;

	char ipv4_addr[NET_IPV4_ADDR_LEN];
	inet_ntop(AF_INET, &server4->sin_addr.s_addr, ipv4_addr, sizeof(ipv4_addr));
	LOG_INF("IPv4 Address found %s", ipv4_addr);

	freeaddrinfo(result);
	return 0;
}

static int server_connect(void)
{
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		LOG_ERR("Failed to create socket: %d.", errno);
		return -errno;
	}

	int err = connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	if (err < 0) {
		LOG_ERR("Connect failed : %d", errno);
		return -errno;
	}
	LOG_INF("Successfully connected to server");
	return 0;
}

static void lte_handler(const struct lte_lc_evt *const evt)
{
	switch (evt->type) {
	case LTE_LC_EVT_NW_REG_STATUS:
		if ((evt->nw_reg_status == LTE_LC_NW_REG_REGISTERED_HOME) ||
		    (evt->nw_reg_status == LTE_LC_NW_REG_REGISTERED_ROAMING)) {
			LOG_INF("Network registration status: %s",
				evt->nw_reg_status == LTE_LC_NW_REG_REGISTERED_HOME
					? "Connected - home network"
					: "Connected - roaming");
			k_sem_give(&lte_connected);
		}
		break;

	case LTE_LC_EVT_RRC_UPDATE:
		LOG_INF("RRC mode: %s",
			evt->rrc_mode == LTE_LC_RRC_MODE_CONNECTED ? "Connected" : "Idle");
		break;
	case LTE_LC_EVT_PSM_UPDATE:
		LOG_INF("PSM parameter update: Periodic TAU: %d s, Active time: %d s",
			evt->psm_cfg.tau, evt->psm_cfg.active_time);
		if (evt->psm_cfg.active_time == -1) {
			LOG_ERR("Network rejected PSM parameters. Failed to enable PSM");
		}
		break;
	case LTE_LC_EVT_EDRX_UPDATE:
		LOG_INF("eDRX parameter update: eDRX: %f, PTW: %f", (double)evt->edrx_cfg.edrx,
			(double)evt->edrx_cfg.ptw);
		break;
	default:
		break;
	}
}

static void print_and_send_fix_data(struct nrf_modem_gnss_pvt_data_frame *pvt_data)
{
	LOG_INF("Latitude: %.06f", pvt_data->latitude);
	LOG_INF("Longitude: %.06f", pvt_data->longitude);

	int data_len = snprintf((char *)gps_data, MESSAGE_SIZE, "Latitude: %.06f, Longitude: %.06f",
				pvt_data->latitude, pvt_data->longitude);
	int err = send(sock, gps_data, data_len, 0);
	if (err < 0) {
		LOG_ERR("Failed to send GPS data: %d", errno);
	} else {
		LOG_INF("GPS data sent successfully");
	}
}

static void gnss_event_handler(int event)
{
	int num_satellites;

	switch (event) {
	case NRF_MODEM_GNSS_EVT_PVT:
		num_satellites = 0;
		for (int i = 0; i < 12; i++) {
			if (pvt_data.sv[i].signal != 0) {
				num_satellites++;
			}
		}
		LOG_INF("Searching. Current satellites: %d", num_satellites);

		if (nrf_modem_gnss_read(&pvt_data, sizeof(pvt_data), NRF_MODEM_GNSS_DATA_PVT)) {
			LOG_ERR("nrf_modem_gnss_read failed");
			return;
		}

		if (pvt_data.flags & NRF_MODEM_GNSS_PVT_FLAG_FIX_VALID) {
			print_and_send_fix_data(&pvt_data);
			if (!first_fix) {
				LOG_INF("Time to first fix: %2.1lld s",
					(k_uptime_get() - gnss_start_time) / 1000);
				first_fix = true;
			}
			return;
		}

		if (pvt_data.flags & NRF_MODEM_GNSS_PVT_FLAG_DEADLINE_MISSED) {
			LOG_INF("GNSS blocked by LTE activity");
		} else if (pvt_data.flags & NRF_MODEM_GNSS_PVT_FLAG_NOT_ENOUGH_WINDOW_TIME) {
			LOG_INF("Insufficient GNSS time window");
		}
		break;
	case NRF_MODEM_GNSS_EVT_PERIODIC_WAKEUP:
		LOG_INF("GNSS has woken up");
		break;
	case NRF_MODEM_GNSS_EVT_SLEEP_AFTER_FIX:
		LOG_INF("GNSS enter sleep after fix");
		break;
	default:
		break;
	}
}

static int modem_configure(void)
{
	int err = nrf_modem_lib_init();
	if (err) {
		LOG_ERR("Failed to initialize modem library: %d", err);
		return err;
	}

	err = lte_lc_psm_req(true);
	if (err) {
		LOG_ERR("lte_lc_psm_req error: %d", err);
		return err;
	}

	err = lte_lc_edrx_req(true);
	if (err) {
		LOG_ERR("lte_lc_edrx_req error: %d", err);
		return err;
	}

	err = lte_lc_connect_async(lte_handler);
	if (err) {
		LOG_ERR("lte_lc_connect_async error: %d", err);
		return err;
	}

	k_sem_take(&lte_connected, K_FOREVER);
	LOG_INF("Connected to LTE network");
	return 0;
}

static int gnss_init_and_start(void)
{
	if (lte_lc_func_mode_set(LTE_LC_FUNC_MODE_NORMAL) != 0) {
		LOG_ERR("Failed to activate GNSS functional mode");
		return -1;
	}

	if (nrf_modem_gnss_event_handler_set(gnss_event_handler) != 0) {
		LOG_ERR("Failed to set GNSS event handler");
		return -1;
	}

	uint16_t interval = 10;
	int err = nrf_modem_gnss_fix_interval_set(interval);
	if (err != 0) {
		LOG_ERR("Failed to set fix interval: %d", err);
		return err;
	}

	uint16_t retry = 0;
	err = nrf_modem_gnss_fix_retry_set(retry);
	if (err != 0) {
		LOG_ERR("Failed to set fix retry: %d", err);
		return err;
	}

	uint8_t angle = 0;
	err = nrf_modem_gnss_elevation_threshold_set(angle);
	if (err != 0) {
		LOG_ERR("Failed to set elevation threshold: %d", err);
		return err;
	}

	uint8_t use_case = NRF_MODEM_GNSS_USE_CASE_MULTIPLE_HOT_START;
	if (nrf_modem_gnss_use_case_set(use_case) != 0) {
		LOG_ERR("Failed to set GNSS use case");
		return -1;
	}

	if (nrf_modem_gnss_start() != 0) {
		LOG_ERR("Failed to start GNSS");
		return -1;
	}

	gnss_start_time = k_uptime_get();
	LOG_INF("Starting GNSS");
	return 0;
}

int main(void)
{
	if (modem_configure()) {
		LOG_ERR("Failed to configure the modem");
		return 0;
	}

	if (server_resolve() != 0) {
		LOG_INF("Failed to resolve server name");
		return 0;
	}

	if (server_connect() != 0) {
		LOG_INF("Failed to initialize client");
		return 0;
	}

	if (gnss_init_and_start() != 0) {
		LOG_ERR("Failed to initialize and start GNSS");
		return 0;
	}

	while (1) {
		int received = recv(sock, recv_buf, sizeof(recv_buf) - 1, 0);
		if (received < 0) {
			LOG_ERR("Socket error: %d, exit", errno);
			break;
		} else if (received == 0) {
			break;
		}
		recv_buf[received] = 0;
		LOG_INF("Data received from the server: (%s)", recv_buf);
	}

	close(sock);
	return 0;
}
