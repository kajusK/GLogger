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
 * @file    drivers/test_ramdisk.c
 * @brief   Unit tests for ramdisk.c
 *
 * @addtogroup tests
 * @{
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <main.h>
#include "drivers/ramdisk.c"

#define RAMDISK_NAME "name"

#define RAMDISK_TEXT "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur volutpat augue vel metus condimentum pharetra. In mattis, mauris non tincidunt maximus, augue felis iaculis eros, vitae rutrum ipsum diam ac elit. Sed volutpat pellentesque libero, eu tempor elit hendrerit vel. Nulla vehicula arcu ac turpis pulvinar, vestibulum rhoncus elit rutrum. Nullam interdum bibendum tortor, at finibus lorem fringilla in. Maecenas vehicula ipsum eu gravida aliquam. Nullam non purus sit amet felis dignissim scelerisque. Proin nec viverra mi, ut viverra orci. Maecenas scelerisque sagittis vestibulum. Mauris eu aliquam erat, vel pharetra velit. Ut tellus lectus, fermentum non odio in, accumsan blandit magna.\
Suspendisse et volutpat turpis, quis condimentum nisl. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Ut finibus ante eu maximus eleifend. Ut nulla tortor, iaculis nec est vel, laoreet euismod purus. In mattis felis sed pharetra dignissim. Aliquam varius, enim rhoncus auctor vulputate, magna leo sollicitudin elit, efficitur porta eros sapien quis neque. Quisque odio lacus, porta nec pretium sed, sodales quis diam. Praesent nibh magna, placerat vel placerat a, mattis nec leo.\
Nulla sed luctus nisl. Proin egestas, tortor vitae vestibulum vestibulum, sem leo commodo magna, ut ornare quam ligula sit amet sem. Aenean leo lorem, elementum nec suscipit at, vulputate efficitur massa. Praesent nunc ipsum, ornare in tortor eu, congue cursus augue. Nam sit amet venenatis diam. Fusce tincidunt et risus eleifend volutpat. Aliquam sit amet lectus metus. Fusce porttitor lacinia dolor id sodales. Sed placerat semper gravida. Aenean magna leo, interdum ut ipsum eu, elementum gravida lectus. Sed facilisis eleifend urna sit amet molestie. Aliquam eu tincidunt est. Suspendisse lobortis volutpat urna. Ut id tellus in nunc malesuada sodales. In non feugiat neque, at laoreet lorem. Ut eget purus diam.\
Morbi condimentum suscipit ante, nec venenatis sem consequat ut. Nullam dignissim ornare justo, eu ultricies sem rhoncus vitae. Mauris at augue eu ipsum tristique interdum. Etiam quis diam et sapien rhoncus vehicula et eu sem. Praesent eget quam sit amet urna ultricies sagittis interdum quis ligula. Pellentesque varius lorem eget tortor rutrum faucibus. Ut id dui urna. Quisque suscipit efficitur tortor, non condimentum turpis. Ut imperdiet mattis leo quis sodales. Vestibulum facilisis ligula ut fringilla suscipit.\
Sed sagittis ipsum rhoncus nibh luctus convallis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aenean mollis nibh et ligula gravida, at tincidunt tellus tincidunt. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Ut ac tortor ut justo elementum laoreet sit amet ac erat. Duis commodo lectus eget est aliquam, id porta sem fermentum. Suspendisse fermentum volutpat tellus, sed suscipit lorem faucibus eu. Quisque id vulputate elit, id iaculis turpis. Nunc cursus, enim ultrices mollis dictum, ligula diam dictum enim, id consectetur nibh dui non turpis. \
"

/* *****************************************************************************
 * Helpers
***************************************************************************** */
static void Ramdisk_File1(uint32_t offset, uint8_t *buf, size_t len)
{
    (void) offset;
    memset(buf, 'a', len);
}

static void Ramdisk_File2(uint32_t offset, uint8_t *buf, size_t len)
{
    (void) offset;
    memset(buf, 'b', len);
}

/* *****************************************************************************
 * Tests
***************************************************************************** */
TEST_GROUP(RAMDISK);

TEST_SETUP(RAMDISK)
{
    struct tm time;

    /* 16 MB ramdisk */
    Ramdisk_Init(0, RAMDISK_NAME);

    time.tm_hour = 12;
    time.tm_min = 32;
    time.tm_sec = 11;
    time.tm_mday = 11;
    time.tm_mon = 6;
    time.tm_year = 119;
    TEST_ASSERT_TRUE(Ramdisk_AddFile("Foo", "br", mktime(&time), 12000000, Ramdisk_File1));
    TEST_ASSERT_TRUE(Ramdisk_AddFile("bar", "txt", mktime(&time), 180000, Ramdisk_File2));
    TEST_ASSERT_TRUE(Ramdisk_AddTextFile("lorem", "txt", mktime(&time), RAMDISK_TEXT));
}

TEST_TEAR_DOWN(RAMDISK)
{
    Ramdisk_Clear();
}

TEST(RAMDISK, BootSector)
{
    uint8_t buf[SECTOR_SIZE];

    Ramdisk_Read(0, buf);

    TEST_ASSERT_EQUAL_STRING_LEN("mkdosfs", &buf[0x03], 8);
    /* sector size */
    TEST_ASSERT_EQUAL(512, buf[0x0b] | buf[0x0c] << 8);
    /* Sectors per cluster */
    TEST_ASSERT_EQUAL(SECTORS_PER_CLUSTER, buf[0x0d]);
    /* Reserved sectors */
    TEST_ASSERT_EQUAL(1, buf[0x0e] | buf[0x0f] << 8);
    /* fat copies */
    TEST_ASSERT_EQUAL(2, buf[0x10]);
    /* fat table size */
    TEST_ASSERT_EQUAL(ramdiski_info.fat_sectors, buf[0x16] | buf[0x17] << 8);
    /* volume size in sectors */
    if (ramdiski_info.sectors_count < 65535) {
        TEST_ASSERT_EQUAL(ramdiski_info.sectors_count, buf[0x13] | buf[0x14] << 8);
        TEST_ASSERT_EQUAL(0, buf[0x20] | buf[0x21] << 8 | buf[0x22] << 16 | buf[0x23] << 24);
    } else {
        TEST_ASSERT_EQUAL(0, buf[0x13] | buf[0x14] << 8);
        TEST_ASSERT_EQUAL(ramdiski_info.sectors_count, buf[0x20] | buf[0x21] << 8 | buf[0x22] << 16 | buf[0x23] << 24);
    }

    /* volume label */
    TEST_ASSERT_EQUAL_STRING_LEN(RAMDISK_NAME "       ", &buf[0x2b], 11);
    /* volume type */
    TEST_ASSERT_EQUAL_STRING_LEN("FAT16   ", &buf[0x36], 8);
    /* signature */
    TEST_ASSERT_EQUAL_HEX8(0x55, buf[0x1fe]);
    TEST_ASSERT_EQUAL_HEX8(0xaa, buf[0x1ff]);
}

TEST(RAMDISK, FatTable)
{
    uint8_t buf[SECTOR_SIZE];
    uint16_t offset;

    Ramdisk_Read(1, buf);
    TEST_ASSERT_EQUAL_HEX8(0xf8, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0xff, buf[1]);
    TEST_ASSERT_EQUAL_HEX8(0xff, buf[2]);
    TEST_ASSERT_EQUAL_HEX8(0xff, buf[3]);

    offset = 4;
    for (unsigned i = 3; i < SECTOR_SIZE*2; i++) {
        TEST_ASSERT_EQUAL_HEX16(i, buf[offset] | buf[offset+1] << 8);
        offset += 2;
        if (offset >= SECTOR_SIZE) {
            offset = 0;
            Ramdisk_Read(1 + i / (SECTOR_SIZE/2), buf);
        }
    }
}

TEST(RAMDISK, RootDirectory)
{
    uint16_t offset;
    uint16_t cluster;
    uint8_t buf[SECTOR_SIZE];

    Ramdiski_GetRootDirectory(buf, 0);

    /* volume label */
    TEST_ASSERT_EQUAL_STRING_LEN(RAMDISK_NAME"       ", &buf[0], 11);
    /* attribute */
    TEST_ASSERT_EQUAL_HEX8(0x08, buf[0x0b]);
    /* start cluster */
    TEST_ASSERT_EQUAL_HEX8(0, buf[0x1a]);
    TEST_ASSERT_EQUAL_HEX8(0, buf[0x1b]);

    /* file 1 */
    offset = 32;
    TEST_ASSERT_EQUAL_STRING_LEN("Foo     ", &buf[offset], 8);
    TEST_ASSERT_EQUAL_STRING_LEN("br ", &buf[offset+8], 3);
    TEST_ASSERT_EQUAL_HEX8(0x01, buf[offset+0xb]);
    /* time 12:32:11 */
    TEST_ASSERT_EQUAL_HEX8(5 | ((32 << 5) & 0x1f), buf[offset+0x16]);
    TEST_ASSERT_EQUAL_HEX8((32 >> 3) | (12 << 3), buf[offset+0x17]);
    /* date 11.7.2019 */
    TEST_ASSERT_EQUAL_HEX8(11 | ((7 & 0x1f) << 5), buf[offset+0x18]);
    TEST_ASSERT_EQUAL_HEX8((7 >> 3) | 39 << 1, buf[offset+0x19]);
    /* starting cluster */
    TEST_ASSERT_EQUAL(2, buf[offset+0x1a] | buf[offset+0x1b] << 8);
    /* file size */
    TEST_ASSERT_EQUAL(12000000, buf[offset+0x1c] | buf[offset+0x1d] << 8 | buf[offset+0x1e] << 16 | buf[offset+0x1f] << 24);

    /* file 2 */
    offset += 32;
    TEST_ASSERT_EQUAL_STRING_LEN("bar     ", &buf[offset], 8);
    TEST_ASSERT_EQUAL_STRING_LEN("txt", &buf[offset+8], 3);
    /* cluster */
    cluster = 2 + ceil_div(12000000, CLUSTER_SIZE);
    TEST_ASSERT_EQUAL(cluster, buf[offset+0x1a] | buf[offset+0x1b] << 8);
    /* file size */
    TEST_ASSERT_EQUAL(180000, buf[offset+0x1c] | buf[offset+0x1d] << 8 | buf[offset+0x1e] << 16 | buf[offset+0x1f] << 24);
}

TEST(RAMDISK, GetFile)
{
    uint8_t buf[SECTOR_SIZE];

    Ramdiski_GetFile(buf, 0);
    TEST_ASSERT_EACH_EQUAL_HEX8('a', buf, SECTOR_SIZE);
    Ramdiski_GetFile(buf, 1);
    TEST_ASSERT_EACH_EQUAL_HEX8('a', buf, SECTOR_SIZE);
    Ramdiski_GetFile(buf, 12000000/SECTOR_SIZE+SECTORS_PER_CLUSTER);
    TEST_ASSERT_EACH_EQUAL_HEX8('b', buf, SECTOR_SIZE);
}

/*
 * Generate example ramdisk for optional checking with external tool
 */
TEST(RAMDISK, GenerateRamdisk)
{
    uint8_t buf[512];
    uint32_t sectors = Ramdisk_GetSectors();
    FILE *f;

    f = fopen("ramdisk.img", "wb");
    TEST_ASSERT_NOT_NULL(f);

    for (uint32_t i = 0; i < sectors; i++) {
        Ramdisk_Read(i, buf);
        fwrite(buf, 1, 512, f);
    }
    fclose(f);
}

TEST_GROUP_RUNNER(RAMDISK)
{
    RUN_TEST_CASE(RAMDISK, BootSector);
    RUN_TEST_CASE(RAMDISK, FatTable);
    RUN_TEST_CASE(RAMDISK, RootDirectory);
    RUN_TEST_CASE(RAMDISK, GetFile);
    RUN_TEST_CASE(RAMDISK, GenerateRamdisk);
}

void Ramdisk_RunTests(void)
{
    RUN_TEST_GROUP(RAMDISK);
}

/** @} */


