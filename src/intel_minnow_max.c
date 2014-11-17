/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "intel_minnow_max.h"

#define MAX_SIZE 64
#define SYSFS_CLASS_GPIO "/sys/class/gpio"

void
mraa_set_pininfo(mraa_board_t* board, int mraa_index, char *name, mraa_pincapabilities_t caps, int gpio_map) 
{
    if (mraa_index < board->phy_pin_count) {
	mraa_pininfo_t* pin_info = &board->pins[mraa_index];
        strncpy(pin_info->name, name, 8);
        pin_info->capabilites = caps;
        if (caps.gpio)
	    pin_info->gpio.pinmap = gpio_map;
    }
}
 

mraa_board_t*
mraa_intel_minnow_max()
{
    mraa_board_t* b = (mraa_board_t*) malloc(sizeof(mraa_board_t));
    if (b == NULL)
        return NULL;

    b->phy_pin_count = MRAA_INTEL_MINNOW_MAX_PINCOUNT;
    //b->gpio_count = 14;
    b->aio_count = 0;
    b->adc_raw = 0;
    b->adc_supported = 0;
    b->pwm_default_period = 500;
    b->pwm_max_period = 2147483;
    b->pwm_min_period = 1;

    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t)*MRAA_INTEL_MINNOW_MAX_PINCOUNT);
    mraa_set_pininfo(b,  1, "GND",      (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, -1);
    mraa_set_pininfo(b,  2, "GND",      (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, -1);
    mraa_set_pininfo(b,  3, "5v",       (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, -1);
    mraa_set_pininfo(b,  4, "3.3v",     (mraa_pincapabilities_t){1,0,0,0,0,0,0,0}, -1);
    mraa_set_pininfo(b,  5, "SPI_CS",   (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, 220);
    mraa_set_pininfo(b,  6, "UART1_TX", (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, 225);
    mraa_set_pininfo(b,  7, "SPI_MISO", (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, 221);
    mraa_set_pininfo(b,  8, "UART1_RX", (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, 224);
    mraa_set_pininfo(b,  9, "SPI_MOSI", (mraa_pincapabilities_t){1,1,0,0,1,0,0,0}, 222);
    mraa_set_pininfo(b, 10, "UART1_CT", (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 227);
    mraa_set_pininfo(b, 11, "SPI_CLK",  (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, 223);
    mraa_set_pininfo(b, 12, "UART1_RT", (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, 226);
    mraa_set_pininfo(b, 13, "I2C_SCL",  (mraa_pincapabilities_t){1,1,0,0,0,1,0,0}, 243);
    b->pins[13].i2c.pinmap = 1;
    b->pins[13].i2c.mux_total = 0;
    mraa_set_pininfo(b, 14, "I2S_CLK",  (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 216);
    mraa_set_pininfo(b, 15, "I2C_SDA",  (mraa_pincapabilities_t){1,1,0,0,0,1,0,0}, 242);
    b->pins[15].i2c.pinmap = 1;
    b->pins[15].i2c.mux_total = 0;
    mraa_set_pininfo(b, 16, "I2S_FRM",  (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 217);
    mraa_set_pininfo(b, 17, "UART2_TX", (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, 229);
    mraa_set_pininfo(b, 18, "I2S_DO",   (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 219);
    mraa_set_pininfo(b, 19, "UART2_RX", (mraa_pincapabilities_t){1,1,0,0,0,0,0,1}, 228);
    mraa_set_pininfo(b, 20, "I2S_DI",   (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 218);
    mraa_set_pininfo(b, 21, "S5_0",     (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 82);
    mraa_set_pininfo(b, 22, "PWM0",     (mraa_pincapabilities_t){1,1,1,0,0,0,0,0}, 248);
    mraa_set_pininfo(b, 23, "S5_1",     (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 83);
    mraa_set_pininfo(b, 24, "PWM1",     (mraa_pincapabilities_t){1,1,1,0,0,0,0,0}, 249);
    mraa_set_pininfo(b, 25, "S5_4",     (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 84);
    mraa_set_pininfo(b, 26, "IBL_8254", (mraa_pincapabilities_t){1,1,0,0,0,0,0,0}, 208);

    //BUS DEFINITIONS
    b->i2c_bus_count = 9;
    int ici;
    for (ici = 0; ici < 9; ici++) {
        b->i2c_bus[ici].bus_id = -1;
    }
    b->def_i2c_bus = 0;
    // helpfully, all minnowboard max docs refer to i2c #6 but this is actually
    // mapped to /dev/i2c-0
    b->i2c_bus[0].bus_id = 0;
    b->i2c_bus[0].sda = 15;
    b->i2c_bus[0].scl = 13;

    b->spi_bus_count = 1;
    b->def_spi_bus = 0;
    b->spi_bus[0].bus_id = 0;
    b->spi_bus[0].slave_s = 0;
    b->spi_bus[0].cs = 5;
    b->spi_bus[0].mosi = 9;
    b->spi_bus[0].miso = 7;
    b->spi_bus[0].sclk = 11;

    return b;
}
