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
 * @file    drivers/gps.h
 * @brief   Driver for gps receiver (sleep/wake up implemented for SIM28)
 *
 * @addtogroup drivers
 * @{
 */

#ifndef __DRIVERS_GPS_H_
#define __DRIVERS_GPS_H_

#include <types.h>
#include <time.h>

#include "modules/nmea.h"

typedef struct {
    nmea_float_t lat;   /* In decimal degrees */
    nmea_float_t lon;   /* In deciaml degrees */
    int32_t altitude_dm;/* Altitude in dm */
    int32_t speed_dmh;  /* speed in dm/s */
    int32_t hdop_dm;    /* location precision */
    uint8_t satellites; /* Visible satellites */
    time_t timestamp;   /* Unix timestamp of the data received */
} gps_info_t;

/**
 * Put GPS device into low power mode (invalidates currently stored gps info)
 */
extern void Gps_Sleep(void);

/**
 * Wake up GPS device from sleep mode
 */
extern void Gps_WakeUp(void);

/**
 * Get received GPS data
 *
 * Should be called periodically as it fetches the data from uart buffer
 *
 * @return NULL if no valid data received, else pointer to received data
 */
gps_info_t *Gps_Get(void);

/**
 * Initialize GPS module
 */
extern void Gps_Init(void);

#endif

/** @} */
