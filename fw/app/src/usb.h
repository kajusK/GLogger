/*
 * Copyright (C) 2020 Jakub Kaderka
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
 * @file    app/usb.h
 * @brief   USB configuration, file descriptors, etc.
 *
 * @addtogroup app
 * @{
 */

#ifndef __APP_USB_H_
#define __APP_USB_H_

/**
 * Poll USB stack, call periodically
 */
extern void Usb_Poll(void);

/**
 * Initialize the USB and it's services
 */
extern void Usb_Init(void);

#endif

/** @} */
