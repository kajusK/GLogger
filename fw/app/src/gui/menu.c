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
 * @file    app/gui/menu.c
 * @brief   GUI menu processing
 *
 * @addtogroup app
 * @{
 */

#include <string.h>
#include "modules/log.h"
#include "drivers/ssd1306.h"
#include "modules/cgui/cgui.h"
#include "storage.h"
#include "stats.h"
#include "version.h"
#include "gui.h"

/**
 * Callback for menu action request
 *
 * @return If true, return to previous menu, else stay where you are
 */
typedef bool (*gui_menu_action_cb_t)(void);

/**
 * Callback to get currently selected item from list of values
 * @return Selected value index
 */
typedef uint8_t (*gui_menu_value_get_cb_t)(void);

/**
 * Callback to set newly selected item from list of values
 * @param index     Selected value index
 */
typedef void (*gui_menu_value_set_cb_t)(uint8_t index);


/** Possible types of menu items */
typedef enum {
    GUI_MENU_SUBMENU,   /**< Enter submenu */
    GUI_MENU_VALUES,    /**< List of values that can be selected */
    GUI_MENU_ACTION,    /**< Action will happen after entering this item */
    GUI_MENU_BACK,      /**< Return to previous menu */
    GUI_MENU_EMPTY,     /**< This item marks end of menu items */
} gui_menu_type_t;

/** Values list for the menu */
typedef const struct {
    char *const (*list)[];              /**< List of possible values */
    uint8_t count;                      /**< Amount of values */
    gui_menu_value_get_cb_t get_cb;     /**< Get currently selected value */
    gui_menu_value_set_cb_t set_cb;     /**< Value selected */
} gui_menu_values_t;

/** Single menu item */
typedef const struct {
    const char *name;       /**< Name of this menu item */
    gui_menu_type_t type;   /**< Type of this menu item */

    union {
        struct gui_menu_t *submenu;         /**< Pointer to submenu */
        gui_menu_action_cb_t action_cb;     /**< Callback for action run */
        const gui_menu_values_t *values;    /**< List of selectable values */
    };
} gui_menu_item_t;

/** Description of the menu */
typedef struct gui_menu_t {
    const char *name;                   /**< Name of this menu */
    const gui_menu_item_t (*items)[];   /**< List of menu items */
    struct gui_menu_t *prev;            /**< Pointer to previous menu */
    uint8_t cursor;                     /**< Cursor pointer to item */
    uint8_t rot;                        /**< Screen rotated to index on top */
} gui_menu_t;

/**
 * Draw currently used menu to display
 *
 * @param menu      Menu structure
 */
static void Guii_DrawMenu(gui_menu_t *menu)
{
    uint16_t y = 0;
    uint16_t x = 0;
    uint8_t lines;
    uint8_t end;
    uint8_t i;
    const gui_menu_item_t *item;

    Cgui_FillScreen(0);

    Cgui_Puts(0, 0, menu->name);
    y = Cgui_GetFontHeight() + 1;
    Cgui_DrawLine(0, y, Cgui_GetWidth()-1, y);
    y += 1;

    lines = (Cgui_GetHeight() - y) / Cgui_GetFontHeight();
    if (abs(menu->rot - menu->cursor) >= lines) {
        menu->rot += menu->rot > menu->cursor ? -1 : 1;
    }

    i = menu->rot;
    end = menu->rot + lines - 1;
    while (i <= end && (*menu->items)[i].type != GUI_MENU_EMPTY) {
        item = &(*menu->items)[i];
        if (i == menu->cursor) {
            Cgui_Putc(0, y, '>');
        }
        Cgui_Puts(Cgui_GetFontWidth(), y, item->name);
        if (item->type == GUI_MENU_VALUES &&
                item->values->get_cb() < item->values->count) {
            x = Cgui_GetFontWidth() * (strlen(item->name) + 1);
            Cgui_Putc(x, y, ':');
            x += Cgui_GetFontWidth();
            Cgui_Puts(x, y, (*item->values->list)[item->values->get_cb()]);
        }
        y += Cgui_GetFontHeight();
        i++;
    }
    SSD1306_Flush();
}

/**
 * Run storage erase action
 */
static bool Guii_StorageErase(void)
{
    Gui_Popup("Erasing...\n");
    Storage_Erase();
    Stats_Init();
    Gui_Popup("Erasing\nfinished");
    return true;
}

uint8_t val = 0;
static uint8_t test_get(void)
{
    return val;
}

static void test_set(uint8_t index)
{
    val = index;
}

/**
 * Run system info action
 */
static bool Guii_SysInfo(void)
{
    Cgui_FillScreen(0);
    uint32_t mem_used = Storage_SpaceUsed();
    uint32_t mem_size = Storage_GetSize();
    Cgui_Printf(0, 0, "Deadbadger.cz\nMem used: %d%%\nMem: %d\nFw: v%d.%d\nHw: v%d.%d",
            mem_used*100/mem_size, mem_size,
            FW_MAJOR, FW_MINOR, HW_MAJOR, HW_MINOR);

    SSD1306_Flush();
    Gui_CustomPopup();
    return false;
}

/** System menu description */
static gui_menu_t guii_menu = {
    .name = "Menu",
    .items = &(gui_menu_item_t []) {
        { .name = "System info",
          .type = GUI_MENU_ACTION,
          .action_cb = Guii_SysInfo,
        },
        { .name = "Erase memory",
          .type = GUI_MENU_SUBMENU,
          .submenu = &(gui_menu_t) {
                .name = "Erase Memory",
                .items = &(const gui_menu_item_t[]) {
                    { .name = "No", .type = GUI_MENU_BACK },
                    { .name = "No", .type = GUI_MENU_BACK },
                    { .name = "Yes", .type = GUI_MENU_ACTION,
                            .action_cb = Guii_StorageErase },
                    { .name = "No", .type = GUI_MENU_BACK },
                    { .name = "Back", .type = GUI_MENU_BACK },
                    { .type = GUI_MENU_EMPTY },
                },
            },
        },
        { .name = "Log period",
          .type = GUI_MENU_VALUES,
          .values = &(gui_menu_values_t){
                .list = &(char *const[]){"auto", "1s", "10s", "1m", "10m"},
                .count = 5,
                .get_cb = test_get,
                .set_cb = test_set,
            }
        },
#if 0
        { .name = "Charging",
          .type = GUI_MENU_VALUES,
          .values = &(gui_menu_values_t){
                .list = &(char *const[]){"en", "dis"},
                .count = 2,
                .get_cb = test_get,
                .set_cb = test_set,
          }
        },
        { .name = "Acceler",
          .type = GUI_MENU_VALUES,
          .values = &(gui_menu_values_t) {
                .list = &(char *const[]){"en", "dis"},
                .count = 2,
                .get_cb = test_get,
                .set_cb = test_set,
          }
        },
#endif
        /*
         * TODO debug screen - accelerometer calibration, raw data, manual
         * power control (lcd, gps, accel, whole unit),...
         */

        { .name = "Back", .type = GUI_MENU_BACK },
        { .type = GUI_MENU_EMPTY },
    },
};

bool Gui_Menu(gui_event_t event)
{
    static gui_menu_t *current = &guii_menu;
    const gui_menu_item_t *item = &(*current->items)[current->cursor];

    switch (event) {
        case GUI_EVT_ENTERED:
            current->cursor = 0;
            current->rot = 0;
            break;
        case GUI_EVT_SHORT_NEXT:
            current->cursor += 1;
            if ((*current->items)[current->cursor].type == GUI_MENU_EMPTY) {
                current->cursor = 0;
                current->rot = 0;
            }
            break;
        case GUI_EVT_SHORT_ENTER:
            switch (item->type) {
                case GUI_MENU_SUBMENU:
                    if (item->submenu == NULL) {
                        Log_Warning("GUI", "Empty submenu");
                    } else {
                        item->submenu->prev = current;
                        current = item->submenu;
                        current->cursor = 0;
                        current->rot = 0;
                    }
                    break;
                case GUI_MENU_BACK:
                    if (current->prev == NULL) {
                        return false;
                    }
                    current = current->prev;
                    break;
                case GUI_MENU_ACTION:
                    if (item->action_cb == NULL) {
                        Log_Warning("GUI", "Empty action");
                    } else {
                        if (item->action_cb()) {
                            if (current->prev == NULL) {
                                return false;
                            }
                            current = current->prev;
                        }
                        /* action could draw over menu, don't redraw */
                        return true;
                    }
                    break;
                case GUI_MENU_VALUES:
                    if (item->values->get_cb() >= item->values->count - 1) {
                        item->values->set_cb(0);
                    } else {
                        item->values->set_cb(item->values->get_cb() + 1);
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    Guii_DrawMenu(current);
    return true;
}

/** @} */
