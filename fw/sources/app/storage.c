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
 * @file    app/storage.c
 * @brief   Storing GPS data into external flash
 *
 * @addtogroup app
 * @{
 */

#include "drivers/spi_flash.h"
#include "app/config.h"

#include "app/storage.h"

static uint32_t storagei_offset = 0;

/**
 * Check if storage item is empty (all bits are 0xff - erased flash)
 *
 * @param item      Item to be checked
 * @return true if empty
 */
static bool Storagei_ItemEmpty(storage_item_t *item)
{
    uint8_t *pos = (uint8_t *) item;

    for (size_t i = 0; i < sizeof(storage_item_t); i++) {
        if (*pos++ != 0xff) {
            return false;
        }
    }
    return true;
}

void Storage_Erase(void)
{
    SpiFlash_Erase();
    storagei_offset = 0;
}

size_t Storage_SpaceRemaining(void)
{
    return (STORAGE_SIZE - storagei_offset)/sizeof(storage_item_t);
}

size_t Storage_SpaceUsed(void)
{
    return storagei_offset/sizeof(storage_item_t);
}

bool Storage_Add(const gps_info_t *info)
{
    storage_item_t item;

    if (Storage_SpaceRemaining() == 0) {
        return false;
    }

    item.lat_deg = info->lat.deg;
    item.lat_min = info->lat.min;
    item.lat_frac = info->lat.frac;
    item.lon_deg = info->lon.deg;
    item.lon_min = info->lon.min;
    item.lon_frac = info->lon.frac;
    item.timestamp = info->timestamp;
    item.elevation_m = info->altitude_dm / 10;

    SpiFlash_Write(storagei_offset, (uint8_t *) &item, sizeof(item));
    storagei_offset += sizeof(item);
    return true;
}

bool Storage_Get(uint32_t id, storage_item_t *item)
{
    uint32_t offset = id * sizeof(storage_item_t);
    if (offset >= storagei_offset) {
        return false;
    }

    SpiFlash_Read(offset, (uint8_t *) item, sizeof(storage_item_t));
    return true;
}

void Storage_Init(void)
{
    uint8_t buf[sizeof(storage_item_t)];
    uint8_t item_size = sizeof(storage_item_t);
    storagei_offset = 0;

    while (storagei_offset < STORAGE_SIZE) {
        SpiFlash_Read(storagei_offset, buf, item_size);
        if (Storagei_ItemEmpty((storage_item_t *) buf)) {
            break;
        }
        storagei_offset += item_size;
    }
}

/** @} */
