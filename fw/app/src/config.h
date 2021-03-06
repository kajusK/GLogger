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
 * @file    app/config.h
 * @brief   System configuration
 *
 * @addtogroup app
 * @{
 */

#ifndef __APP_CONFIG_H_
#define __APP_CONFIG_H_

/** Size of the external flash in bytes */
#define STORAGE_SIZE 4000000U

#define USB_VENDOR 0x0483 /* STMicroelectronics */
#define USB_PRODUCT 0x5720 /* Mass storage device */
#define USB_MANUFACTURE_STR "Deadbadger"
#define USB_DEVICE_STR "Device"
#define USB_VERSION_STR "0000"

#endif

/** @} */
