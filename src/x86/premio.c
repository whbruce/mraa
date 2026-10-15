/*
 * Author: Henry Bruce <henry.bruce@pano.ai>
 * Copyright (c) 2025 Pano AI, Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/io.h>

#include "common.h"
#include "x86/premio.h"

#define PLATFORM_VERSION "1.0.0"
#define DCO_1000_PLATFORM_NAME "Premio DCO-1000"
#define RCO_3400_PLATFORM_NAME "Premio RCO-3400"
#define BASE_PORT 0xA02


static mraa_result_t
mraa_premio_set_pininfo(mraa_board_t* board, int mraa_index, char* name, int io_addr, int bit)
{
    if (mraa_index < board->phy_pin_count) {
        mraa_pininfo_t* pin_info = &board->pins[mraa_index];
        strncpy(pin_info->name, name, MRAA_PIN_NAME_SIZE);
        pin_info->capabilities = (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 };
        pin_info->gpio.pinmap = mraa_index;
        // Use gpio_chip and gpio_line to hold io port and bit
        pin_info->gpio.gpio_chip = io_addr;
        pin_info->gpio.gpio_line = bit;
        return MRAA_SUCCESS;
    }
    return MRAA_ERROR_INVALID_RESOURCE;
}


static mraa_result_t
premio_gpio_init_internal(mraa_gpio_context dev, int pin)
{
    printf("premio_gpio_init_internal %d\n", pin);
    dev->phy_pin = pin;
    return MRAA_SUCCESS;
}

static mraa_result_t
premio_gpio_dir(mraa_gpio_context dev, mraa_gpio_dir_t dir)
{
    printf("premio_gpio_dir %d %d\n", dev->pin, dir);
    if ((dev->pin % 2 == 0) && (dir == MRAA_GPIO_IN))
        return MRAA_ERROR_INVALID_RESOURCE;
    if ((dev->pin % 2) && (dir == MRAA_GPIO_OUT))
        return MRAA_ERROR_INVALID_RESOURCE;
    return MRAA_SUCCESS;
}

static int
premio_gpio_read(mraa_gpio_context dev)
{
    int pin = dev->pin;
    int port = plat->pins[pin].gpio.gpio_chip;
    int bit = plat->pins[pin].gpio.gpio_line;
    unsigned char read_value = inb(port);
    unsigned char mask = 1 << bit;
    int value = (read_value & mask) ? 1 : 0;
    printf("premio_gpio_read %d %03X %d -> %X %d\n", pin, port, bit, read_value, value);
    return value;
}


static mraa_result_t
premio_gpio_write(mraa_gpio_context dev, int value)
{
    int pin = dev->pin;
    int port = plat->pins[pin].gpio.gpio_chip;
    int bit = plat->pins[pin].gpio.gpio_line;
    unsigned char read_value = inb(port);
    unsigned char mask = 1 << bit;
    unsigned char write_value;
    if (value == 0)
        write_value = read_value & ~mask;
    else
        write_value = read_value | mask;
    printf("premio_gpio_write %d %03X %d %d\n", pin, port, bit, value);
    printf("premio_gpio_write %03X %X %03X\n", read_value, mask, write_value);
    printf("premio_gpio_write pre-result: %s\n", strerror(errno));
    outb(write_value, port);
    printf("premio_gpio_write result: %s\n", strerror(errno));
    return MRAA_SUCCESS;
}

static mraa_board_t*
mraa_premio_board_init(char* board_name, int pin_count)
{
    mraa_board_t* b = (mraa_board_t*) calloc(1, sizeof(mraa_board_t));

    if (b == NULL) {
        return NULL;
    }

    int port_range = (pin_count == MRAA_PREMIO_RCO_3400_PINCOUNT) ? 5 : 2;
    int res = ioperm(BASE_PORT, port_range, 1);
    if (res != 0) {
        syslog(LOG_ERR, "premio_gpio_init: ioperm failed (%s)", strerror(errno));
        goto error;
    }

    b->platform_name = board_name;
    b->platform_version = PLATFORM_VERSION;
    b->phy_pin_count = pin_count;
    b->gpio_count = pin_count;

    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t) * MRAA_PREMIO_DCO_1000_PINCOUNT);
    if (b->pins == NULL) {
        goto error;
    }

    b->adv_func = (mraa_adv_func_t*) calloc(1, sizeof(mraa_adv_func_t));
    if (b->adv_func == NULL) {
        free(b->pins);
        goto error;
    }

    b->adv_func->gpio_init_internal_replace = premio_gpio_init_internal;
    b->adv_func->gpio_dir_replace = premio_gpio_dir;
    b->adv_func->gpio_read_replace = premio_gpio_read;
    b->adv_func->gpio_write_replace = premio_gpio_write;

    return b;

error:
    syslog(LOG_CRIT, "%s platform failed to initialise", board_name);
    free(b);
    return NULL;
}

mraa_board_t*
mraa_premio_rco_3400()
{
    mraa_board_t* b = mraa_premio_board_init(RCO_3400_PLATFORM_NAME, MRAA_PREMIO_RCO_3400_PINCOUNT);

    if (b == NULL) {
        return NULL;
    }

    // DI/0 Connector
    int output_port = BASE_PORT;
    int input_port = BASE_PORT + 1;
    mraa_premio_set_pininfo(b,  1,  "DIN1",  input_port,  4);
    mraa_premio_set_pininfo(b,  2,  "DOUT1", output_port, 0);
    mraa_premio_set_pininfo(b,  3,  "DIN2",  input_port,  5);
    mraa_premio_set_pininfo(b,  4,  "DOUT2", output_port, 1);
    mraa_premio_set_pininfo(b,  5,  "DIN3",  input_port,  6);
    mraa_premio_set_pininfo(b,  6,  "DOUT3", output_port, 2);
    mraa_premio_set_pininfo(b,  7,  "DIN4",  input_port,  7);
    mraa_premio_set_pininfo(b,  8,  "DOUT4", output_port, 3);
    mraa_premio_set_pininfo(b,  9,  "DIN5",  BASE_PORT+4, 0);
    mraa_premio_set_pininfo(b, 10,  "DOUT5", BASE_PORT+4, 4);
    mraa_premio_set_pininfo(b, 11,  "DIN6",  BASE_PORT+4, 1);
    mraa_premio_set_pininfo(b, 12,  "DOUT6", BASE_PORT+5, 7);
    mraa_premio_set_pininfo(b, 13,  "DIN7",  BASE_PORT+4, 2);
    mraa_premio_set_pininfo(b, 14,  "DOUT7", BASE_PORT+6, 0);
    mraa_premio_set_pininfo(b, 15,  "DIN8",  BASE_PORT+4, 3);
    mraa_premio_set_pininfo(b, 16,  "DOUT8", BASE_PORT+2, 7);

    return b;
}


mraa_board_t*
mraa_premio_dco_1000()
{
    mraa_board_t* b = mraa_premio_board_init(DCO_1000_PLATFORM_NAME, MRAA_PREMIO_DCO_1000_PINCOUNT);

    if (b == NULL) {
        return NULL;
    }

    // DI/0 Connector
    int output_port = BASE_PORT;
    int input_port = BASE_PORT + 1;
    mraa_premio_set_pininfo(b, 1,  "IN1_1",  input_port,  0);
    mraa_premio_set_pininfo(b, 2,  "OUT1_1", output_port, 0);
    mraa_premio_set_pininfo(b, 3,  "IN2_1",  input_port,  1);
    mraa_premio_set_pininfo(b, 4,  "OUT2_1", output_port, 1);
    mraa_premio_set_pininfo(b, 5,  "IN3_1",  input_port,  2);
    mraa_premio_set_pininfo(b, 6,  "OUT3_1", output_port, 2);
    mraa_premio_set_pininfo(b, 7,  "IN4_1",  input_port,  3);
    mraa_premio_set_pininfo(b, 8,  "OUT4_1", output_port, 3);

    return b;
}

