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
 * @file    app/test_gui.c
 * @brief   Unit tests for gui.c
 *
 * @addtogroup tests
 * @{
 */

#include <string.h>
#include <time.h>
#include "modules/cgui/cgui.c"
#include "modules/cgui/fonts.c"
#include <main.h>
#include "gui/gui.c"
#include "gui/menu.c"
#include "gui/screens.c"

static bool pixmap[SSD1306_WIDTH][SSD1306_HEIGHT];
static gps_info_t info;
static gps_sat_t sat;
static stats_t stats;

/* *****************************************************************************
 * Helpers
***************************************************************************** */
/**
 * @brief convert framebuffer content to PBM format image
 *
 * @param name  Filename to store image to
 */
static void print2pbm(const char *name)
{
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        return;
    }

    fprintf(f, "P1\n%d %d\n", SSD1306_WIDTH, SSD1306_HEIGHT);

    for (int y = 0; y < SSD1306_HEIGHT; y++) {
        for (int x = 0; x < SSD1306_WIDTH; x++) {
            if (pixmap[x][y]) {
                fputc('1', f);
            } else {
                fputc('0', f);
            }
        }
        fputc('\n', f);
    }
    fclose(f);
}

/* *****************************************************************************
 * Mocks
***************************************************************************** */
void SSD1306_Flush(void)
{
    return;
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, bool value)
{
    TEST_ASSERT_LESS_THAN(SSD1306_WIDTH, x);
    TEST_ASSERT_LESS_THAN(SSD1306_HEIGHT, y);
    pixmap[x][y] = value;
}

gps_info_t *Gps_Get(void)
{
    return &info;
}

gps_sat_t *Gps_GetSat(void)
{
    return &sat;
}

stats_t *Stats_Get(void)
{
    return &stats;
}

void Storage_Erase(void)
{

}

size_t Storage_SpaceUsed(void)
{
    return 1234;
}

size_t Storage_GetSize(void)
{
    return 5678;
}

void Stats_Init(void)
{

}

void Log_Raw(log_level_t level, const char *source,
        const char *format, ...)
{
    (void) level;
    (void) source;
    (void) format;
}
/* *****************************************************************************
 * Tests
***************************************************************************** */
TEST_GROUP(GUI);

TEST_SETUP(GUI)
{
    /* clear pix map */
    memset((bool *)pixmap, 0xff, sizeof(pixmap));
    Gui_Init();

    info.lat.num = -49123456;
    info.lat.scale = 1000000;
    info.lon.num = 162123456;
    info.lon.scale = 1000000;
    info.hdop_dm = 123;
    info.altitude_dm = 12345;
    info.timestamp = 123456;
    info.satellites = 4;

    sat.count = 4;
    sat.visible = 4;
    sat.sat[0].azimuth = 123;
    sat.sat[0].elevation = 25;
    sat.sat[0].prn = 11;
    sat.sat[0].snr = 10;

    sat.sat[1].azimuth = 11;
    sat.sat[1].elevation = 78;
    sat.sat[1].prn = 31;
    sat.sat[1].snr = 78;

    sat.sat[2].azimuth = 9;
    sat.sat[2].elevation = 12;
    sat.sat[2].prn = 2;
    sat.sat[2].snr = 0;

    sat.sat[3].azimuth = 341;
    sat.sat[3].elevation = 87;
    sat.sat[3].prn = 67;
    sat.sat[3].snr = 11;

    stats.today.dist_dm = 1123450;
    stats.today.ascend_dm = 25000;
    stats.today.descend_dm = 123;
    stats.today.time_s = 12345;

    stats.all.dist_dm = 11123450;
    stats.all.ascend_dm = 125000;
    stats.all.descend_dm = 11123;
    stats.all.time_s = 1234500;
    stats.storage_used_pct = 12;
}

TEST_TEAR_DOWN(GUI)
{
}

TEST(GUI, Popup)
{
    Gui_Popup("FooBar");
    print2pbm("popup.pbm");
}

TEST(GUI, Popup2)
{
    Gui_Popup("Foo\nBar");
    print2pbm("popup2.pbm");
}

TEST(GUI, ScrGpsFix)
{
    Guii_DrawGpsFix(&info);
    print2pbm("scr_gps_fix.pbm");
}

TEST(GUI, ScrGpsSat)
{
    Guii_DrawGpsSat(&sat);
    print2pbm("scr_gps_sat.pbm");
}

TEST(GUI, ScrStats)
{
    Guii_DrawStats(12, &info, &stats, true);
    print2pbm("scr_stats.pbm");

    Guii_DrawStats(100, &info, &stats, false);
    print2pbm("scr_stats.pbm");
}

TEST(GUI, ScrDevInfo)
{
    Guii_DrawDeviceInfo(9001, 12345);
    print2pbm("scr_devinfo.pbm");
}

TEST(GUI, ScrMenu)
{
    Gui_Menu(GUI_EVT_REDRAW);
    print2pbm("scr_menu.pbm");
}

TEST_GROUP_RUNNER(GUI)
{
    RUN_TEST_CASE(GUI, Popup);
    RUN_TEST_CASE(GUI, Popup2);
    RUN_TEST_CASE(GUI, ScrGpsFix);
    RUN_TEST_CASE(GUI, ScrGpsSat);
    RUN_TEST_CASE(GUI, ScrStats);
    RUN_TEST_CASE(GUI, ScrDevInfo);
    RUN_TEST_CASE(GUI, ScrMenu);
}

void Gui_RunTests(void)
{
    RUN_TEST_GROUP(GUI);
}

/** @} */


