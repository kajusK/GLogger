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
 * @file    app/stats.c
 * @brief   Stats from the logs - distance travelled, etc.
 *
 * @addtogroup app
 * @{
 */

#include <string.h>

#include "storage.h"
#include "utils/nav.h"
#include "stats.h"

/* Min distance between logs to be considered as a movement */
#define STATS_MIN_DIST_M 5

/* Max time between points in storage to be considered as single log */
#define STATS_MAX_TIME_MIN  10

static stats_t statsi;

void Stats_Update(const gps_info_t *gps)
{
    static gps_info_t prev;
    static bool prev_ready = false;

    uint32_t distance;
    int32_t altitude;
    uint32_t time;

    /** Skip logs with incomplete data */
    if (gps->timestamp == 0 || gps->altitude_dm == 0) {
        return;
    }
    /* First log after boot */
    if (prev_ready == false) {
        memcpy(&prev, gps, sizeof(prev));
        prev_ready = true;
        return;
    }

    distance = Nav_GetDistanceDm(&gps->lat, &gps->lon, &prev.lat, &prev.lon);
    altitude = (gps->altitude_dm - prev.altitude_dm);
    time = gps->timestamp - prev.timestamp;
    /* Points too close to each other, skip */
    if (distance/10 < STATS_MIN_DIST_M) {
        return;
    }

    statsi.storage_used_pct = (Storage_SpaceUsed()*100)/Storage_GetSize();
    statsi.today.dist_dm += distance;
    statsi.all.dist_dm += distance;
    statsi.today.time_s += time;
    statsi.all.time_s += time;
    if (altitude >= 0) {
        statsi.today.ascend_dm += altitude;
        statsi.all.ascend_dm += altitude;
    } else {
        statsi.today.descend_dm += -altitude;
        statsi.all.descend_dm += -altitude;
    }

    memcpy(&prev, gps, sizeof(prev));
}

stats_t *Stats_Get(void)
{
    return &statsi;
}

void Stats_Init(void)
{
    storage_item_t item;
    storage_item_t prev;
    int i = 1;

    memset(&statsi, 0x00, sizeof(stats_t));
    Storage_Get(0, &prev);

    while (Storage_Get(i++, &item) != false) {
        int32_t time_diff = (int32_t)item.timestamp - (int32_t)prev.timestamp;
        uint32_t distance;
        int32_t altitude;
        nmea_float_t lat1, lon1, lat2, lon2;

        /* Ignore incomplete points */
        if (item.timestamp == 0 || item.elevation_m == 0) {
            prev = item;
            continue;
        }
        /* Ignore time differences larger than 10 minutes (new record) */
        if (time_diff < 0 || time_diff > 60*STATS_MAX_TIME_MIN) {
            prev = item;
            continue;
        }

        lat1.num = prev.lat;
        lat1.scale = prev.lat_scale;
        lon1.num = prev.lon;
        lon1.scale = prev.lon_scale;
        lat2.num = item.lat;
        lat2.scale = item.lat_scale;
        lon2.num = item.lon;
        lon2.scale = item.lon_scale;
        distance = Nav_GetDistanceDm(&lat1, &lon1, &lat2, &lon2);
        /* Ignore points too close to each other (noise, hdop,...) */
        if (distance/10 < STATS_MIN_DIST_M) {
            continue;
        }

        altitude = item.elevation_m - prev.timestamp;
        statsi.all.dist_dm += distance;
        statsi.all.time_s += time_diff;
        if (altitude >= 0) {
            statsi.all.ascend_dm += altitude;
        } else {
            statsi.all.descend_dm += -altitude;
        }

        prev = item;
    }

    statsi.storage_used_pct = (Storage_SpaceUsed()*100)/Storage_GetSize();
}

/** @} */
