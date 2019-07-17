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
 * @file    app/gpx.h
 * @brief   GPX file generator
 *
 * @addtogroup app
 * @{
 */

#ifndef __APP_GPX_H_
#define __APP_GPX_H_

#include <types.h>

/**
 * Get size of gpx file
 *
 * @return Size in bytes
 */
extern uint32_t GPX_GetSize(void);

extern bool GPX_Get(uint32_t offset, uint8_t *buf, uint32_t len);

#endif

/** @} */

