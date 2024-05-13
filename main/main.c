#include "esp_log.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"

static const char *TAG = "app";

static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    size_t rx_size = 0;
    esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK)
    {
        ESP_LOGD(TAG, "Data from channel %d:", itf);
        ESP_LOG_BUFFER_HEXDUMP(TAG, buf, rx_size, ESP_LOG_DEBUG);
        if (rx_size > 0)
        {
            tud_hid_n_report(0, buf[0], &buf[1], rx_size - 1);
        }
    }
}

extern tinyusb_config_t tusb_cfg;
int main(void)
{
    ESP_LOGI(TAG, "USB initialization");
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback,
        .callback_rx_wanted_char = NULL,
    };
    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");
}

void app_main(void)
{
    main();
}
