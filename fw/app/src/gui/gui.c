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
 * @file    app/gui/gui.c
 * @brief   Graphical user interface
 *
 * @addtogroup app
 * @{
 */

#include <string.h>
#include "modules/log.h"
#include "modules/cgui/cgui.h"
#include "drivers/ssd1306.h"
#include "gui.h"

static bool guii_popup_shown = false;

/**
 * Show popup over current screen
 *
 * @param str   String to be displayed
 */
static void Guii_DrawPopup(const char *str)
{
    uint16_t height = 1;
    const char *c = str;

    Cgui_DrawFilledBox(10, 10, Cgui_GetWidth() - 10, Cgui_GetHeight() - 10, true);
    Cgui_DrawFilledBox(12, 12, Cgui_GetWidth() - 12, Cgui_GetHeight() - 12, false);

    while (*c != '\0') {
        if (*c == '\n') {
            height++;
        }
        c++;
    }
    height = height * Cgui_GetFontHeight();

    Cgui_Puts(14, Cgui_GetHeight()/2-height/2, str);
    SSD1306_Flush();
}

void Gui_Popup(const char *str)
{
    guii_popup_shown = true;
    Guii_DrawPopup(str);
}

void Gui_Event(gui_event_t event)
{
    static bool in_menu = false;

    /* Any button press will close popup */
    if (guii_popup_shown) {
        /* Ignore redraws when popup is show */
        if (event == GUI_EVT_REDRAW) {
            return;
        }
        guii_popup_shown = false;
        event = GUI_EVT_REDRAW;
    }

    if (in_menu) {
        in_menu = Gui_Menu(event);
        if (!in_menu) {
            event = GUI_EVT_REDRAW;
            Gui_Screens(event);
        }
    } else {
        in_menu = !Gui_Screens(event);
        if (in_menu) {
            event = GUI_EVT_ENTERED;
            Gui_Menu(event);
        }
    }
}

void Gui_Init(void)
{
    Cgui_Init(SSD1306_DrawPixel, SSD1306_WIDTH, SSD1306_HEIGHT);
}

/** @} */
