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
 * @file    app/gui.c
 * @brief   Graphical interface for user
 *
 * @addtogroup app
 * @{
 */

#include <time.h>
#include <string.h>

#include "modules/cgui/cgui.h"
#include "drivers/gps.h"
#include "storage.h"
#include "version.h"
#include "drivers/ssd1306.h"

#define GUI_WIDTH SSD1306_WIDTH
#define GUI_HEIGHT SSD1306_HEIGHT

#define GUI_MARGIN 5

typedef enum {
    GUI_SCR_NORMAL_TODAY,
    GUI_SCR_NORMAL_ALL,
    GUI_SCR_MENU,
    GUI_SCR_DEV_INFO,
    GUI_SCR_COUNT,
} gui_screen_t;

static gui_screen_t guii_screen = GUI_SCR_NORMAL_TODAY;

/**
 * Show popup over current screen
 *
 * @param str   String to be displayed
 */
static void Gui_Popup(const char *str)
{
    uint16_t len;

    Cgui_DrawFilledBox(10, 10, GUI_WIDTH - 10, GUI_HEIGHT - 10, true);
    Cgui_DrawFilledBox(12, 12, GUI_WIDTH - 12, GUI_HEIGHT - 12, false);

    len = strlen(str)*Cgui_GetFontWidth();

    Cgui_Puts(GUI_WIDTH/2 - len/2, GUI_HEIGHT/2-Cgui_GetFontHeight()/2, str);
    SSD1306_Flush();
}

/**
 * Draw screen with device connected to PC message
 */
static void Gui_ScrUSB(void)
{
    Cgui_FillScreen(0);
    Cgui_DrawFilledBox(5, 5, GUI_WIDTH - 5, GUI_HEIGHT - 5, true);
    Cgui_DrawFilledBox(10, 10, GUI_WIDTH - 10, GUI_HEIGHT - 10, false);

    Cgui_Printf(15, GUI_HEIGHT/2 - Cgui_GetFontHeight(), "Connected to\nUSB");
    SSD1306_Flush();
}

/**
 * Show current GPS informations
 *
 * @param info      GPS data
 */
static void Gui_ScrGps(const gps_info_t *info)
{
    Cgui_FillScreen(0);
    char lat_dir = 'N';
    char lon_dir = 'E';
    nmea_float_t lat = info->lat;
    nmea_float_t lon = info->lon;
    struct tm *time;

    if (lat.num < 0) {
        lat.num = -lat.num;
        lat_dir = 'S';
    }
    if (lon.num < 0) {
        lon.num = -lon.num;
        lat_dir = 'W';
    }

    time = gmtime(&info->timestamp);

    Cgui_Printf(0, 0, "%c%d.%d\n%c%d.%d\nAlt:%dm\nDOP:%dm Sat:%d\n"
            "%d:%d %d.%d.%d",
            lat_dir, lat.num/lat.scale, lat.num % lat.scale,
            lon_dir, lon.num/lon.scale, lon.num % lon.scale,
            info->altitude_dm/10, info->hdop_dm/10, info->satellites,
            time->tm_hour, time->tm_min, time->tm_mday, time->tm_mon + 1,
            time->tm_year + 1900);
    SSD1306_Flush();
}

/**
 * Display info about this device
 */
static void Gui_ScrDeviceInfo(void)
{
    int mem;

    Cgui_FillScreen(0);
    mem = (Storage_SpaceUsed()*100)/Storage_GetSize();
    Cgui_Printf(0, 0, "Mem used: %d%%\nMem: %d\nFw: v%d.%d\nHw: v%d.%d",
            mem, Storage_GetSize(), FW_MAJOR, FW_MINOR, HW_MAJOR, HW_MINOR);

    SSD1306_Flush();
}

/**
 * Draw screen with current statistics data
 *
 * @param bat_pct   Battery charge in percents
 * @param dist_m    Distance traveled in meters
 * @param ascend_m  Sum of distance ascended
 * @param descend_m Sum of distance descended
 * @param time_s    Time of the movement in seconds
 * @param since_boot    If true, screen shows data since last reboot, else since
 *                  last storage clear
 */
static void Gui_ScrNormal(uint8_t bat_pct, uint16_t hdop_m, uint32_t dist_m,
        uint32_t ascend_m, uint32_t descend_m, int32_t alt_m, uint32_t time_s,
        bool since_boot)
{
    char buf[16];

    Cgui_FillScreen(0);

    /* header - gps state, battery */
    if (hdop_m == (uint16_t) -1) {
        Cgui_Puts(0, 0, "NoFix");
    } else {
        Cgui_Printf(0, 0, "G:%dm", hdop_m);
    }
    if (!since_boot) {
        Cgui_Puts(Cgui_GetFontWidth()*7, 0, "All");
    }

    if (bat_pct == 100) {
        bat_pct = 99;
    }
    Cgui_Printf(GUI_WIDTH - Cgui_GetFontWidth()*5, 0, "B:%d%%",
            bat_pct);

    Cgui_DrawLine(0, Cgui_GetFontHeight(), GUI_WIDTH - 1, Cgui_GetFontHeight());

    /* Statistics */
    Cgui_Printf(0, Cgui_GetFontHeight()+1, "Dist: %d.%dkm",
            dist_m/1000, (dist_m%1000)/100);
    Cgui_Printf(0, Cgui_GetFontHeight()*2+1, "A:%dm", ascend_m);
    Cgui_Printf(GUI_WIDTH/2, Cgui_GetFontHeight()*2+1, "D:%dm", descend_m);
    Cgui_Printf(0, Cgui_GetFontHeight()*3+1, "Alt: %dm", alt_m);
    Cgui_Printf(0, Cgui_GetFontHeight()*4+1, "Time: %dh %dm",
            time_s/3600, (time_s/60)%60);

    SSD1306_Flush();
}

/**
 * Show system menu
 */
static void Gui_ScrMenu()
{
    Cgui_FillScreen(0);
    Cgui_Printf(0, 0, ">Device Info\n Erase memory\n Log: 1min");
    SSD1306_Flush();
}

void Gui_Init(void)
{
    Cgui_Init(SSD1306_DrawPixel, SSD1306_WIDTH, SSD1306_HEIGHT);
}

/** @} */
