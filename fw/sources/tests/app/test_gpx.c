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
 * @file    app/test_gpx.c
 * @brief   Unit tests for gpx.c
 *
 * @addtogroup tests
 * @{
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <main.h>
#include "app/gpx.c"

/* *****************************************************************************
 * Mocks
***************************************************************************** */
static size_t Storage_SpaceUsed(void)
{
    return 21;
}

static bool Storage_Get(uint32_t id, storage_item_t *item)
{
    if (id >= Storage_SpaceUsed()) {
        return false;
    }
    if (id != 0 && id % 10 == 0) {
        memset(item, 0x00, sizeof(storage_item_t));
        return true;
    }
    item->elevation_m = id;
    item->lat = 49123456;
    item->lat_scale = 1000000;
    item->lon = -123456789;
    item->lon_scale = 1000000;
    item->timestamp = id*1000;
    return true;
}

static bool Storage_IsEOL(const storage_item_t *item)
{
    const uint8_t *pos = (uint8_t *) item;

    for (size_t i = 0; i < sizeof(storage_item_t); i++) {
        if (*pos++ != 0x00) {
            return false;
        }
    }
    return true;
}

/* *****************************************************************************
 * Tests
***************************************************************************** */
TEST_GROUP(GPX);

TEST_SETUP(GPX)
{
}

TEST_TEAR_DOWN(GPX)
{
}

TEST(GPX, GetTrkpt)
{
    char buf[GPX_ITEM_SIZE + 50];
    char expected[] = \
        "      <trkpt lat=\"49.123456\" lon=\"-123.456789\">\n"\
        "        <ele>1</ele>\n"\
        "        <time>1970-01-01T00:16:40Z</time>\n"\
        "      </trkpt>";

    TEST_ASSERT_TRUE(GPXi_GetTrkpt(1, buf));
    TEST_ASSERT_EQUAL(GPX_ITEM_SIZE, strlen(buf));
    TEST_ASSERT_EQUAL_STRING_LEN(expected, buf, strlen(expected));
    TEST_ASSERT_EQUAL('\n', buf[strlen(buf) - 1]);
    /* enough space to make all message fields as long as possible */
    TEST_ASSERT_EQUAL(6, (strlen(buf) + 1) - strlen(expected));

    TEST_ASSERT_FALSE(GPXi_GetTrkpt(Storage_SpaceUsed(), buf));
}

TEST(GPX, GetTrkHeader)
{
    char buf[GPX_ITEM_SIZE + 50];
    char expected[] = \
        "    </trkseg>\n"\
        "  </trk>\n"\
        "  <trk>\n"\
        "    <name>Track 01.01.1970 00:16</name>\n"\
        "    <trkseg>";

    TEST_ASSERT_TRUE(GPXi_GetTrkHeader(1, buf));
    TEST_ASSERT_EQUAL(GPX_ITEM_SIZE, strlen(buf));
    TEST_ASSERT_EQUAL_STRING_LEN(expected, buf, strlen(expected));
    TEST_ASSERT_EQUAL('\n', buf[strlen(buf) - 1]);

    TEST_ASSERT_FALSE(GPXi_GetTrkHeader(Storage_SpaceUsed(), buf));
}

TEST(GPX, Generate)
{
    uint32_t size = GPX_GetSize();
    uint8_t buf[size+512];
    uint32_t offset = 0;
    uint8_t *pos = buf;
    storage_item_t item;

    while (offset < size) {
        GPX_Get(offset, &buf[offset], 512);
        offset += 512;
    }
    TEST_ASSERT_EQUAL(size, strlen((char *)buf));

    TEST_ASSERT_EQUAL_STRING_LEN(GPX_HEADER, pos, strlen(GPX_HEADER));
    pos += strlen(GPX_HEADER);
    TEST_ASSERT_EQUAL_STRING_LEN("  <trk>", pos, 7);
    pos += GPX_ITEM_SIZE;
    for (uint32_t id = 0; id < Storage_SpaceUsed(); id++) {
        Storage_Get(id, &item);
        if (Storage_IsEOL(&item)) {
            if (id + 1 == Storage_SpaceUsed()) {
                break;
            }
            TEST_ASSERT_EQUAL_STRING_LEN("    </trkseg>", pos, 13);
        } else {
            TEST_ASSERT_EQUAL_STRING_LEN("      <trkpt", pos,  12);
        }

        pos += GPX_ITEM_SIZE;
    }
    TEST_ASSERT_EQUAL_STRING_LEN(GPX_FOOTER, pos, strlen(GPX_FOOTER));
}

TEST_GROUP_RUNNER(GPX)
{
    RUN_TEST_CASE(GPX, GetTrkpt);
    RUN_TEST_CASE(GPX, GetTrkHeader);
    RUN_TEST_CASE(GPX, Generate);
}

void Gpx_RunTests(void)
{
    RUN_TEST_GROUP(GPX);
}

/** @} */


