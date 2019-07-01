/*
 * Copyright (C) 2019 Jakub Kaderka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file    hal/io.h
 * @brief   IO driver, IO initialization,...
 *
 * Sets IO mode based on board_gpio.h file (generated by mx2board.py) from
 * stm32cubemx project
 *
 * @addtogroup hal
 * @{
 */

#ifndef __HAL_IO_H
#define __HAL_IO_H

#include <types.h>
#include <libopencm3/stm32/gpio.h> /* required for PORTA,... defines */

/** setting bits in IO registers */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/** PAL_LINE for chibios format of line define */
#define PAL_LINE(a, b)  a, b

#include "app/board_gpio.h"

/**
 * Set pin output
 *
 * Can be used with PAL_LINE defines like IOd_SetLine(LINE_FOO, true)
 *
 * @param port      GPIO register (GPIOA, ...)
 * @param pad       Pin number
 * @param value     Value to set pin to
 */
void IOd_SetLine(uint32_t port, uint8_t pad, bool value);

/**
 * Get value on input
 *
 * Can be used with PAL_LINE defines like IOd_SetLine(LINE_FOO, true)
 *
 * @param port      GPIO register (GPIOA, ...)
 * @param pad       Pin number
 * @return          Value of given port,pin
 */
bool IOd_GetLine(uint32_t port, uint8_t pad);

/**
 * Initialize IO pins of the MCU based on configuration from board_gpio.h
 */
void IOd_Init(void);

#endif

/** @} */
