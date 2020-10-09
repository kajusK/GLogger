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
 * @file    app/stats.h
 * @brief   Stats from the logs - distance travelled, etc.
 *
 * @addtogroup app
 * @{
 */

#ifndef __APP_STATS_H_
#define __APP_STATS_H_

#include <types.h>
#include "drivers/gps.h"

typedef struct {
    uint32_t dist_dm;        /** Distance travelled */
    uint32_t ascend_dm;      /** Amount of meters ascended */
    uint32_t descend_dm;     /** Amount of meters descended */
    uint32_t time_s;        /** Time for how long the device was on */
} stats_comm_t;

typedef struct {
    stats_comm_t today;     /* Stats since device power on */
    stats_comm_t all;       /* Stats since latest storage erase */
    uint8_t storage_used_pct;
} stats_t;

/**
 * Add new point to statistics
 *
 * @param gps   GPS measured data
 */
extern void Stats_Update(const gps_info_t *gps);

/**
 * Get current statistics
 *
 * @return pointer to statistic data
 */
extern stats_t *Stats_Get(void);

/**
 * Initialize stats, will load all records from storage and calculate stats
 */
extern void Stats_Init(void);

#endif

/** @} */

