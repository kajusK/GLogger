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
 * @file    app/storage.h
 * @brief   Storing GPS data into external flash
 *
 * @addtogroup app
 * @{
 */

#ifndef __APP_STORAGE_H
#define __APP_STORAGE_H

#include <types.h>
#include "drivers/gps.h"

typedef struct {
    int32_t lat;
    int32_t lat_scale;
    int32_t lon;
    int32_t lon_scale;
    time_t timestamp;
    int16_t elevation_m;
} __attribute__((packed)) storage_item_t;

/**
 * Check if given item is end of log mark
 *
 * @param item  Item to be checked
 * @return true if EOL
 */
extern bool Storage_IsEOL(const storage_item_t *item);

/**
 * Erase all records in the memory
 */
extern void Storage_Erase(void);

/**
 * Get amount of remaining space in the memory
 *
 * @return Remaining amount of items that will fit the memory
 */
extern size_t Storage_SpaceRemaining(void);

/**
 * Get amount of space already used by gps records
 *
 * @return Items already stored in memory
 */
extern size_t Storage_SpaceUsed(void);

/**
 * Get size of the storage
 *
 * @return Size of the storage in items
 */
extern size_t Storage_GetSize(void);

/**
 * Add GPS record to memory
 *
 * @param info      Record to store
 * @return False if memory full
 */
extern bool Storage_Add(const gps_info_t *info);

/**
 * Read one record from memory
 *
 * Item with all fields equal to 0 is considered end of current log
 *
 * @param id        Record id (sequence number, from 0)
 * @param item      Item to store result to
 * @return  False if item of given id does not exist
 */
extern bool Storage_Get(uint32_t id, storage_item_t *item);

/**
 * Check the content of the flash, find last record, add end of log mark
 */
extern void Storage_Init(void);

#endif

/** @} */
