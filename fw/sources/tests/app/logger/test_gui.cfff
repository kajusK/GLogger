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
#include "app/logger/gui.c"

static bool pixmap[GUI_WIDTH][GUI_HEIGHT];

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

    fprintf(f, "P1\n%d %d\n", GUI_WIDTH, GUI_HEIGHT);

    for (int y = 0; y < GUI_HEIGHT; y++) {
        for (int x = 0; x < GUI_WIDTH; x++) {
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
static void SSD1306_Flush(void)
{
    return;
}

static void SSD1306_DrawPixel(uint16_t x, uint16_t y, bool value)
{
    TEST_ASSERT_LESS_THAN(GUI_WIDTH, x);
    TEST_ASSERT_LESS_THAN(GUI_HEIGHT, y);
    pixmap[x][y] = value;
}

static size_t Storage_GetSize(void)
{
    return 100;
}

static size_t Storage_SpaceUsed(void)
{
    return 62;
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
}

TEST_TEAR_DOWN(GUI)
{
}

TEST(GUI, Popup)
{
    Gui_ScrNormal(72, 123, 128762, 1451, 892, 1234, 433482, false);
    Gui_Popup("FooBar");
    print2pbm("popup.pbm");
}

TEST(GUI, ScrUSB)
{
    Gui_ScrUSB();
    print2pbm("scr_usb.pbm");
}

TEST(GUI, ScrGps)
{
    gps_info_t info;

    info.lat.num = -49123456;
    info.lat.scale = 1000000;
    info.lon.num = 162123456;
    info.lon.scale = 1000000;

    info.hdop_dm = 123;
    info.altitude_dm = 12345;
    info.timestamp = 123456;
    info.satellites = 4;
    Gui_ScrGps(&info);
    print2pbm("scr_gps.pbm");
}

TEST(GUI, ScrNormal)
{
    Gui_ScrNormal(72, 123, 128762, 1451, 892, 1235, 433482, false);
    print2pbm("scr_normal.pbm");

    Gui_ScrNormal(100, (uint16_t)-1, 123, 112, 0, -12, 433482, true);
    print2pbm("scr_normal2.pbm");
}

TEST(GUI, ScrDevInfo)
{
    Gui_ScrDeviceInfo();
    print2pbm("scr_devinfo.pbm");
}

TEST(GUI, ScrMenu)
{
    Gui_ScrMenu();
    print2pbm("scr_menu.pbm");

}

TEST_GROUP_RUNNER(GUI)
{
    RUN_TEST_CASE(GUI, Popup);
    RUN_TEST_CASE(GUI, ScrUSB);
    RUN_TEST_CASE(GUI, ScrGps);
    RUN_TEST_CASE(GUI, ScrNormal);
    RUN_TEST_CASE(GUI, ScrDevInfo);
    RUN_TEST_CASE(GUI, ScrMenu);
}

void Gui_RunTests(void)
{
    RUN_TEST_GROUP(GUI);
}

/** @} */


