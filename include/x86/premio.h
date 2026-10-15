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

#define MRAA_PREMIO_DCO_1000_PINCOUNT (8 + 1)
#define MRAA_PREMIO_RCO_3400_PINCOUNT (16 + 1)

mraa_board_t*
mraa_premio_dco_1000();

mraa_board_t*
mraa_premio_rco_3400();

#ifdef __cplusplus
}
#endif
