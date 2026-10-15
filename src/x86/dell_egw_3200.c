/*
 * Author: Henry Bruce <henry.bruce@pano.ai>
 * Copyright (c) 2025 Pano AI, Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "x86/dell_egw_3200.h"

#define PLATFORM_NAME "Dell EGW-3200"
#define PLATFORM_VERSION "1.0.0"


static mraa_result_t
mraa_egw_3200_set_pininfo(mraa_board_t* board, int mraa_index, char* name, mraa_pincapabilities_t caps, int sysfs_pin)
{
    if (mraa_index < board->phy_pin_count) {
        mraa_pininfo_t* pin_info = &board->pins[mraa_index];
        strncpy(pin_info->name, name, MRAA_PIN_NAME_SIZE);
        pin_info->capabilities = caps;
        if (caps.gpio) {
            pin_info->gpio.pinmap = sysfs_pin;
            pin_info->gpio.mux_total = 0;
        }
        if (caps.i2c) {
            pin_info->i2c.pinmap = 1;
            pin_info->i2c.mux_total = 0;
        }
        if (caps.pwm) {
            int controller = 0;
            if (strncmp(name, "PWM", 3) == 0 && strlen(name) > 3 && isdigit(name[3]))
                controller = name[3] - '0';
            pin_info->pwm.parent_id = (unsigned int) controller;
            pin_info->pwm.pinmap = 0;
            pin_info->pwm.mux_total = 0;
        }
        if (caps.spi) {
            pin_info->spi.mux_total = 0;
        }
        if (caps.aio) {
            pin_info->aio.mux_total = 0;
            pin_info->aio.pinmap = 0;
        }
        if (caps.uart) {
            pin_info->uart.mux_total = 0;
        }
        return MRAA_SUCCESS;
    }
    return MRAA_ERROR_INVALID_RESOURCE;
}

static mraa_result_t
mraa_egw_3200_get_pin_index(mraa_board_t* board, char* name, int* pin_index)
{
    int i;
    for (i = 0; i < board->phy_pin_count; ++i) {
        if (strncmp(name, board->pins[i].name, MRAA_PIN_NAME_SIZE) == 0) {
            *pin_index = i;
            return MRAA_SUCCESS;
        }
    }

    syslog(LOG_CRIT, "egw-3200: Failed to find pin name %s", name);

    return MRAA_ERROR_INVALID_RESOURCE;
}


mraa_board_t*
mraa_dell_egw_3200()
{
    mraa_board_t* b = (mraa_board_t*) calloc(1, sizeof(mraa_board_t));

    if (b == NULL) {
        return NULL;
    }

    b->platform_name = PLATFORM_NAME;
    b->platform_version = PLATFORM_VERSION;
    b->phy_pin_count = MRAA_DELL_EGW_3200_PINCOUNT;
    b->gpio_count = MRAA_DELL_EGW_3200_PINCOUNT;

    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t) * MRAA_DELL_EGW_3200_PINCOUNT);
    if (b->pins == NULL) {
        goto error;
    }

    b->adv_func = (mraa_adv_func_t*) calloc(1, sizeof(mraa_adv_func_t));
    if (b->adv_func == NULL) {
        free(b->pins);
        goto error;
    }

    // DI/0 Connector
    mraa_egw_3200_set_pininfo(b, 0,  "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 1,  "DI0",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 884);
    mraa_egw_3200_set_pininfo(b, 2,  "DI1",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 885);
    mraa_egw_3200_set_pininfo(b, 3,  "DI2",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 886);
    mraa_egw_3200_set_pininfo(b, 4,  "DI3",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 887);
    mraa_egw_3200_set_pininfo(b, 5,  "DI4",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 897);
    mraa_egw_3200_set_pininfo(b, 6,  "DI5",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 898);
    mraa_egw_3200_set_pininfo(b, 7,  "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 8,  "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 9,  "GND",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 10, "CAN1_H", (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 11, "CAN1_L", (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 12, "GND",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 13, "CAN0_H", (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 14, "CAN0_L", (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 15, "GND",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 16, "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 17, "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 18, "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 19, "DO0",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 986);
    mraa_egw_3200_set_pininfo(b, 20, "DO1",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 985);
    mraa_egw_3200_set_pininfo(b, 21, "DO2",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 983);
    mraa_egw_3200_set_pininfo(b, 22, "DO3",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 984);
    mraa_egw_3200_set_pininfo(b, 23, "DO4",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 912);
    mraa_egw_3200_set_pininfo(b, 24, "DO5",    (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 916);
    mraa_egw_3200_set_pininfo(b, 25, "N/C",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);
    mraa_egw_3200_set_pininfo(b, 26, "GND",    (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 },  -1);

    // LEDs (virtual pin indices)
    mraa_egw_3200_set_pininfo(b, 27, "U1",     (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 881);
    mraa_egw_3200_set_pininfo(b, 28, "U2",     (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 882);
    mraa_egw_3200_set_pininfo(b, 29, "U3",     (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 }, 883);    

 
    // Set number of i2c adaptors usable from userspace
    b->i2c_bus_count = 0;
    b->def_i2c_bus = 0;
    int i2c_bus_num;

    // Configure UART #1 (default)
    b->uart_dev_count = 2;
    b->uart_dev[0].device_path = "/dev/ttyS0";
    b->uart_dev[1].device_path = "/dev/ttyS1";
    b->def_uart_dev = 0;

    return b;

error:
    syslog(LOG_CRIT, "EGW-3200 platform failed to initialise");
    free(b);
    return NULL;
}
