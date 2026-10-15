/*
 * Author: Henry Bruce <henry.bruce@pano.ai>
 * Copyright (c) 2025 Pano AI, Inc.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mraa_internal.h"

#define MRAA_DELL_EGW_3200_DIO_PINCOUNT (26 + 1)
#define MRAA_DELL_EGW_3200_LED_PINCOUNT 3
#define MRAA_DELL_EGW_3200_PINCOUNT \
        (MRAA_DELL_EGW_3200_DIO_PINCOUNT + MRAA_DELL_EGW_3200_LED_PINCOUNT)

mraa_board_t*
mraa_dell_egw_3200();

#ifdef __cplusplus
}
#endif
