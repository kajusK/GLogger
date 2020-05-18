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
 * @file    app/gui/screens.c
 * @brief   GUI screens processing
 *
 * @addtogroup app
 * @{
 */

#include <time.h>
#include <string.h>

#include "modules/cgui/cgui.h"
#include "drivers/ssd1306.h"
#include "drivers/gps.h"
#include "storage.h"
#include "stats.h"
#include "version.h"
#include "gui.h"

typedef enum {
    GUI_SCR_TODAY,     /**< Stats since boot */
    GUI_SCR_ALL,       /**< Stats since restart */
    GUI_SCR_GPS_FIX,   /**< GPS fix info */
    GUI_SCR_GPS_SAT,   /**< GPS satellite info */
    GUI_SCR_INFO,      /**< Storage info */
    GUI_SCR_COUNT,     /**< Amount of main screens */
} gui_screen_t;

/**
 * Show current GPS informations
 *
 * @param info      GPS data
 */
static void Guii_DrawGpsFix(const gps_info_t *info)
{
    Cgui_FillScreen(0);
    char lat_dir = 'N';
    char lon_dir = 'E';
    nmea_float_t lat;
    nmea_float_t lon;
    struct tm *time;

    if (info == NULL) {
        //TODO show satellites signals
        Cgui_Printf(0, 0, "No GPS fix yet");
        SSD1306_Flush();
        return;
    }

    lat = info->lat;
    lon = info->lon;
    if (lat.num < 0) {
        lat.num = -lat.num;
        lat_dir = 'S';
    }
    if (lon.num < 0) {
        lon.num = -lon.num;
        lat_dir = 'W';
    }

    time = gmtime(&info->time);

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
 * Show current GPS satellite info
 *
 * @param info      GPS data
 */
static void Guii_DrawGpsSat(const gps_sat_t *sat)
{
    const uint8_t margin = 3;
    const uint8_t width = 5;
    uint8_t i;
    uint16_t height;
    uint16_t x = margin;
    uint16_t top = Cgui_GetFontHeight() + margin;
    uint16_t bottom = Cgui_GetHeight() - margin;

    Cgui_FillScreen(0);
    Cgui_Printf(0, 0, "Gps sats: %d", sat->visible);

    for (i = 0; i < sat->count && x <= Cgui_GetWidth() - width; i++) {
        height = ((bottom - top) * sat->sat[i].snr) / MAX_SV_SNR;
        Cgui_DrawFilledBox(x, bottom-height, x+width, bottom, true);
        x += width+margin;
    }
    SSD1306_Flush();
}

/**
 * Display info about this device
 */
static void Guii_DrawDeviceInfo(uint32_t mem_used, uint32_t mem_size)
{
    Cgui_FillScreen(0);

    Cgui_Printf(0, 0, "Mem used: %d%%\nMem: %d\nFw: v%d.%d\nHw: v%d.%d",
            mem_used*100/mem_size, mem_size,
            FW_MAJOR, FW_MINOR, HW_MAJOR, HW_MINOR);

    SSD1306_Flush();
}

/**
 * Draw screen with current statistics data
 *
 * @param bat_pct   Battery charge in percents
 * @param gps       Gps data
 * @param stats     Statistics data
 * @param today     If true, screen shows data since last reboot, else since
 *                  last storage clear
 */
static void Guii_DrawStats(uint8_t bat_pct, const gps_info_t *gps,
        const stats_t *stats, bool today)
{
    const stats_comm_t *data;

    Cgui_FillScreen(0);

    /* header - gps state, battery */
    if (gps == NULL) {
        Cgui_Puts(0, 0, "NoFix");
    } else {
        Cgui_Printf(0, 0, "G:%dm", gps->hdop_dm/10);
    }
    if (!today) {
        Cgui_Puts(Cgui_GetFontWidth()*7, 0, "All");
        data = &stats->all;
    } else {
        data = &stats->today;
    }

    if (bat_pct == 100) {
        bat_pct = 99;
    }
    Cgui_Printf(Cgui_GetWidth() - Cgui_GetFontWidth()*5, 0, "B:%d%%",
            bat_pct);

    Cgui_DrawLine(0, Cgui_GetFontHeight(), Cgui_GetWidth() - 1, Cgui_GetFontHeight());

    /* Statistics */
    Cgui_Printf(0, Cgui_GetFontHeight()+1, "Dist: %d.%dkm",
            data->dist_dm/10/1000, ((data->dist_dm/10)%10000)/100);
    Cgui_Printf(0, Cgui_GetFontHeight()*2+1, "A:%dm", data->ascend_dm/10);
    Cgui_Printf(Cgui_GetWidth()/2, Cgui_GetFontHeight()*2+1, "D:%dm", data->descend_dm/10);
    Cgui_Printf(0, Cgui_GetFontHeight()*3+1, "Alt: %dm", gps != NULL ? gps->altitude_dm/10 : 0);
    Cgui_Printf(0, Cgui_GetFontHeight()*4+1, "Time: %dh %dm",
            data->time_s/3600, (data->time_s/60)%60);

    SSD1306_Flush();
}

bool Gui_Screens(gui_event_t event)
{
    static gui_screen_t scr = GUI_SCR_TODAY;
    uint8_t bat_pct = 0;

    switch (event) {
        case GUI_EVT_ENTERED:
        case GUI_EVT_LONG_NEXT:
            scr = 0;
            break;
        case GUI_EVT_SHORT_NEXT:
            scr += 1;
            if (scr >= GUI_SCR_COUNT) {
                scr = 0;
            }
            break;
        case GUI_EVT_SHORT_ENTER:
            return false;
            break;
        default:
            break;
    }

    switch (scr) {
        case GUI_SCR_TODAY:
            Guii_DrawStats(bat_pct, Gps_Get(), Stats_Get(), true);
            break;
        case GUI_SCR_ALL:
            Guii_DrawStats(bat_pct, Gps_Get(), Stats_Get(), false);
            break;
        case GUI_SCR_GPS_FIX:
            Guii_DrawGpsFix(Gps_Get());
            break;
        case GUI_SCR_GPS_SAT:
            Guii_DrawGpsSat(Gps_GetSat());
            break;
        case GUI_SCR_INFO:
            Guii_DrawDeviceInfo(Storage_SpaceUsed(), Storage_GetSize());
            break;
        default:
            break;
    }

    return true;
}

/** @} */
