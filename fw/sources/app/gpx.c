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
 * @file    app/gpx.c
 * @brief   GPX file generator
 *
 * @addtogroup app
 * @{
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "app/storage.h"
#include "app/gpx.h"

#define GPX_ITEM_SIZE 130

#define GPX_LATLON_SCALE 1000000

/** XML header of the gpx file */
#define GPX_HEADER \
    "<?xml version=\"1.0\"?>\n"\
    "<gpx version=\"1.0\" creator=\"GPSBabel - http://www.gpsbabel.org\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/0\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd\">\n"\
    "  <metadata>\n"\
    "    <link href=\"http://www.deadbadger.cz\">\n"\
    "      <text>Deadbadger.cz</text>\n"\
    "    </link>\n"\
    "  </metadata>\n"

#define GPX_FOOTER "    </trkseg>\n  </trk>\n</gpx>"

/**
 * Generate trk header with constant length equal to GPX_ITEM_SIZE
 *
 * If id item was not found (last item), generate gpx footer
 *
 * @param id    First item of the track id
 * @param buf   Target buffer to generate data to (length GPX_ITEM_SIZE + 1)
 * @return  False if item of given id not found
 */
static bool GPXi_GetTrkHeader(uint32_t id, char *buf)
{
    storage_item_t item;
    struct tm *time;
    time_t timestamp;
    uint32_t len = 0;

    if (Storage_Get(id, &item) == false) {
        return false;
    }

    if (id != 0) {
        snprintf(buf, GPX_ITEM_SIZE, "    </trkseg>\n  </trk>\n");
        len = strlen(buf);
    }

    timestamp = item.timestamp;
    time = gmtime(&timestamp);
    snprintf(buf + len, GPX_ITEM_SIZE,
            "  <trk>\n"\
            "    <name>Track %02d.%02d.%04d %02d:%02d</name>\n"\
            "    <trkseg>",
            time->tm_mday, time->tm_mon + 1, time->tm_year + 1900,
            time->tm_hour, time->tm_min);

    len = strlen(buf);
    while (len < GPX_ITEM_SIZE - 1) {
        buf[len++] = ' ';
    }
    buf[len++] = '\n';
    buf[len] = '\0';
    return true;
}

/**
 * Generate single track point item of constant length equal to GPX_ITEM_SIZE
 *
 * @param id    Item id
 * @param buf   Target buffer to generate data to (length GPX_ITEM_SIZE + 1)
 * @return  False if item of given id not found
 */
static bool GPXi_GetTrkpt(uint32_t id, char *buf)
{
    storage_item_t item;
    struct tm *time;
    time_t timestamp;
    int32_t lat_deg, lon_deg;
    int32_t lat_dec, lon_dec;
    uint32_t len;

    if (Storage_Get(id, &item) == false) {
        return false;
    }

    if (Storage_IsEOL(&item)) {
        return GPXi_GetTrkHeader(id + 1, buf);
    }

    lat_deg = item.lat / item.lat_scale;
    lat_dec = abs(item.lat - lat_deg * item.lat_scale);
    /* Round to given amount of decimal places */
    if (item.lat_scale <= GPX_LATLON_SCALE) {
        lat_dec *= GPX_LATLON_SCALE/item.lat_scale;
    } else {
        lat_dec /= item.lat_scale/GPX_LATLON_SCALE;
    }

    lon_deg = item.lon / item.lon_scale;
    lon_dec = abs(item.lon - lon_deg * item.lon_scale);
    if (item.lon_scale <= GPX_LATLON_SCALE) {
        lon_dec *= GPX_LATLON_SCALE/item.lon_scale;
    } else {
        lon_dec /= item.lon_scale/GPX_LATLON_SCALE;
    }

    timestamp = item.timestamp;
    time = gmtime(&timestamp);

    /*
     * typed to long to work on both arm and x64 cpu (unit tests)
     * sizeof(int) != sizeof(int32_t) == sizeof(long int) on ARM
     */
    snprintf(buf, GPX_ITEM_SIZE - 1,
            "      <trkpt lat=\"%ld.%ld\" lon=\"%ld.%ld\">\n"\
            "        <ele>%d</ele>\n"\
            "        <time>%4d-%02d-%02dT%02d:%02d:%02dZ</time>\n"\
            "      </trkpt>",
            (long)lat_deg, (long)lat_dec, (long)lon_deg, (long)lon_dec,
            item.elevation_m,
            time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec);

    len = strlen(buf);
    while (len < GPX_ITEM_SIZE - 1) {
        buf[len++] = ' ';
    }
    buf[len++] = '\n';
    buf[len] = '\0';

    return true;
}

/**
 * Get size of the GPX file
 *
 * @return  Size of the file in bytes
 */
uint32_t GPX_GetSize(void)
{
    uint32_t items = Storage_SpaceUsed();
    storage_item_t item;

    Storage_Get(items-1, &item);
    if (!Storage_IsEOL(&item)) {
        items += 1;
    }

    return strlen(GPX_HEADER)+items*GPX_ITEM_SIZE + strlen(GPX_FOOTER);
}

/**
 * Read logged data and generate gpx file on the fly
 *
 * @param offset        Offset to the file
 * @param buf           Buffer to store data to
 * @param len           Amount of bytes to store
 * @return  true if succeeded
 */
bool GPX_Get(uint32_t offset, uint8_t *buf, uint32_t len)
{
    char *header = GPX_HEADER;
    uint32_t header_len;
    uint32_t bytes;
    uint32_t id;
    uint8_t itembuf[GPX_ITEM_SIZE+1];

    header_len = strlen(GPX_HEADER);

    if (offset < header_len) {
        if (offset + len > header_len) {
            bytes = header_len - offset;
        } else {
            bytes = len;
        }
        memcpy(buf, &header[offset], bytes);
        if (len == bytes) {
            return true;
        }
        offset += bytes;
        len -= bytes;
        buf += bytes;
    }

    if (offset > header_len) {
        offset -= header_len;
    } else {
        offset = 0;
    }
    id = offset / GPX_ITEM_SIZE;
    offset %= GPX_ITEM_SIZE;

    while (len != 0) {
        bytes = GPX_ITEM_SIZE - offset;
        if (bytes > len) {
            bytes = len;
        }
        if (id == 0) {
            GPXi_GetTrkHeader(0, (char *) itembuf);
        } else if (id - 1 == Storage_SpaceUsed()) {
            strcpy((char *) buf, GPX_FOOTER);
            return true;
        } else {
            if (GPXi_GetTrkpt(id - 1, (char *) itembuf) == false) {
                bytes = 0;
            }
        }
        memcpy(buf, &itembuf[offset], bytes);
        offset = 0;
        len -= bytes;
        buf += bytes;
        id++;
    }

    return true;
}

/** @} */
