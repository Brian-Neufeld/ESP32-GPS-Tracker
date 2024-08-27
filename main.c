#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "esp_mac.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/gpio.h"


#define BUF_SIZE (1024)

#define EXAMPLE_MAX_CHAR_SIZE    128

char data[EXAMPLE_MAX_CHAR_SIZE];

static const char *TAG = "example";

#define MOUNT_POINT "/sdcard"

#define PIN_NUM_MISO  9
#define PIN_NUM_MOSI  10
#define PIN_NUM_CLK   8
#define PIN_NUM_CS    4

static esp_err_t s_example_write_file(const char *path, char *data)
{
    ESP_LOGI(TAG, "Opening file %s", path);
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    return ESP_OK;
}

static void uart_task()
{

    uart_config_t uart_config = 
    {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM_1, BUF_SIZE, BUF_SIZE, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 2, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uint8_t *GPS_data = (uint8_t*)malloc(BUF_SIZE);

    char* GPS_rate_command = "$PMTK220,100*2F\r\n";

    char* GPS_NMEA_sentence_command = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";

    char* GPS_erase_flash = "$PMTK184,1*22\r\n";

    
    uart_write_bytes(UART_NUM_1, (const char*)GPS_NMEA_sentence_command, 52);
    uart_wait_tx_done(UART_NUM_1, 1000);

    

    uart_write_bytes(UART_NUM_1, (const char*)GPS_rate_command, 18);
    uart_wait_tx_done(UART_NUM_1, 1000);

    

    uart_write_bytes(UART_NUM_1, (const char*)GPS_erase_flash, 16);
    uart_wait_tx_done(UART_NUM_1, 1000);

    

    while (1)
    {
        const int len = uart_read_bytes(UART_NUM_1, GPS_data, BUF_SIZE, 2);
        if (len>0)
        {
            GPS_data[len] = 0;
            
            printf("%s\n", (const char *)data);
            printf("\n");

            const char *file_hello = MOUNT_POINT"/DATA.txt";

            snprintf(data, EXAMPLE_MAX_CHAR_SIZE, "%s", GPS_data);
            s_example_write_file(file_hello, data);
            
        }
    }

    uart_flush(UART_NUM_1);

}

void SD_Setup(void)
{
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    #ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
    #else
        .format_if_mount_failed = false,
    #endif
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };


    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = 4;
    slot_config.host_id = host.slot;
    

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    sdmmc_card_print_info(stdout, card);


}

void app_main(void)
{

    SD_Setup();
    
    uart_task();
}
