#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUF_SIZE (1024)



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

    uint8_t *data = (uint8_t*)malloc(BUF_SIZE);

    while (1)
    {
        const int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 10);
        if (len>0)
        {
            data[len] = 0;
            uart_write_bytes(UART_NUM_1, (const char *)data, len);
            printf("%s\n", (const char *)data);
            vTaskDelay(100);
        }
    }

    uart_flush(UART_NUM_1);

}

void app_main(void)
{
    uart_task();
}
